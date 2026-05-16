# ------------------------------------------
# For Unix Makefile build system for Release:
# mkdir release
# cd release
# cmake .. -DCMAKE_BUILD_TYPE=Release
# make install
#--------------------------------------------
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

#Set VTK_DIR to point to directory where VTKConfig.cmake or vtk-config.cmake is located when compiling from source.
#Leave commented (do not set) if VTK is installed with package management system (MacPorts for example)
#set(VTK_DIR ../../VTK/build)
project(LignumVTK DESCRIPTION "Create VTK files from Lignum XML files")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include(ExternalProject)
include(CMakePrintHelpers)

#Using "add_subdirectory" aligns with modern CMake as it supports namespaced targets
#and there is no need to use explicit directory paths.
#Also, the maintenance is easier - to refer to "lignum-core" relative to LignumForest
#change the first argument "../" to point to "lignum-core" CMakeLists.txt location.
#"lignum-core_build" is the local build directory for lignum-core,
#located inside the LignumVTK build directory.
add_subdirectory(../ lignum-core_build)

find_package(HDF5 COMPONENTS C CXX HL REQUIRED)

#Use one of Qt4, Qt5 or Qt6 for XML parsing
#Qt4 is obsolete in macOS Tahoe and available only for macOS with Intel processors
#find_package(Qt4 COMPONENTS QtOpenGL QtXml QtGui QtCore REQUIRED)
#Qt5 package
find_package(Qt5 COMPONENTS Xml Core  REQUIRED)
#Qt6 package
#find_package(Qt6 COMPONENTS Xml Core  REQUIRED)
find_package(VTK REQUIRED COMPONENTS 
  CommonColor
  CommonComputationalGeometry
  CommonCore
  CommonDataModel
  FiltersSources
  InteractionStyle
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
  IOLegacy
  IOXML
  IOParallelXML
)
if (NOT VTK_FOUND)
  message(FATAL_ERROR "LignumVTK: Unable to find VTK.")
endif()

cmake_print_variables(CMAKE_CXX_COMPILER)
cmake_print_variables(CMAKE_CXX_COMPILER_ID) 
cmake_print_variables(CMAKE_CXX_STANDARD)
cmake_print_variables(CMAKE_CXX_FLAGS)
cmake_print_variables(CMAKE_OSX_DEPLOYMENT_TARGET)
cmake_print_variables(VTK_LIBRARIES)

set(SOURCE_FILES_LGM lignumvtk.cc
  src/LignumVTKTree.cc
  src/LignumVTKXML.cc
  src/HDF5ToLignum.cc
  src/CreateVTPCFile.cc
)

set(SOURCE_FILES_VS vsvtk.cc
  src/LignumVTKTree.cc
  src/LignumVTKXML.cc
  src/HDF5ToLignum.cc
  src/CreateVTPCFile.cc
  src/VoxelSpaceToVTK.cc
)

#Two examples from VTK CMakeLists.txt
#Prevent a "command line is too long" failure in Windows.
#set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
#add_executable(ParametricSpline MACOSX_BUNDLE ${SOURCE_FILES})

#Targets for trees and voxel space 
add_executable(lignumvtk ${SOURCE_FILES_LGM})
add_executable(vsvtk ${SOURCE_FILES_VS})

target_include_directories(lignumvtk PUBLIC
  include
  ${CMAKE_CURRENT_LIST_DIR}/../XMLTree
  ${CMAKE_CURRENT_LIST_DIR}/../lignum-tls/include
)
target_include_directories(vsvtk PUBLIC
  include
  ${CMAKE_CURRENT_LIST_DIR}/../XMLTree
  ${CMAKE_CURRENT_LIST_DIR}/../lignum-tls/include
)

target_link_libraries(lignumvtk PRIVATE ${VTK_LIBRARIES} HDF5::HDF5  Qt5::Xml Qt5::Core sky::sky LGM::LGM cxxadt::cxxadt)
target_link_libraries(vsvtk PRIVATE ${VTK_LIBRARIES} HDF5::HDF5  Qt5::Xml Qt5::Core sky::sky LGM::LGM cxxadt::cxxadt)


#The CMake command vtk_module_autoinit is used in VTK9+ to automatically trigger
#static code construction and object factory registration required by certain VTK modules.
#For example the VTK render window may become unresponsive and mappers-related
#runtime errors - such as  "Error: no override found for..." - may appear.
vtk_module_autoinit(
  TARGETS lignumvtk vsvtk
  MODULES ${VTK_LIBRARIES}
)

add_custom_command(TARGET lignumvtk
  COMMAND echo "Compilation for lignumvtk  done"
  POST_BUILD
  COMMENT "REMEMBER to type \"make install\" to install \"lignumvtk\" into the LignumVTK working directory above."
)
add_custom_command(TARGET vsvtk
  COMMAND echo "Compilation for vsvtk  done"
  POST_BUILD
  COMMENT "REMEMBER to type \"make install\" to install \"vsvtk\" into the LignumVTK working directory above."
)

install(TARGETS lignumvtk vsvtk DESTINATION "${CMAKE_CURRENT_LIST_DIR}")
