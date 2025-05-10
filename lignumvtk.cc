#include <cstdlib>
#include <ParseCommandLine.h>
#include <LGMHDF5File.h>
#include <Lignum.h>
#include <XMLTree.h>
#include <HDF5ToLignum.h>
#include <LignumVTK.h>
#include <LignumVTKXML.h>
#include <LignumVTKTree.h>
#include <CreateVTPCFile.h>

using namespace lignumvtk;
int Usage()
{
  cout << "Usage:" <<endl;
  cout << "./lignumvtk -help" <<endl;
  cout << "./lignumvtk -input file.h5 [-list] [-output file.vtpc] [-year -number>] [-dataset <path_string>]" << endl;
  cout << "./lignumvtk -input file.xml -output file.vtpc" << endl;
  cout << "Examples:" <<endl;
  cout << "Read Lignum XML file and produce VTK/VTPC file" << endl; 
  cout << "  ./lignumvtk -input File.xml -output VTKFile.vtpc" << endl;
  cout << "Read Lignum HDF5 file and list tree dataset paths" <<endl; 
  cout << "  ./lignumvtk -input File.h5 -list" << endl;
  cout << "Read Lignum HDF5 file and produce VTK/VTPC files for growth year 20" <<endl;
  cout << "./lignumvtk -input File.h5 -output VTKFile.vtpc -year 20" << endl;
  cout << "Read Lignum HDF5 file and produce VTK/VTPC file for Tree_8 in year 60" <<endl;
  cout << "./lignumvtk -input File.h5 -output VTKFile.vtpc -dataset /TreeXML/60/Tree_8" << endl;
  return EXIT_SUCCESS;
}
  
    
int main(int argc,char* argv[])
{
  if (argc < 2||CheckCommandLine(argc,argv,"-help") ){
    return Usage();
  }
  bool list_content = false;
  if (CheckCommandLine(argc,argv,"-list")){
    list_content = true;
  }
  std::string input_file;
  bool is_hdf5 = true;
  bool is_xml = true;
  if (ParseCommandLine(argc,argv,"-input",input_file)){
     std::string::size_type n;
     n = input_file.find(".h5");
     if (n == std::string::npos){
       is_hdf5 = false;
     }
     n = input_file.find(".xml");
     if (n == std::string::npos){
       is_xml = false;
     }
     if (!(is_xml || is_hdf5)){
       cout << "Input file " << input_file << " should have suffix .xml or .h5, exiting" <<endl;
       return EXIT_FAILURE;
     }
  }
  else{
    cout << "No inputfile, exiting"<< endl;
    return EXIT_FAILURE;
  }
  std::string output_file;
  bool output_found=false;
  if (ParseCommandLine(argc,argv,"-output",output_file)){
    output_found = true;
  }
  std::string year;
  int growth_year = -1;
  bool use_year = false;
  if (ParseCommandLine(argc,argv,"-year",year)){
    growth_year = std::stoi(year);
    use_year=true;
  }
  std::string dataset;
  bool use_dataset = false;
  if (ParseCommandLine(argc,argv,"-dataset",dataset)){
    use_dataset = true;
  }
  //Command line parsed, do the following:
  //Case 1: List the content of the HDF5 tree files  
  if (list_content == true){
    if (is_hdf5 == false){
      cout << "The HDF5 input file " << input_file << " to list datasets should have suffix .h5" <<endl;
      return EXIT_FAILURE;
    }
    HDF5ToLignum hdf5lignum;
    hdf5lignum.openFile(input_file);
    string tree_group=hdf5lignum.getTreeGroupName();
    vector<string> v = hdf5lignum.getDataSetNames(tree_group);
    std::copy(v.begin(),v.end(),std::ostream_iterator<string>(std::cout, "\n"));
    cout << "Total of " << v.size() << " trees" <<endl;
    return EXIT_SUCCESS;
  }
  //At this point check the output file exists
  int retval=0;
  if (!output_found){
    cout << "No output file, exiting" <<endl;
    return EXIT_FAILURE;
  }
  //Case 2: The input file is a single xml file
  if (is_xml){
    retval = CreateVTPCFileFromXML(input_file,output_file);
  }
  //Case 3: The HDF5 datasets from a given year are used
  else if (use_year == true){
    retval = CreateVTPCFileFromHDF5(input_file,output_file,growth_year);
  }
  //Case 4: The HDF5 Dataset path is used
  else if (use_dataset == true){
    retval = CreateVTPCFileFromHDF5(input_file,output_file,dataset);
  }
  else{
    cout << "Define input XML file or define year or dataset name for HDF5 file" << endl;
    return EXIT_FAILURE;
  }
  
  return retval;
}
