#include <VoxelSpaceToVTK.h>

namespace lignumvtk{

  VoxelSpaceToVTK::VoxelSpaceToVTK(int nx, int ny, int nz)
  {
    sgrid->SetDimensions(nx,ny,nz);
    vtkNew<vtkPoints> points;
    points->Allocate(nx * ny * nz);
    for (int k = 0; k < nz; k++) {
        for (int j = 0; j < ny; j++) {
            for (int i = 0; i < nx; i++) {
                // Define actual 3D coordinates
                // For a straight regular grid, these can just be i, j, k
                points->InsertNextPoint(static_cast<double>(i),
					static_cast<double>(j),
                                        static_cast<double>(k));
            }
        }
    }
    sgrid->SetPoints(points);
  }
	
  VoxelSpaceToVTK::VoxelSpaceToVTK(int nx, int ny, int nz, double x_edge, double y_edge, double z_edge)
  {
    sgrid->SetDimensions(nx,ny,nz);
    vtkNew<vtkPoints> points;
    points->Allocate(nx * ny * nz);
    //Note these structured bindings may generate warnings but are available since C++17
    for (auto [k,pz] = std::pair<int,double>{0,0.0}; k < nz; k++, pz = pz + z_edge) {
      for (auto [j,py] = std::pair<int,double>{0,0.0}; j < ny; j++, py = py + y_edge) {
	for (auto [i,px] = std::pair<int,double>{0,0.0}; i < nx; i++, px = px + x_edge) {
                // Define actual 3D coordinates (x, y, z)
                // For a straight regular grid, these start from (0,0,0) with edge size increments
                points->InsertNextPoint(static_cast<double>(px),
					static_cast<double>(py), 
                                        static_cast<double>(pz));
            }
        }
    }
    sgrid->SetPoints(points);
  }
  
  VoxelSpaceToVTK& VoxelSpaceToVTK::insertVoxelSpaceData(const cxxadt::TMatrix4D<double>& vsm,const vector<string>& attr_names)
  {
    int dims[3]{0,0,0};
    sgrid->GetDimensions(dims);
    //Make assertions: right matrix size and right number of attributes
    assert(dims[0] == vsm.a() && dims[1] == vsm.b() && dims[2] == vsm.c() && "Failed Right matrix size");
    assert(vsm.d() == attr_names.size() && "Failed Right number of attribute names");
    //Attribute names
    vector<vtkNew<vtkDoubleArray>> v_attr_cell(attr_names.size());
    for (int i = 0; i < v_attr_cell.size();i++){
      v_attr_cell[i]->SetName(attr_names[i].c_str());
      //Space for attribute data
      v_attr_cell[i]->SetNumberOfTuples(sgrid->GetNumberOfCells());
    }
    //Cell attribute values
    //Note: number of cells (i-1)*(j-1)*(k-1) vs. number of points i*j*k
    for (int k = 0; k < dims[2]-1; k++){
      for (int j = 0; j < dims[1]-1; j++){
	for (int i = 0; i < dims[0]-1; i++){
	  int ijk[3]={i,j,k};
	  int cell_id =  vtkStructuredData::ComputeCellId(dims,ijk);
	  ///Set values from the matrix data column
	  for (int d = 0; d < v_attr_cell.size(); d++){
	    double cell_value = static_cast<double>(vsm[i][j][k][d]);
	    if (std::isnan(cell_value)){
	      cout << "NaN value for cell " << cell_id << endl;
	      v_attr_cell[d]->SetValue(cell_id,0.0);
	    }
	    else{
	      v_attr_cell[d]->SetValue(cell_id,cell_value);
	    }
	  }
	}
      }
    }
    
    //Point attribute values
    vector<vtkNew<vtkDoubleArray>> v_attr_point(attr_names.size());
    for (int i = 0; i < v_attr_point.size();i++){
      v_attr_point[i]->SetName(attr_names[i].c_str());
      v_attr_point[i]->SetNumberOfTuples(sgrid->GetNumberOfPoints());
    }
    for (int k = 0; k < dims[2]; k++){
      for (int j = 0; j < dims[1]; j++){
	for (int i = 0; i < dims[0]; i++){
	  int ijk[3]={i,j,k};
	  int point_id =  vtkStructuredData::ComputePointId(dims,ijk);
	  ///Set values from the matrix data column
	  for (int d = 0; d < v_attr_point.size(); d++){
	    double point_value = static_cast<double>(vsm[i][j][k][d]);
	    if (std::isnan(point_value)){
	      cout << "NaN value for point " << point_id << endl;
	      v_attr_point[d]->SetValue(point_id,0.0);
	    }
	    else{
	      v_attr_point[d]->SetValue(point_id,point_value);
	    }
	  }
	}
      }
    }
    //Assign attribute values to cell and point data
    for (int i = 0; i < v_attr_cell.size();i++){
      sgrid->GetCellData()->AddArray(v_attr_cell[i]);
      sgrid->GetPointData()->AddArray(v_attr_point[i]);
    }
    //Active scalar
    sgrid->GetCellData()->SetActiveScalars(attr_names[0].c_str());
    return *this;
  }

  VoxelSpaceToVTK& VoxelSpaceToVTK::writeStructuredGrid(const string& file_name)
  {
    const char* ext = writer->GetDefaultFileExtension();
    std::string extension("."+std::string(ext));
    std::string::size_type n = file_name.rfind(extension);
    if (n == std::string::npos){
      cout << "File name extension should be " << std::string(ext) << endl;
    }
    writer->SetFileName(file_name.c_str());
    writer->SetInputDataObject(this->sgrid);
    writer->SetDataModeToAscii(); 
    cout << "Updating writer" <<endl;
    writer->Update();
    cout << "Writing data" <<endl;
    writer->Write();
    cout << "Write file done" <<endl;
    return *this;
  }
}
