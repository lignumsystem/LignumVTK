# ------------------------------------------
# For Unix Makefile build system for Release:
# mkdir release
# cd release
# cmake .. -DCMAKE_BUILD_TYPE=Release
# make install
#--------------------------------------------
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

#Set VTK_DIR to point to directory where VTKConfig.cmake or vtk-config.cmake is located (compiling from source)
#Leave commented (do not set) if VTK is installed with package management system (MacPorts for example)
#set(VTK_DIR ../../VTK/build)
project(LignumVTK DESCRIPTION "Create VTK files from Lignum XML files")
#set(CMAKE_CXX_COMPILER /opt/local/bin/clang++-mp-21)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include(ExternalProject)
include(CMakePrintHelpers)
find_package(HDF5 COMPONENTS C CXX HL REQUIRED)
#Use one of Qt4, Qt5 or Qt6 for XML parsing
#Qt4 is obsolete and only for macOS with Intel processors
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

cmake_print_variables(CMAKE_CXX_COMPILER)
cmake_print_variables(CMAKE_CXX_COMPILER_ID) 
cmake_print_variables(CMAKE_CXX_STANDARD)
cmake_print_variables(CMAKE_CXX_FLAGS)
cmake_print_variables(CMAKE_OSX_DEPLOYMENT_TARGET)
#cmake_print_variables(VTK_LIBRARIES)

if (NOT VTK_FOUND)
  message(FATAL_ERROR "LignumVTK: Unable to find the VTK build folder.")
endif()

ExternalProject_Add(lignum-core
SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..
DOWNLOAD_COMMAND "")

#The '*.cmake' files for dependencies are preproduced with 'cmake' configuration
#for 'lignum-core' (see lignum-core/CMakeLists.txt).  
include(../c++adt/lib/cxxadt.cmake)
include(../Firmament/lib/sky.cmake)
include(../stl-lignum/lib/LGM.cmake)
include(../stl-voxelspace/lib/voxel.cmake)
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

# Prevent a "command line is too long" failure in Windows.
#set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
#add_executable(ParametricSpline MACOSX_BUNDLE ${SOURCE_FILES})
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

target_link_libraries(lignumvtk PRIVATE ${VTK_LIBRARIES} HDF5::HDF5  Qt5::Xml Qt5::Core sky LGM cxxadt)
target_link_libraries(vsvtk PRIVATE ${VTK_LIBRARIES} HDF5::HDF5  Qt5::Xml Qt5::Core sky LGM cxxadt)
add_dependencies(lignumvtk lignum-core)
add_dependencies(vsvtk lignum-core)

# vtk_module_autoinit is needed
#The VTK render window is nonresponsive if vtk_module_autoinit is not done
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
