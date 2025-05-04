# ------------------------------------------
# For Unix Makefile build system for Release:
# mkdir release
# cd release
# cmake .. -DCMAKE_BUILD_TYPE=Release
# make install
#--------------------------------------------
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(LignumVTK DESCRIPTION "Create VTK files from Lignum XML files")
include(ExternalProject)
include(CMakePrintHelpers)
find_package(Qt5 COMPONENTS Xml Core  REQUIRED)
find_package(VTK COMPONENTS 
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
  message(FATAL_ERROR "ParametricSpline: Unable to find the VTK build folder.")
endif()

ExternalProject_Add(lignum-core
SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..
DOWNLOAD_COMMAND "")

#cmake_print_variables(VTK_LIBRARIES)

#The '*.cmake' files for dependencies are preproduced with 'cmake' configuration
#for 'lignum-core' (see lignum-core/CMakeLists.txt).  
include(../c++adt/lib/cxxadt.cmake)
include(../Firmament/lib/sky.cmake)
include(../stl-lignum/lib/LGM.cmake)
include(../stl-voxelspace/lib/voxel.cmake)
set(SOURCE_FILES lignumvtk.cc
  src/LignumVTKTree.cc
)

# Prevent a "command line is too long" failure in Windows.
#set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
#add_executable(ParametricSpline MACOSX_BUNDLE ${SOURCE_FILES})
add_executable(lignumvtk ${SOURCE_FILES})
target_include_directories(lignumvtk PUBLIC
  include
  ${CMAKE_CURRENT_LIST_DIR}/../XMLTree
  ${CMAKE_CURRENT_LIST_DIR}/../lignum-tls/include
)
target_compile_options(lignumvtk PUBLIC -std=c++17)
target_link_libraries(lignumvtk PRIVATE ${VTK_LIBRARIES} Qt5::Xml Qt5::Core sky LGM cxxadt)
add_dependencies(lignumvtk lignum-core)
# vtk_module_autoinit is needed
#The VTK render window is nonresponsive if vtk_module_autoinit is not done
vtk_module_autoinit(
  TARGETS lignumvtk
  MODULES ${VTK_LIBRARIES}
)
install(TARGETS lignumvtk DESTINATION "${CMAKE_CURRENT_LIST_DIR}")
