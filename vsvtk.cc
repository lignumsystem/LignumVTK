#include <cstdlib>
#include <algorithm>
#include <ParseCommandLine.h>
#include <LGMHDF5File.h>
#include <Lignum.h>
#include <XMLTree.h>
#include <HDF5ToLignum.h>
#include <LignumVTK.h>
#include <LignumVTKXML.h>
#include <LignumVTKTree.h>
#include <VoxelSpaceToVTK.h>
#include <CreateVTPCFile.h>

int Usage()
{
  cout << "Usage:" << endl;
  cout << "./vsvtk [-h | -help]" <<endl;
  cout << "./vsvtk -input|-i file.h5 [-list] [-output|-o file.vts] [-dataset <path_string>] [-edge <integer>]" << endl;
  cout << "Examples:" << endl;
  cout << "Read voxel space HDF5 and list dataset paths" << endl;
  cout << "  ./vsvtk -i File.h5 -list" << endl;
  cout << "Read voxel space HDF5 file and produce VTK/VTS file with voxel edge size 3" << endl;
  cout << "  ./vsvtk -i File.h5 -o VTSFile.vts -edge 3" << endl;
  return EXIT_SUCCESS;
}
     
int main(int argc, char* argv[])
{
  if (argc < 2||CheckCommandLine(argc,argv,"-help")||CheckCommandLine(argc,argv,"-h")){
    return Usage();
  }
  
  string fname;
  if (!(ParseCommandLine(argc,argv,"-input",fname) || ParseCommandLine(argc,argv,"-i",fname))){
      cout << "No input file, exiting" << endl;
      return EXIT_FAILURE;
  }
  string outfile_name;
  bool outfile_found = false;
  if (ParseCommandLine(argc,argv,"-output",outfile_name) || ParseCommandLine(argc,argv,"-o",outfile_name)){
    outfile_found = true;
  }
  bool list_content=false;
  if (CheckCommandLine(argc,argv,"-list")){
    list_content = true;
  }

  lignumvtk::HDF5ToVoxelSpace hdf5vs;
  if (list_content){
    cout << "Dataset names:" << endl;
    hdf5vs.openFile(fname);
    vector<string> v = hdf5vs.getDataSetNames(hdf5vs.getRoot());
    std::copy(v.begin(),v.end(),std::ostream_iterator<string>(std::cout, "\n"));
    vector<double> vsdset;
    lignumvtk::GridData gdata = hdf5vs.getVoxelSpace(v[0],vsdset);
    std::tuple<double,double,double> v_size = gdata.second;
    cout << "Single voxel size:" << endl <<
      std::get<0>(v_size) << " " << std::get<1>(v_size) << " " << std::get<2>(v_size) << endl;
    cout << "Attribute names:" << endl;
    vector<string> attr_v = hdf5vs.readAttributeNames();
    copy(attr_v.begin(),attr_v.end(),std::ostream_iterator<string>(std::cout, "\n"));
    return EXIT_SUCCESS;
  }
  string dataset_name;
  bool dataset_found = false;
  if (ParseCommandLine(argc,argv,"-dataset",dataset_name)){
    dataset_found = true;
  }
  if ((dataset_found == false) || (outfile_found == false)){
    if (!dataset_found){
      cout << "No dataset name" << endl;
    }
    if (!outfile_found){
      cout << "Output file name missing" << endl;
    }
    cout << "Exiting" << endl;
    return EXIT_FAILURE;
  }
  string vedge;
  double voxel_edge = 0.0;
  if (ParseCommandLine(argc,argv,"-edge",vedge)){
    voxel_edge = std::stod(vedge);
  }
  cout << "Reading voxel space file" << endl;
  hdf5vs.openFile(fname);
  vector<double> vsdset;
  lignumvtk::GridData gdata = hdf5vs.getVoxelSpace(dataset_name,vsdset);
  std::tuple<int,int,int,int> t_grid = gdata.first;
  std::tuple<double,double,double> v_size = gdata.second;
  cxxadt::TMatrix4D<double> vs_data(std::get<0>(t_grid),std::get<1>(t_grid),
				    std::get<2>(t_grid),std::get<3>(t_grid),vsdset);
  vector<string> col_names;
  hdf5vs.readDataSetAttribute(dataset_name,lignumvtk::VOXELBOX_DATA_ATTRIBUTE_NAME,col_names);
  if (voxel_edge == 0.0){
    voxel_edge = std::get<0>(v_size);
  }
  lignumvtk::VoxelSpaceToVTK vsvtk(vs_data.a(),vs_data.b(),vs_data.c(),voxel_edge,voxel_edge,voxel_edge);
  vsvtk.insertVoxelSpaceData(vs_data,col_names);
  vsvtk.writeStructuredGrid(outfile_name);
  return EXIT_SUCCESS;
}
