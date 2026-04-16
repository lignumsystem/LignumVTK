#include <HDF5ToLignum.h>

namespace lignumvtk{
  herr_t InsertDataSet(hid_t loc_id, const char* name, const H5O_info_t* info, void* user_data)
  {
    auto dataset_names = static_cast<DataSetNames*>(user_data);
    if(info->type == H5O_TYPE_DATASET){
      std::string prefix = dataset_names->prefix;
      dataset_names->names.push_back(prefix+std::string(name));
    }
    return 0;
  }

  void AttributeNameCollector(H5::H5Object &loc, const std::string attr_name, void *user_data)
  {
    auto attribute_names = static_cast<vector<string>*>(user_data);
    attribute_names->push_back(attr_name);
  }
  
  HDF5Base& HDF5Base::openFile(const string& name)
  {
    hdf5_file.openFile(name,H5F_ACC_RDONLY);
    return *this;
  }

  HDF5Base::~HDF5Base()
  {
    hdf5_file.close();
  }

  vector<string>& HDF5Base::getDataSetNames(const string& name)
  {
    dataset_names.names.clear();
    dataset_names.prefix=name;
    try{
      Exception::dontPrint();
      H5::Group g = hdf5_file.openGroup(name);
      hid_t group_id = g.getId();
      herr_t status;
      status = H5Ovisit(group_id, H5_INDEX_NAME, H5_ITER_NATIVE,InsertDataSet,
			static_cast<void*>(&dataset_names),H5O_INFO_ALL);
      g.close();
    }
    catch (H5::FileIException e){
      try{
	H5::DataSet dataset = hdf5_file.openDataSet(name);
	dataset_names.names.push_back(name);
	dataset.close();
      }
      catch (H5::FileIException e){
	cout << name << " is neither a group nor a dataset" <<endl;
      }
    }
    return dataset_names.names;
  }
  
  vector<string>& HDF5Base::getDataSetNames(int year)
  {
    dataset_names.names.clear();
    string group_name = getMainGroupName()+std::to_string(year)+std::string("/");
    dataset_names.prefix = group_name;
    H5::Group g;
    try{
      g = hdf5_file.openGroup(group_name);
    }
    catch (H5::FileIException e){
      cout << "HDF5ToLignum::getDataSetNames(): H5::FileIException No data for year " << year << endl;
      cout << "Exiting" <<endl;
      exit(0);
    }
    hid_t group_id = g.getId();
    herr_t status;
    status = H5Ovisit(group_id, H5_INDEX_NAME, H5_ITER_NATIVE,InsertDataSet,
		      static_cast<void*>(&dataset_names),H5O_INFO_ALL);
    g.close();
    return dataset_names.names;
  }
  
  vector<string> HDF5Base::readAttributeNames()
  {
    vector<string> v;
    if (dataset_names.names.size() > 0){
      const std::string dset_name = dataset_names.names[0];
      H5::DataSet dset = hdf5_file.openDataSet(dset_name);
      unsigned int index = 0;
      dset.iterateAttrs(AttributeNameCollector,&index,&v);
    }
    return v;
  }
  
  int HDF5Base::readDataSetAttribute(const string& dset_name, const string& attr_name,double& attr_value)
  {
    try{
      DataSet dset = hdf5_file.openDataSet(dset_name);
      Attribute attr = dset.openAttribute(attr_name);
      attr.read(PredType::NATIVE_DOUBLE,&attr_value);
    }
    catch (DataSetIException error){
      error.printErrorStack();
      return -1;
    }
    catch (AttributeIException error) {
      error.printErrorStack();
      return -1;
    }
    return 0;
  }

  int HDF5Base::readDataSetAttribute(const string& dset_name, const string& attr_name,vector<double>& attr_values)
  {
    try{
      DataSet dset = hdf5_file.openDataSet(dset_name);
      Attribute attr = dset.openAttribute(attr_name);
      H5::DataSpace dspace = attr.getSpace();
      //Vector has rank 1 
      int rank = dspace.getSimpleExtentNdims();
      vector<hsize_t> dims(rank);
      dspace.getSimpleExtentDims(dims.data(), NULL);
      //Number of vector elements
      size_t n_elmnt = dims[0];
      //Reader knows how much to read but argument vector must meet the size required
      attr_values.resize(n_elmnt);
      attr.read(H5::PredType::NATIVE_DOUBLE, attr_values.data());
    }
    catch (DataSetIException error){
      error.printErrorStack();
      return -1;
    }
    catch (AttributeIException error) {
      error.printErrorStack();
      return -1;
    }
    return 0;
  }

  int HDF5Base::readDataSetAttribute(const string& dset_name, const string& attr_name,vector<string>& attr_values)
  {
    try{
      DataSet dset = hdf5_file.openDataSet(dset_name);
      Attribute attr = dset.openAttribute(attr_name);
      H5::DataSpace dspace = attr.getSpace();
      //Vector has rank 1 
      int rank = dspace.getSimpleExtentNdims();
      vector<hsize_t> dims(rank);
      dspace.getSimpleExtentDims(dims.data(), NULL);
      //Strings need some own treatment
      //Number of vector elements
      size_t n_elmnt = dims[0];
      //Variable string length data 
      H5::StrType memtype(H5::PredType::C_S1, H5T_VARIABLE);
      //Right size of char* buffer
      std::vector<char*> attr_data(n_elmnt);
      //Read data
      attr.read(memtype, attr_data.data());
      //Convert to std::vector<std::string>
      for (size_t i = 0; i < n_elmnt; i++) {
	attr_values.push_back(std::string(attr_data[i]));
      }
      //Important: free the memory allocated by HDF5
      H5::DataType type = attr.getDataType();
      H5Treclaim(type.getId(), dspace.getId(), H5P_DEFAULT, attr_data.data());
      //H5::Attribute::freeReclaim(attr_data.data(), memtype, dspace);
    }
    catch (DataSetIException error){
      error.printErrorStack();
      return -1;
    }
    catch (AttributeIException error) {
      error.printErrorStack();
      return -1;
    }
    return 0;
  }
  
  const std::string HDF5ToLignum::getLignumTreeString(const std::string& dataset_name)const
  {
    std::string lignum_string;
    try{
      Exception::dontPrint();
      DataSet dataset = hdf5_file.openDataSet(dataset_name);
      DataSpace dataspace = dataset.getSpace();
      //int rank = dataspace.getSimpleExtentNdims();
      //cout << "Dataspce rank " << rank <<endl;
      StrType strdatatype(PredType::C_S1, H5T_VARIABLE);
      DataSpace strdataspace(H5S_SCALAR);
      std::string lignum_string;
      dataset.read(lignum_string,strdatatype, strdataspace);
      dataset.close();
      return lignum_string;
    }
    catch (H5::FileIException e){
      cout << "Could not open dataset: " << dataset_name <<endl;
    }
    return lignum_string;
  }

  const GridData HDF5ToVoxelSpace::getVoxelSpace(const string& dataset_name, vector<double>& v)
  {
    try{
      Exception::dontPrint();
      DataSet dataset = hdf5_file.openDataSet(dataset_name);
      DataSpace dataspace = dataset.getSpace();
      int rank = dataspace.getSimpleExtentNdims(); // Should be 4
      vector<hsize_t> dims(rank);
      dataspace.getSimpleExtentDims(dims.data(), NULL);
      int x = dims[0];
      int y = dims[1];
      int z = dims[2];
      int d = dims[3];
      v.resize(x*y*z*d,0.0);
      dataset.read(v.data(), PredType::NATIVE_DOUBLE);
      vector<double> v_attr;
      readDataSetAttribute(dataset_name,VB_EDGE_SIZE_NAME,v_attr);
      std::tuple<int,int,int,int> data_dim(x,y,z,d);
      std::tuple<double,double,double> edge_dim(v_attr[0],v_attr[1],v_attr[2]);
      GridData grid_data(data_dim,edge_dim);
      return grid_data;
    }
    catch (H5::FileIException e){
      cout << "Could not open dataset: " << dataset_name <<endl;
    }
    catch (H5::AttributeIException e){
      cout << "Could not open attribute: " << VB_EDGE_SIZE_NAME << endl;
    }
    return GRID_DATA_ERROR;
  }
    
}
