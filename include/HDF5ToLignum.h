#ifndef HDF5_TO_LIGNUM_H
#define HDF5_TO_LIGNUM_H
#include <LGMHDF5File.h>
#include <Lignum.h>


  
namespace lignumvtk{

  ///\brief Helper function for H5Ovisit to collect HDF5 dataset paths
  ///\param loc_id HDF5 group id
  ///\param [out] name HDF5 dataset name
  ///\param [out] info  HDF5 gropu or dataset information
  ///\param [in,out] data User data 
  herr_t InsertDataSet(hid_t loc_id, const char* name, const H5O_info_t* info, void* data);

  ///\brief Collect dataset names
  ///
  ///Helper class for InsertDataSet.
  ///\sa InsertDataSet
  class DataSetNames{
  public:
    vector<std::string> names;///< Vector for dataset paths
    std::string prefix;///< Prefix for the dataset path name 
  };

  ///\brief Query datasets from HDF5 file.
  ///
  ///Query dataset paths. Retrieve Lignum xml as std::string from
  ///given dataset.
  class HDF5ToLignum{
  public:
    ///\brief Open HDF5 file
    ///\param file_name HDF5 file
    HDF5ToLignum& openFile(const string& file_name);
    ///\brief Destructor
    ///
    ///Close HDF5 file
    ~HDF5ToLignum();
    ///\brief Query dataset names base on HDF5 path
    ///\param name HDF5 path
    ///\return Vector of HDF5 paths
    vector<std::string>& getDataSetNames(const string& name);
    ///\brief Query dataset names base on simulation year
    ///\param year Simulation year
    ///\return Vector of HDF5 path names
    vector<std::string>& getDataSetNames(int year);
    ///\brief Query Lignum XML string
    ///\param dataset_name HDF5 dataset path for the XML sring
    ///\return Lignum xml as std:;string
    const string getLignumTreeString(const string& dataset_name);
    ///\brief Root group name for Lignum XML trees
    ///\return The string `/TreeXML/`
    ///\sa LignumForest::TXMLGROUP
    const string getTreeGroupName(){return std::string("/TreeXML/");}
  private:
    DataSetNames dataset_names;///< Collection of HDF5 path names
    H5::H5File hdf5_file;///< HDF5 file
  };
}
#endif
