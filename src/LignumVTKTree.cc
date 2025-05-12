#include <LignumVTKTree.h>
namespace lignumvtk{
  TubeActorVector& CreateVTKTubeActor::operator()(TubeActorVector& v, PDMptr pdm)const
  {
    vtkNew<vtkNamedColors> colors;
    vtkNew<vtkActor> ta;
    ta->GetProperty()->SetColor(colors->GetColor3d("sandy_brown").GetData());
    ta->SetMapper(pdm);
    v.push_back(ta);
    return v;
  }

  TubeMapperVector& CreateVTKTubeMapper::operator()(TubeMapperVector& v, TFptr tf)const
  {
    vtkNew<vtkPolyDataMapper> tm;
    tm->SetInputConnection(tf->GetOutputPort());
    vtkPolyData* polydata = tf->GetOutput();
    //If scalar visibility is on, the mapper will override actor color
    tm->SetScalarRange(polydata->GetScalarRange());
    //tm->ScalarVisibilityOn();
    tm->ScalarVisibilityOff();
    tm->Update();
    v.push_back(tm);
    return v;
  }
    
  TubeFilterVector& CreateVTKTubeFilter::operator()(TubeFilterVector& v, PFSptr fs)const
  {
    vtkNew<vtkTubeFilter> tf;
    vtkPolyData* polydata = fs->GetOutput();
    tf->AddInputData(polydata);
    tf->SetVaryRadiusToVaryRadiusByAbsoluteScalar();
    tf->SetNumberOfSides(TUBE_NUMBER_OF_SIDES);
    tf->CappingOn();
    tf->Update();
    v.push_back(tf);
    return v;
  }

  PFSVector& CreateVTKSpline::operator()(PFSVector& v, TSData& data)const
  {
    vtkNew<vtkParametricFunctionSource> pfs;
    vtkNew<vtkParametricSpline> spline;
    vtkNew<vtkPoints> points;

    vtkIdType n = static_cast<vtkIdType>(data.vpoints.size());
    for (vtkIdType i=0; i < n; i++){
      Point p = data.vpoints[i];
      points->InsertPoint(i,p.getX(),p.getY(),p.getZ());
    }
    
    spline->SetPoints(points);
    pfs->SetParametricFunction(spline);
    pfs->SetUResolution((this->resolution) * points->GetNumberOfPoints());
    pfs->Update();
    v.push_back(pfs);
    return v;
  }

  LineActorVector& CreateVTKLineActor::operator()(LineActorVector& v, TSData& data)const
  {
    vtkNew<vtkLineSource> line_source;
    vtkNew<vtkPoints> points;
    vtkNew<vtkPolyData> polydata;
    vtkNew<vtkPolyDataMapper> mapper;
    vtkNew<vtkActor> actor;
    vtkNew<vtkNamedColors> colors;
    
    vtkIdType n = static_cast<vtkIdType>(data.vpoints.size());
    assert(n==2 && "CreateVTKLineActor: Line can have two points");
    Point p1 = data.vpoints[0];
    Point p2 = data.vpoints[1];
    //cout << "P1:   " << p1 << " P2:   " << p2 << " D: " << (p1||p2) << endl;
    for (vtkIdType i=0; i<n; i++){
      Point p = data.vpoints[i];
      points->InsertPoint(i,p.getX(),p.getY(),p.getZ());
    }
    double vtkp1[3];
    double vtkp2[3];
    points->GetPoint(0,vtkp1);
    points->GetPoint(1,vtkp2);
    //cout << "P1vtk: " << vtkp1[0] << " " << vtkp1[1] << " " << vtkp1[2]
    //	 <<" P2vtk: " << vtkp2[0] << " " << vtkp2[1] << " " << vtkp2[2] <<endl;
    line_source->SetPoint1(vtkp1);
    line_source->SetPoint2(vtkp2);
    line_source->Update();
    vtkSmartPointer<vtkPolyData> polyData = line_source->GetOutput();
    vtkSmartPointer<vtkPoints> outputPoints = polyData->GetPoints();
    vtkSmartPointer<vtkPointData> pointdata = polyData->GetPointData();
    vtkNew<vtkDoubleArray> line_width;
    int npoints = outputPoints->GetNumberOfPoints();
    line_width->SetNumberOfTuples(npoints);
    line_width->SetName(data.vname[0].c_str());
    line_width->InsertTuple1(0,data.vR[0]);
    line_width->InsertTuple1(1,data.vR[1]);
    pointdata->SetScalars(line_width);
    // colors->SetNumberOfComponents(3);
    // colors->SetNumberOfTuples(1);
    // colors->SetTuple3(0,34, 139, 34);
    // polydata->GetCellData()->SetScalars(colors);
    polydata->GetCellData()->SetActiveScalars(data.vname[0].c_str());
    mapper->SetInputConnection(line_source->GetOutputPort());
    mapper->Update();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetLineWidth(LINE_WIDTH);
    actor->GetProperty()->SetColor(colors->GetColor3d("green").GetData());
    v.push_back(actor);
    return v;
  }

  LineActorVector& CreateVTKKiteLeafActor::operator()(LineActorVector& v, TSData& data)const
  {
    vtkNew<vtkTriangleStrip> triangle_strip;
    vtkNew<vtkPoints> points;
    vtkNew<vtkCellArray> cell_array;
    vtkNew<vtkPolyData> polydata;
    vtkNew<vtkPolyDataMapper> mapper;
    vtkNew<vtkActor> actor;
    vtkNew<vtkNamedColors> colors;
    
    vtkIdType n = static_cast<vtkIdType>(data.vpoints.size());
    assert(n==4 && "CreateVTKKiteLeafActor: Kite leaf must have exactly four points");
    //Leaf points are in counterclockwise order. Single TriangleStrip needs four points
    //for two triangles. In VTK three consecutive points create a triangle for TriangleStrip 
    Point p0 = data.vpoints[0];//Base cornet
    Point p1 = data.vpoints[1];//Right corner
    Point p2 = data.vpoints[2];//Apex corner
    Point p3 = data.vpoints[3];//Left corner
    //Reorder points so that two triangles form a  Kite shape
    points->InsertPoint(0,p0.getX(),p0.getY(),p0.getZ());//Base point
    points->InsertPoint(1,p3.getX(),p3.getY(),p3.getZ());//Left corner
    points->InsertPoint(2,p1.getX(),p1.getY(),p1.getZ());//Right corner
    points->InsertPoint(3,p2.getX(),p2.getY(),p2.getZ());//Apex corner
    triangle_strip->GetPointIds()->SetNumberOfIds(4);
    triangle_strip->GetPointIds()->SetId(0, 0);
    triangle_strip->GetPointIds()->SetId(1, 1);
    triangle_strip->GetPointIds()->SetId(2, 2);
    triangle_strip->GetPointIds()->SetId(3, 3);
    cell_array->InsertNextCell(triangle_strip);
    polydata->SetPoints(points);
    polydata->SetStrips(cell_array);
    int ncells = polydata->GetNumberOfCells();
    //cout << "Kite leaf cells: " << ncells << " leaf area " << data.vA[0] << endl <<endl;
    //Add scalars required
    vtkNew<vtkDoubleArray> leaf_area;
    vtkNew<vtkDoubleArray> qin;
    vtkNew<vtkDoubleArray> qabs;
    vtkNew<vtkDoubleArray> photosynthesis;
    leaf_area->SetName(data.vname[0].c_str());
    qin->SetName(QIN_SCALAR.c_str());
    qabs->SetName(QABS_SCALAR.c_str());
    photosynthesis->SetName(PHOTOSYNTHESIS_SCALAR.c_str());
    leaf_area->SetNumberOfTuples(ncells);
    qin->SetNumberOfTuples(ncells);
    qabs->SetNumberOfTuples(ncells);
    photosynthesis->SetNumberOfTuples(ncells);
    for (unsigned int i=0; i < ncells;i++){
      leaf_area->InsertTuple1(i,data.vA[0]);
      qin->InsertTuple1(i,data.vQin[0]);
      qabs->InsertTuple1(i,data.vQabs[0]);
      photosynthesis->InsertTuple1(i,data.vP[0]);
    }
    polydata->GetCellData()->AddArray(leaf_area);
    polydata->GetCellData()->AddArray(qin);
    polydata->GetCellData()->AddArray(qabs);
    polydata->GetCellData()->AddArray(photosynthesis); 
    polydata->GetCellData()->SetActiveScalars(data.vname[0].c_str());
    //Scalars added
    mapper->SetInputData(polydata);
    mapper->SetScalarRange(polydata->GetScalarRange());
    mapper->ScalarVisibilityOff();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(colors->GetColor3d(LEAF_COLOR).GetData());
    v.push_back(actor);
    return v;
  }
  
  TSDataVector& LignumToVTK::treeToPetioleData(LignumVTKTree& t, TSDataVector& v)
  {
    v = AccumulateDown(t,v,Append,CollectPetioleData<VTKHwSegment,VTKBud,Kite>());
    return v;
  }

  TSDataVector& LignumToVTK::treeToKiteLeafData(LignumVTKTree& t, TSDataVector& v)
  {
    v = AccumulateDown(t,v,Append,CollectLeafData<VTKHwSegment,VTKBud,Kite>());
    return v;
  }
  
  TSDataVector& LignumToVTK::treeToHwTSData(LignumVTKTree& t, TSDataVector& v)
  {
    v = AccumulateDown(t,v,Append,CollectTSData<VTKHwSegment,VTKBud>());
    return v;
  }

   TSDataVector& LignumToVTK::treeToCfTSData(LignumVTKCfTree& t, TSDataVector& v)
  {
    v = AccumulateDown(t,v,Append,CollectTSData<VTKCfSegment,VTKCfBud>());
    return v;
  }
  
  LineActorVector& LignumToVTK::createLineActors(TSDataVector& v,LineActorVector& lav)
  {
    lav = accumulate(v.begin(),v.end(),lav,CreateVTKLineActor());
    return lav;
  }

   LineActorVector& LignumToVTK::createKiteLeafActors(TSDataVector& v,LineActorVector& lav)
  {
    lav = accumulate(v.begin(),v.end(),lav,CreateVTKKiteLeafActor());
    return lav;
  }
  
  PFSVector& LignumToVTK::vtkPointsToVtkSpline(TSDataVector& v, PFSVector& pfsv)const
  {
    pfsv=accumulate(v.begin(),v.end(),pfsv,CreateVTKSpline(resolution));
    return pfsv;
  }

  PFSVector& LignumToVTK::createTubeRadiusScalars(TSDataVector& v, PFSVector& pfsv, const string& scalar_name)const
  {
    //There are equal number of axes and their representation as spline segments
    assert(v.size() == pfsv.size() && "The vectors must be of equal length");
    for (unsigned int i = 0; i < v.size(); i++){
      TSData& data = v[i];
      vtkParametricFunctionSource* fs = pfsv[i];
      unsigned int npoints = fs->GetOutput()->GetNumberOfPoints();
      vtkNew<vtkDoubleArray> tube_radius;
      tube_radius->SetNumberOfTuples(npoints);
      tube_radius->SetName(scalar_name.c_str());
      //Foliage mass in a segment
      vtkNew<vtkDoubleArray> wfarray;
      wfarray->SetNumberOfTuples(npoints);
      wfarray->SetName(FOLIAGE_MASS_SCALAR.c_str());
      for (unsigned int j = 0; j < data.vR.size(); j++){
	double r = 0.0;
	double wf= 0.0;
	if (scalar_name == TUBE_RADIUS_SCALAR){
	  r =  data.vR[j];
	}
	else if (scalar_name == TUBE_HW_RADIUS_SCALAR){
	  r = data.vRh[j];
	}
	else if (scalar_name == TUBE_FOLIAGE_RADIUS_SCALAR){
	  r = data.vRf[j];
	  wf = data.vWf[j];
	}
	for (int k = 0; k < (this->resolution); k++){
	  //Copy the radius for each spline point
	  tube_radius->InsertTuple1(j*(this->resolution)+k,r);
	  wfarray->InsertTuple1(j*(this->resolution)+k,wf);
	}
      }
      //There are Npoints*Resolution+1 spline points (i.e. NPoints*Resolution line segments)
      //Add radius for the last point
      tube_radius->InsertTuple1(data.vR.size()*(this->resolution),data.vR[data.vR.size()-1]);
      wfarray->InsertTuple1(data.vWf.size()*(this->resolution),data.vWf[data.vWf.size()-1]);
      vtkPolyData* polydata = fs->GetOutput();
      polydata->GetPointData()->AddArray(tube_radius);
      polydata->GetPointData()->SetActiveScalars(scalar_name.c_str());
      //Add Foliage mass only to one polydata representing radius to foliage scalar   
      if (scalar_name == TUBE_FOLIAGE_RADIUS_SCALAR){
	polydata->GetPointData()->AddArray(wfarray);
      }
    }
    return pfsv;
  }
   
  TubeFilterVector& LignumToVTK::createTubeFilters(PFSVector& pfsv, TubeFilterVector& v)const
  {
    v = accumulate(pfsv.begin(),pfsv.end(),v,CreateVTKTubeFilter());
    return v;
  }

  TubeMapperVector& LignumToVTK::createTubeMappers(TSDataVector& tsv,TubeMapperVector& v,const string& scalar_name)const
  {
    PFSVector sv;
    sv = vtkPointsToVtkSpline(tsv,sv);
    sv = createTubeRadiusScalars(tsv,sv,scalar_name);
    TubeFilterVector tfv;
    tfv = createTubeFilters(sv,tfv);
    v = accumulate(tfv.begin(),tfv.end(),v,CreateVTKTubeMapper());
    return v;
  }

  TubeActorVector& LignumToVTK::createTubeActors(TubeMapperVector& tmv, TubeActorVector& v)const
  {
    v = accumulate(tmv.begin(),tmv.end(),v,CreateVTKTubeActor());
    return v;
  }
	
  LignumToVTK& LignumToVTK::addActorsToRenderer(TubeActorVector& v)
  {
    cout << "Adding " << v.size() << " actors to renderer" <<endl;
    for (unsigned int i = 0; i < v.size(); i++){
      this->renderer->AddActor(v[i]);
    }
    return *this;
  }

  LignumToVTK& LignumToVTK::setActorColor(const string& color)
  {
    vtkNew<vtkNamedColors> colors;
    vtkActorCollection* actors = this->renderer->GetActors();
    int nactors = actors->GetNumberOfItems();
    vtkColor3d rgb=colors->GetColor3d(color);
    actors->InitTraversal();
    for (int i = 0; i < nactors; i++){
      vtkActor* actor = actors->GetNextActor();
      actor->GetProperty()->SetColor(rgb.GetRed(),rgb.GetGreen(),rgb.GetBlue());
    }
    return *this;
  }
  
  LignumToVTK& LignumToVTK::addPartitionedDataSet(VTKActorVector& v,const string& block_name)
  {
    cout << "Adding ParitionedDataSet with " << v.size() << " actors" << endl;
    vtkNew<vtkPartitionedDataSet> dataset_new;
    int new_partitions = v.size();
    dataset_new->SetNumberOfPartitions(new_partitions);
    for (unsigned int i=0; i < new_partitions; i++){
      vtkActor* actor = v[i];
      dataset_new->SetPartition(i,actor->GetMapper()->GetInput());
    }
    int partitions = this->dataset_collection->GetNumberOfPartitionedDataSets();    
    this->dataset_collection->SetPartitionedDataSet(partitions,dataset_new);
    //Technique to give name to the dataset block
    this->dataset_collection->GetMetaData(static_cast<unsigned int>(partitions))->Set(vtkPartitionedDataSet::NAME(),block_name);
    return *this;
  }

  LignumToVTK& LignumToVTK::writePartitionedDataSetCollection(const string& file_name)
  {
    std::string::size_type n;
    cout << "PartitionedDataSetCollection file: " << file_name.c_str() << endl;
    const char* ext = this->collection_writer->GetDefaultFileExtension();
    //Simple extension check, c++17 std::filesystem::path is not yet available
    std::string extension("."+std::string(ext));
    n = extension.find(VTPC_EXTENSION);
    if (std::string::npos == n){
      cout << "File name extension should be: " << std::string(ext) <<endl;
    }
    this->collection_writer->SetFileName(file_name.c_str());
    this->collection_writer->SetInputData(this->dataset_collection);
    cout << "Updating writer" <<endl;
    this->collection_writer->Update();
    cout << "Writing data" <<endl;
    this->collection_writer->Write();
    cout << "Write file done" <<endl;
    return *this;
  }
  
  LignumToVTK& LignumToVTK::addMultiBlockDataSet(VTKActorVector& v)
  {
    cout << "Adding  MultiBlockDataSet" <<endl;
    vtkNew<vtkMultiBlockDataSet> mb_set_new;
    int blocks_new = v.size();
    mb_set_new->SetNumberOfBlocks(blocks_new);
    for (unsigned int i=0; i < blocks_new; i++){
      vtkActor* actor = v[i];
      mb_set_new->SetBlock(i,actor->GetMapper()->GetInput());
    }
    int blocks = mb_set->GetNumberOfBlocks();
    this->mb_set->SetBlock(blocks,mb_set_new);
    return *this;
  }

  
  LignumToVTK& LignumToVTK::writeMultiBlockDataSet(const string& file_name)
  {
    std::string::size_type n;
    cout << "MultiBlockDataSet file " << file_name.c_str() << endl;
    const char* ext = this->writer->GetDefaultFileExtension();
    //Simple extension check, c++17 std::filesystem::path is not yet available
    std::string extension("."+std::string(ext));
    n = extension.find(VTM_EXTENSION);
    if (std::string::npos == n){
      cout << "File name extension should be: " << std::string(ext) <<endl;
    }
    this->writer->SetFileName(file_name.c_str());
    this->writer->SetInputData(mb_set);
    cout << " Updating writer" <<endl;
    this->writer->Update();
    cout << " Writing data" <<endl;
    this->writer->Write();
    cout << "Write file done" <<endl;
    return *this;
  }
  
}				     
