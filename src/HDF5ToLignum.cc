#include <HDF5ToLignum.h>

namespace lignumvtk{
  herr_t InsertDataSet(hid_t loc_id, const char* name, const H5O_info_t* info, void* operator_data)
  {
    auto dataset_names = static_cast<DataSetNames*>(operator_data);
    if(info->type == H5O_TYPE_DATASET){
      std::string prefix = dataset_names->prefix;
      dataset_names->names.push_back(prefix+std::string(name));
    }
    return 0;
  }

  HDF5ToLignum& HDF5ToLignum::openFile(const string& name)
  {
    hdf5_file.openFile(name,H5F_ACC_RDONLY);
    return *this;
  }

  HDF5ToLignum::~HDF5ToLignum()
  {
    hdf5_file.close();
  }

  vector<string>& HDF5ToLignum::getDataSetNames(const string& name)
  {
    dataset_names.names.clear();
    dataset_names.prefix=name;
    try{
      Exception::dontPrint();
      H5::Group g = hdf5_file.openGroup(name);
      hid_t group_id = g.getId();
      herr_t status;
      status = H5Ovisit(group_id, H5_INDEX_NAME, H5_ITER_NATIVE,InsertDataSet,static_cast<void*>(&dataset_names),H5O_INFO_ALL);
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
  
  vector<string>& HDF5ToLignum::getDataSetNames(int year)
  {
    dataset_names.names.clear();
    string group_name = getTreeGroupName()+std::to_string(year)+std::string("/");
    dataset_names.prefix = group_name;
    H5::Group g = hdf5_file.openGroup(group_name);
    hid_t group_id = g.getId();
    herr_t status;
    status = H5Ovisit(group_id, H5_INDEX_NAME, H5_ITER_NATIVE,InsertDataSet,static_cast<void*>(&dataset_names),H5O_INFO_ALL);
    g.close();
    return dataset_names.names;
  }

  const std::string HDF5ToLignum::getLignumTreeString(const std::string& dataset_name)
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
}
