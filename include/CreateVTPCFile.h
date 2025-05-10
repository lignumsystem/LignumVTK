#ifndef CREATEVTPCFILE_H
#define CREATEVTPCFILE_H
#include <Lignum.h>
#include <XMLTree.h>
#include <HDF5ToLignum.h>
#include <LignumVTK.h>
#include <LignumVTKXML.h>
#include <LignumVTKTree.h>
namespace lignumvtk{
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
  ///\return EXIT_SUCCESS if VTK/VTPC file is created, EXIT_FAILURE otherwise
  int CreateVTPCFileFromHDF5(const string& input_file,const string& output_file, const string& dataset_path);  
}
#endif
