#ifndef VOXELSPACETOVTK_H
#define VOXELSPACETOVTK_H
#include <cassert>
#include <filesystem>
#include <string>
#include <LignumVTK.h>
#include <TMatrixN.h>
namespace lignumvtk{

  ///\brief Transform VoxelSpace data to a VTK file for visualization
  ///
  ///Convert VoxelSpace and data to vtkStructuredGrid representation.
  ///Write vtkStructuredGrid to file. The primary element in vtkStructuredGrid
  ///is implicitely hexhedra (voxel) in 3D.
  class VoxelSpaceToVTK{
  public:
     ///\brief Constructor
    ///
    ///Create vtkStructuredGrid \p sgrid with default dimensions and grid points.
    ///\param nx Number of voxels in X dimension
    ///\param ny Number of voxels in Y dimension
    ///\param nz Number of voxels in Z dimension
    ///\post The edge length of a hexahedra grid element is 1
    ///\remark The point ordering in the 3D structured grid is i-j-k
    ///with i increasing fastest, then j and k is the slowest.
    VoxelSpaceToVTK(int nx, int ny, int nz);
    ///\brief Constructor
    ///
    ///Create vtkStructuredGrid \p sgrid with dimensions and grid points based on edge lengths.
    ///\param nx Number of vo in X dimension
    ///\param ny Number of voxels in Y dimension
    ///\param nz Number of voxels in Z dimension
    ///\param x_edge Length of a voxel edge in X dimension
    ///\param y_edge Length of a voxel edge in Y dimension
    ///\param z_edge Length of a voxel edge in Z dimension
    ///\remark The point ordering in the 3D structured grid
    ///is i-j-k with i increasing fastest, then j and k is the slowest.  
    VoxelSpaceToVTK(int nx, int ny, int nz, double x_edge, double y_edge, double z_edge);
    ///\brief Insert VoxelSpace data to \p sgrid.
    ///
    ///Data from the matrix \p vsm  is inserted to \p sgrid as cell data named attributes.
    ///The index of an attribute  name in the \p attr_names vector corresponds
    ///to the index in the data (4th) dimension of \p vsm.
    ///\param vsm 4D Matrix representing VoxelSpace spatial dimensions and data
    ///\param attr_names Attribute names
    ///\pre Dimensions between \p sgrid and \p vsm match
    ///\pre The length of \p attr_data equals \p vsm.d()
    ///\retval *this VoxelSpaceToVTK object
    ///\sa sgrid
    VoxelSpaceToVTK& insertVoxelSpaceData(const cxxadt::TMatrix4D<double>& vsm,const vector<string>& attr_names);
    ///\brief Write \p sgrid to a file
    ///\param file_name File name
    ///\retval *this VoxelSpaceToVTK object
    ///\sa sgrid
    VoxelSpaceToVTK& writeStructuredGrid(const std::string& file_name);
  private:
    ///\brief Structured grid for VoxelSpace dimensions and data representation
    vtkNew<vtkStructuredGrid> sgrid;
    ///\brief Structureg grid writer
    vtkNew<vtkXMLStructuredGridWriter> writer;
  };
}
#endif
