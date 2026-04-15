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
  cout << "./vsvtk -input|-i file.h5 [-list] [-output|-o file.vts] [-dataset <path_string>] [-side <integer>]" << endl;
  cout << "Examples:" << endl;
  cout << "Read voxel space HDF5 and list dataset paths" << endl;
  cout << "  ./vsvtk -i File.h5 -list" << endl;
  cout << "Read voxel space HDF5 file and produce VTK/VTS file with voxel side 3 (default 1)" << endl;
  cout << "  ./vsvtk -i File.h5 -o VTSFile.vts -side 3" << endl;
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
  string out_file_name;
  bool out_file_found = false;
  if (ParseCommandLine(argc,argv,"-output",out_file_name) || ParseCommandLine(argc,argv,"-o",out_file_name)){
    out_file_found = true;
  }
  bool list_content=false;
  if (CheckCommandLine(argc,argv,"-list")){
    list_content = true;
  }

  lignumvtk::HDF5ToVoxelSpace hdf5vs;
  if (list_content){
    hdf5vs.openFile(fname);
    vector<string> v = hdf5vs.getDataSetNames(hdf5vs.getRoot());
    std::copy(v.begin(),v.end(),std::ostream_iterator<string>(std::cout, "\n"));
    return EXIT_SUCCESS;
  }
  string dataset_name;
  bool dataset_found = false;
  if (ParseCommandLine(argc,argv,"-dataset",dataset_name)){
    dataset_found = true;
  }
  if ((dataset_found == false) || (out_file_found == false)){
    cout << "Either dataset name or output file name missing, exiting" <<endl;
    return EXIT_FAILURE;
  }
  string vside;
  double voxel_side = 1.0;
  if (ParseCommandLine(argc,argv,"-side",vside)){
    voxel_side = std::stod(vside);
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
  hdf5vs.readDataSetAttribute(dataset_name,"VBDATAColumnNames",col_names);
  lignumvtk::VoxelSpaceToVTK vsvtk(vs_data.a(),vs_data.b(),vs_data.c(),voxel_side,voxel_side,voxel_side);
  vsvtk.insertVoxelSpaceData(vs_data,col_names);
  vsvtk.writeStructuredGrid(out_file_name);
  return EXIT_SUCCESS;
}
    
      
