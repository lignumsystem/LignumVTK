#include <CIESky.h>
namespace lignumvtk{
  CIESOC::CIESOC(int nazim, int nincl, double tot_rad)
  {

    //double nincl_span = 90.0/static_cast<double>(nincl);//Opens the zenith sector 
    //Construct the hempisphere
    hemisphere->SetRadius(1.0);
    //Azimuth divisions
    hemisphere->SetThetaResolution(nazim);
    //Inclination division
    hemisphere->SetPhiResolution(nincl);//nincl bands 
    hemisphere->SetStartPhi(0);         //Zenith
    hemisphere->SetEndPhi(90.0);        //Horizon
    hemisphere->LatLongTessellationOn();//Rectangular sectors, no triangulation
    hemisphere->Update();

    vtkPolyData* data = hemisphere->GetOutput();
    vtkIdType ncells = data->GetNumberOfCells();

    const std::string radiant_scalar= "RadiantIntensity";
    vtkNew<vtkDoubleArray> radiant_intensity;
    radiant_intensity->SetName(radiant_scalar.c_str());
    radiant_intensity->SetNumberOfComponents(1); //single radiant value
    radiant_intensity->SetNumberOfTuples(ncells);//array length

    //Calculate cell centroids and set radiant values per latitude
    for (vtkIdType cell_id=0; cell_id < ncells; cell_id++){
      vtkCell* cell = data->GetCell(cell_id);
      vtkPoints* points = cell->GetPoints();
      vtkIdType npoints = cell->GetNumberOfPoints();
      //cell points
      double cx = 0.0; double cy = 0.0; double cz = 0.0;
      for (vtkIdType i = 0; i < npoints; i++){
	double p[3];
	points->GetPoint(i,p);
	cx += p[0]; cy += p[1]; cz += p[2];
      }
      //cell average center point
      cx /= npoints;
      cy /= npoints;
      cz /= npoints;

      //The zenith angle (angle fron zenith downwards
      double cos_theta=1.0; //Default is zenith 
      double r = std::sqrt(cx*cx+cy*cy+cz*cz);
      if (r > 0.0){
	cos_theta = cz/r;
      }
      if (cos_theta < 0.0){
	cos_theta = 0.0;
      }
      if (cos_theta > 1.0){
	cos_theta = 1.0;
      }
      //SOC radiant intensity distribution 
      double I_cell = tot_rad*((1.0 + 2.0*cos_theta)/3.0);
      radiant_intensity->SetValue(cell_id,I_cell);
    }
    data->GetCellData()->AddArray(radiant_intensity);
    data->GetCellData()->SetActiveScalars(radiant_scalar.c_str());
  }
  
  int CIESOC::writeHemisphere(const std::string& file_name)
  {
    std::cout << "Writing hemisphere to: " << file_name << endl; 
    this->writer->SetFileName(file_name.c_str());
    this->writer->SetInputData(hemisphere->GetOutput());
    this->writer->Update();
    const char* ext = this->writer->GetDefaultFileExtension();
    //Simple extension check, c++17 std::filesystem::path is not yet available
    std::string extension("."+std::string(ext));
    std::string::size_type n = file_name.rfind(extension);
    if (std::string::npos == n){
      std::cout << "File name extension should be: " << std::string(ext) <<endl;
    }
    if (writer->Write()){
      std::cout << "Done" << endl;
      return EXIT_SUCCESS;
    }
    else{
      std::cout << "Error: Faile to write " << file_name << endl;
      return EXIT_FAILURE;
    }
  }

  CIESOCEqualArea::CIESOCEqualArea(int nazim, int nincl, double tot_rad)
  {
    //Create discrete sky pathces and compute the center point of each patch.
    //Calculate the elevation angle and apply the SOC overcast equation.
    //
    //Vectors for points, cells and radiance data.
    vtkNew<vtkPoints> points;
    vtkNew<vtkCellArray> cells;
    vtkNew<vtkDoubleArray> radiance_values;
    radiance_values->SetName("RadiantIntensity");
    
    //Inclination angles for equal area sectors, the angle is from zenith
    std::vector<double> incl_angles;
    for (int i = 0; i <= nincl; i++){
      double cos_theta = 1.0-(static_cast<double>(i)/static_cast<double>(nincl));
      incl_angles.push_back(std::acos(cos_theta));
    }

    //Azimuth divisions
    std::vector<double> azim_angles;
    for (int i = 0; i <= nazim; i++){
      double angle = i*(2.0*PI_VALUE / nazim); 
      azim_angles.push_back(angle);
    }

    //Generate sky dome vertices
    for (double theta : incl_angles){
      for (double phi : azim_angles){
	//theta: polar angle
	//phi: azimuthal angle
	double x = std::sin(theta)*std::cos(phi);
	double y = std::sin(theta)*std::sin(phi);
	double z = std::cos(theta);
	points->InsertNextPoint(x,y,z);
      }
    }
    //Connect all points, from first to last
    int num_cols = nazim+1;
    for (int i = 0; i < nincl; i++){
      //Calculate midpoint in inclination angles
      double mid_theta = (incl_angles[i]+incl_angles[i+1])/2.0;
      //Calculate the angle up from the horizon
      double gamma = (PI_VALUE/2.0) - mid_theta;
      //CIE standard overcast
      //Relative radiant intensity
      double I_rel = (1.0+2.0*std::cos(mid_theta))/3.0;
      //double I_rel = (1.0+2.0*std::sin(gamma))/3.0;
      //Sector radiant intensity
      double I_s = tot_rad*I_rel;
      //vtkQuad connections
      for (int j = 0; j < nazim; j++){
	//The formula maps a 2D coordinate grid to a 1D sequence
	//Index = (Row + Total columns)+Column
	vtkIdType p1 = i*num_cols+j; //Top left corner
	vtkIdType p2 = p1+1;         //Next index to the right of p1, in the same row
	vtkIdType p3 = (i+1)*num_cols+j+1; //Shifted down from p2, to the next row below
	vtkIdType p4 = (i+1)*num_cols+j;   //Shifted left from p3, in the same row 
	//Setup the quad, counter clockwise assembly
	vtkNew<vtkQuad> quad;
	quad->GetPointIds()->SetId(0,p1);
	quad->GetPointIds()->SetId(1,p2);
	quad->GetPointIds()->SetId(2,p3);
	quad->GetPointIds()->SetId(3,p4);
	cells->InsertNextCell(quad);
	radiance_values->InsertNextValue(I_s);
      }
    }
    polydata->SetPoints(points);
    polydata->SetPolys(cells);
    polydata->GetCellData()->SetScalars(radiance_values);
  }   

  int CIESOCEqualArea::writeHemisphere(const string& file_name)
  {
    std::cout << "Writing hemisphere to: " << file_name << endl; 
    this->writer->SetFileName(file_name.c_str());
    this->writer->SetInputData(polydata);
    this->writer->Update();
    const char* ext = this->writer->GetDefaultFileExtension();
    //Simple extension check, c++17 std::filesystem::path is not yet available
    std::string extension("."+std::string(ext));
    std::string::size_type n = file_name.rfind(extension);
    if (std::string::npos == n){
      std::cout << "File name extension should be: " << std::string(ext) <<endl;
    }
    if (writer->Write()){
      std::cout << "Done" << endl;
      return EXIT_SUCCESS;
    }
    else{
      std::cout << "Error: Faile to write " << file_name << endl;
      return EXIT_FAILURE;
    }
    
  }
}  
