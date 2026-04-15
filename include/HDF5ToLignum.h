#ifndef HDF5_TO_LIGNUM_H
#define HDF5_TO_LIGNUM_H
#include <tuple>
#include <LGMHDF5File.h>
#include <Lignum.h>


  
namespace lignumvtk{
  ///\brief VoxelSpace data for VTK vtkStructuredGrid
  ///
  ///To visualize VoxelSpace necessary data is in std::pair.
  ///The first 4-tuple contains the 4D-matrix dimensions, number of voxels in x,y,z directions
  ///and VoxelSpace data. The second 3-tuple contains voxel edge sizes in x,y,z directions.
  ///\sa HDF5ToVoxelSpace::getVoxelSpace()
  typedef std::pair<std::tuple<int,int,int,int>,std::tuple<double,double,double>> GridData;
  ///\brief Error value if HDF5ToVoxelSpace::getVoxelSpace() fails
  const GridData GRID_DATA_ERROR(std::tuple<int,int,int,int>{EXIT_FAILURE,EXIT_FAILURE,EXIT_FAILURE,EXIT_FAILURE},
				 std::tuple<double,double,double>{EXIT_FAILURE,EXIT_FAILURE,EXIT_FAILURE});
  ///\brief Trees are stored in HDF5 under "/TreeXML/" main group
  ///\sa LignumForest::TXMLGROUP
  const string TXMLGROUP= "/TreeXML/";
  ///\brief VoxelSpace main group
  ///\sa LignumForest::VOXELSPACEGROUP
  const string VOXELSPACEGROUP("/VoxelSpace/");
  ///\brief Attribute name for voxel edge sizes
  ///\sa LignumForest::VB_EDGE_SIZE_NAME
  const string VB_EDGE_SIZE_NAME("VoxelEdgeSizeXYZ");
  
  ///\brief Helper function for H5Ovisit to collect HDF5 dataset paths
  ///\param loc_id HDF5 group id
  ///\param [out] name HDF5 dataset name
  ///\param [out] info  HDF5 gropu or dataset information
  ///\param [in,out] data User data 
  herr_t InsertDataSet(hid_t loc_id, const char* name, const H5O_info_t* info, void* data);

  ///\brief Collect dataset names
  ///
  ///Helper class for InsertDataSet.
  ///\sa lignumvtk::InsertDataSet
  class DataSetNames{
  public:
    vector<std::string> names;///< Vector for dataset paths
    std::string prefix;///< Prefix for the dataset path name 
  };

  ///\brief Query dataset paths  from HDF5 file.
  ///
  ///Query dataset paths.
  class HDF5Base{
  public:
    ///\brief Open HDF5 file
    ///\param file_name HDF5 file
    HDF5Base& openFile(const string& file_name);
    ///\brief Destructor
    ///
    ///Close HDF5 file
    ~HDF5Base();
    ///\brief Virtual method for the main HDF5 group of interest
    virtual const string getMainGroupName()const=0;
    ///\brief  HDF5 dataset root
    ///\retval "/" The HDF5 dataset root 
    const string getRoot()const{return "/";}
    ///\brief Query dataset names base on simulation year
    ///\param year Simulation year
    ///\return Vector of HDF5 path names
    vector<std::string>& getDataSetNames(int year);
    ///\brief Query dataset names based on HDF5 path
    ///\param name HDF5 path
    ///\return Vector of HDF5 paths
    vector<std::string>& getDataSetNames(const string& name);
    ///\brief Read dataset scalar attribute
    ///\param[in] dset_name Dataset name
    ///\param[in] attr_name Attribute name
    ///\param[out] value Attribute value
    ///\retval -1 Error
    ///\retval 0 Normal return
    ///\exception DataSetIException
    ///\exception AttributeIException
    int readDataSetAttribute(const string& dset_name, const string& attr_name,double& attr_value);
    ///\brief Read dataset attribute values
    ///
    ///Read dataset \e double type attribute values with vector
    ///\param[in] dset_name Dataset name
    ///\param[in] attr_name Attribute name
    ///\param[out] attr_values Attribute values
    ///\retval -1 Error
    ///\retval 0 Normal return
    ///\exception DataSetIException
    ///\exception AttributeIException
    int readDataSetAttribute(const string& dset_name, const string& attr_name,vector<double>& attr_values);
    ///\brief Read dataset attribute values
    ///
    ///Read dataset \e string type attribute values with vector
    ///\param[in] dset_name Dataset name
    ///\param[in] attr_name Attribute name
    ///\param[out] attr_values Attribute values
    ///\retval -1 Error
    ///\retval 0 Normal return
    ///\exception DataSetIException
    ///\exception AttributeIException
    int readDataSetAttribute(const string& dset_name, const string& attr_name,vector<string>& attr_values);
  protected:
    DataSetNames dataset_names;///< Collection of HDF5 path names
    H5::H5File hdf5_file;///< HDF5 file
  };

  ///\brief Read Lignum trees from HDF5 file
  class HDF5ToLignum: public HDF5Base{
  public:
    ///\brief Query Lignum XML string
    ///\param dataset_name HDF5 dataset path for the XML sring
    ///\return Lignum xml as std:;string
    const string getLignumTreeString(const string& dataset_name)const;
    ///\brief Root group name for Lignum XML trees
    ///\retval TXMLGROUP The main tree group name
    const string getMainGroupName()const override{return TXMLGROUP;}
  };

  ///\brief Read VoxelSpace from HDF5 file
  ///
  class HDF5ToVoxelSpace: public HDF5Base{
  public:
    ///\brief Root group name for Voxel Spaces
    ///\retval VOXELSPACEGROUP The main group for VoxelSpaces
    const string getMainGroupName()const override{return VOXELSPACEGROUP;}
    ///\brief Read one VoxelSpace from dataset and return data to construct vtkStructuredGrid.
    ///
    ///Read and return VoxelSpace data as well as number of voxels in each (x,y,z) dimensions
    ///and voxel edge lengths.
    ///\param[in] dataset_name VoxelSpace dataset name
    ///\param[in,out] v The vector to contain VoxelSpace data
    ///\return v VoxelSpace data in a row-first vector
    ///\return GridData Pair for 4-tuple of number of voxels and voxel data and 3-tuple of voxel edge lengths
    ///\return GridData with EXIT_FAILURE entries if dataset reading error
    ///\exception H5::FileIException Dataset reading error
    ///\exception H5::AttributeIException Dataset attribute reading error
    ///\sa LignumForest::VB_DATA_COLUMN_NAMES
    ///\sa LignumForest::VB_DATA_NAMES
    ///\sa LignumForest::VB_EDGE_SIZE_NAME 
    ///\sa GridData
    const GridData getVoxelSpace(const string& dataset_name,vector<double>& v);
  };
}
#endif
