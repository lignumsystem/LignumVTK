#include <CreateVTPCFile.h>

namespace lignumvtk{

  int CreateVTPCFileFromXML(const string& input_file,const string& output_file)
  {
    XMLDomTreeReader<VTKHwSegment,VTKBud,Kite> tree_hwreader;
    XMLDomTreeReader<VTKCfSegment,VTKCfBud> tree_cfreader;
    LignumVTKXML vtk_xml;
    LignumToVTK lignumvtk;
    Tree<VTKHwSegment,VTKBud> lignum_hwtree(Point(0,0,0), PositionVector(0,0,1.0));
    Tree<VTKCfSegment,VTKCfBud> lignum_cftree;

    //Peek the XML fiel and check for tree type and leaf type
    vtk_xml.setFileName(input_file);
    vtk_xml.parse();
    const string tree_type = vtk_xml.treeType();
    const string leaf_type = vtk_xml.leafType();
    if (tree_type == "Cf"){
      cout << "Coniferous tree" <<endl;
      cout << "Reading tree: " <<  input_file <<endl;
      tree_cfreader.readXMLToTree(lignum_cftree, input_file);
      cout << "Create conifer VTK data sets" <<endl;
      lignumvtk.createConiferTreeVTKDataSets(lignum_cftree,false);
      cout << "Write file" <<endl;
      lignumvtk.writePartitionedDataSetCollection(output_file);
      return EXIT_SUCCESS;
    }
    //Check for leaf  type
    if (leaf_type != "Kite"){
      cout << " CreateVTPCFileFromXML leaf type " << leaf_type << " not yet supported" <<endl;
      return EXIT_FAILURE;
    }
    cout << "Broadleaved tree" <<endl;
    cout << "Reading tree: " <<  input_file <<endl;
    tree_hwreader.readXMLToTree(lignum_hwtree, input_file);
    cout << "Create broadleaved VTK data sets" <<endl;
    lignumvtk.createBroadLeafTreeVTKDataSets(lignum_hwtree,false);
    cout << "Write file" <<endl;
    lignumvtk.writePartitionedDataSetCollection(output_file);
    return EXIT_SUCCESS;
  }

  int CreateVTPCFileFromHDF5(const string& input_file,const string& output_file, int year)
  {
    LignumVTKXML vtk_xml;
    LignumToVTK lignumvtkcf;
    HDF5ToLignum hdf5lignum;
    hdf5lignum.openFile(input_file);
    vector<string> v = hdf5lignum.getDataSetNames(year);
    if (v.size() == 0){
      cout << "No HDF5 datasets for trees for year " << year << endl;
      return EXIT_FAILURE;
    }
    
    std::string lignum_string =  hdf5lignum.getLignumTreeString(v[0]);
    QDomDocument qdoc("LMODEL");
    QString qt_xmlstring(lignum_string.c_str());
    if(!qdoc.setContent(qt_xmlstring)) {
      cout << "CreateVTPCFileFromHDF5 XMLDomTreeReader::readXMLStringToTree: Cannot set XML Dom content" <<endl;
      return EXIT_FAILURE;
    }
    QString type = qdoc.firstChild().toElement().attribute("SegmentType");
    if (type == "Hw"){
      cout << "CreateVTPCFileFromHDF5  broadleaved trees are not yet supported" <<endl;
      return EXIT_FAILURE;
    }
    for (unsigned int i=0; i < v.size(); i++){
      cout << "Tree: " << i <<endl;
      XMLDomTreeReader<VTKCfSegment,VTKCfBud> tree_string_reader;
      Tree<VTKCfSegment,VTKCfBud> lignum_cft;
      std::string lignum_string =  hdf5lignum.getLignumTreeString(v[i]);
      tree_string_reader.readXMLStringToTree(lignum_cft,lignum_string);
      lignumvtkcf.createConiferTreeVTKDataSets(lignum_cft,false);
    }
    cout << "Write file" <<endl;
    lignumvtkcf.writePartitionedDataSetCollection(output_file);    
    return EXIT_SUCCESS;
  }

  int CreateVTPCFileFromHDF5(const string& input_file,const string& output_file, const string& dataset_path, bool exact_match)
  {
    LignumVTKXML vtk_xml;
    LignumToVTK lignumvtkcf;
    HDF5ToLignum hdf5lignum;
    hdf5lignum.openFile(input_file);
    string tree_group=hdf5lignum.getTreeGroupName();
    vector<string> v = hdf5lignum.getDataSetNames(tree_group);
    vector<string> valid_paths;
    if (exact_match == true){
      std::copy_if(v.begin(),v.end(), std::back_inserter(valid_paths),FindExactMatch(dataset_path));
    }
    else{
      std::copy_if(v.begin(),v.end(), std::back_inserter(valid_paths),FindSubstring(dataset_path));
    }
    if (valid_paths.size() == 0){
      cout << "No HDF5 datasets for trees for path: " << dataset_path << endl;
      return EXIT_FAILURE;
    }
    //Test for broadleaved trees
    std::string lignum_string =  hdf5lignum.getLignumTreeString(valid_paths[0]);
    QDomDocument qdoc("LMODEL");
    QString qt_xmlstring(lignum_string.c_str());
    if(!qdoc.setContent(qt_xmlstring)) {
      cout << "CreateVTPCFileFromHDF5 XMLDomTreeReader::readXMLStringToTree: Cannot set XML Dom content" <<endl;
      return EXIT_FAILURE;
    }
    QString type = qdoc.firstChild().toElement().attribute("SegmentType");
    if (type == "Hw"){
      cout << "CreateVTPCFileFromHDF5  broadleaved trees are not yet supported" <<endl;
      return EXIT_FAILURE;
    }
    for (unsigned int i=0; i < valid_paths.size(); i++){
      cout << "Tree " << i << ": " << valid_paths[i] << endl;
      XMLDomTreeReader<VTKCfSegment,VTKCfBud> tree_string_reader;
      Tree<VTKCfSegment,VTKCfBud> lignum_cft;
      std::string lignum_string =  hdf5lignum.getLignumTreeString(valid_paths[i]);
      tree_string_reader.readXMLStringToTree(lignum_cft,lignum_string);
      lignumvtkcf.createConiferTreeVTKDataSets(lignum_cft,false);
    }
    cout << "Write file" <<endl;
    lignumvtkcf.writePartitionedDataSetCollection(output_file);
    return EXIT_SUCCESS;
  }
}
