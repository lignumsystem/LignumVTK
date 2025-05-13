#ifndef CREATEVTPCFILE_H
#define CREATEVTPCFILE_H
#include <Lignum.h>
#include <XMLTree.h>
#include <HDF5ToLignum.h>
#include <LignumVTK.h>
#include <LignumVTKXML.h>
#include <LignumVTKTree.h>
namespace lignumvtk{

  ///\brief Find HDF5 dataset path with substring
  class FindSubstring{
  public:
    ///\brief Constructor
    ///\param path HDF5 dataset path
    FindSubstring(const string& path):substring(path){}
    ///\brief Find the first occurence of \p substring in \p path
    ///\param path HDF5 dataset path
    ///\return true if \p substring is in \p path, false otherwise
    bool operator()(const string& path){
      std::string::size_type n = path.find(substring);
      if (std::string::npos == n){
	return false;
      }
      else{
	return true;
      }
    }
  private:
    const string substring;///< HDF5 path to search  
  };

  ///\brief Find HDF5 dataset with exact match.
  ///
  ///The HDF5 dataset  is assumed to be the last element in the HDF5 dataset path .
  class FindExactMatch{
  public:
    ///\brief Constructor
    ///\param path HDF5 dataset name
    FindExactMatch(const string& path):exact_name(path){}
    ///\brief Find dataset path with \p exact_match as suffix 
    ///
    ///\param path HDF5 dataset path
    ///return  true if \p path ends with \p exact_match, false otherwise
    bool operator()(const string& path){
      std::string::size_type n = path.rfind(exact_name);
      if (std::string::npos == n){
	return false;
      }
      else{
	//If distance equals the the length of substring then suffix is found
	std::string::size_type dist = path.size()-n; 
	std::string::size_type l = exact_name.size();
	if (dist == l){
	  return true;
	}
	else{
	  return false;
	}
      }
    }
  private:
    const string exact_name;///<HDF5 dataset name
  };
  
  ///\brief Create VTK/VTPC file from Lignum XML
  ///\param input_file Lignum XM file
  ///\param output_file VTK/VTPC file
  ///\return EXIT_SUCCESS if VTK/VTPC file is created, EXIT_FAILURE otherwise
  int CreateVTPCFileFromXML(const string& input_file,const string& output_file);
  ///\brief Create VTK/VTPC file from Lignum HDF5 file
  ///\param input_file Lignum XM file
  ///\param output_file VTK/VTPC file
  ///\param year Simulation year group in HDF5 file
  ///\return EXIT_SUCCESS if VTK/VTPC file is created, EXIT_FAILURE otherwise
  int CreateVTPCFileFromHDF5(const string& input_file,const string& output_file,int year);
  ///\brief Create VTK/VTPC file from Lignum HDF5 file
  ///
  ///Collect datasets matching  \p dataset_path string and create VTK/VTPC file for those datasets
  ///\param input_file Lignum XM file
  ///\param output_file VTK/VTPC file
  ///\param dataset_path HDF5 path substring
  ///\param exact_match If true Search as suffix if false search as substring 
  ///\return EXIT_SUCCESS if VTK/VTPC file is created, EXIT_FAILURE otherwise
  int CreateVTPCFileFromHDF5(const string& input_file,const string& output_file, const string& dataset_path, bool exact_match);  
}
#endif
