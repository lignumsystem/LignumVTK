# LignumVTK
`lignumvtk` and `vsvtk` programs create VTK files for [ParaView](https://www.paraview.org/). 
ParaView is a 2D and 3D computer geometry model builder, data analysis and  visualization software 
for scientific data. The VTK library is the visualization toolkit ParaView relies on.

`lignumvtk` and `vsvtk` map Lignum tree models and voxel spaces to VTK's geometric objects 
and their attributes for the final visualization with ParaView graphics pipelines. This post-processing 
requires neither programming skills nor knowledge of VTK library. Primary purpose of the visualization
is to extract qualitative insight from simulation results.

## Building LignumVTK
### Prerequisites 
Download *LignumVTK* and *lignum-core* from GitHub. Install the VTK library package.
Using MacPorts type:

	sudo port install vtk

Download [ParaView](https://www.paraview.org). For macOS be careful to download for the right 
processor architecture. MacPorts version reports poor port health, i.e. it is likely not 
in working order. 

### CMake
Use CMake to compile `lignumvtk` and `vsvtk`:
	
	mkdir release #In LignumVTK directory
	cd release
	cmake .. -DCMAKE_BUILD_TYPE=Release
	make install

The `lignumvtk` and `vsvtk` binaries are installed in the LignumVTK directory. 
See the CMakeLists.txt file for details.

## Trees and forest stands
The `lignumvtk` program can produce VTK/VTPC[^vtpc] files from Lignum XML and HDF5 files.
Input data for `lignumvtk` is either a single XML tree file or an HDF5 file containing 
the tree datasets in XML format. Trees are reconstructed and converted to VTK spline tube segments for
wooden parts and conifer foliage, and triangle strips for Kite shaped hardwood leaves. 
Attributes are assigned from Lignum trees to describe relevant properties in visualization. 
`lignumvtk` can implicitely recognize between conifers and hardwood trees.

### Command line 
The `lignumvtk` command line is:

	./lignumvtk -help|-h
	./lignumvtk -input|-i path/to/file.[h5|xml] [-list] [-output|-o path/to/file.vtpc] [-year <number>] \
	[-dataset <string>] [-substring <string>] [-spline <number>] [-sides <number>] [-view 0|1]
	
### Examples 
**Example 1**: List datasets for trees in Lignum HDF5 file:

	./lignumvtk -input File.h5 -list          # List all trees
	./lignumvtk -input File.h5 -list -year 80 # List trees for the year 80
	
**Example 2**: Create VTK/VTPC file from Lignum XML file:
	
	./lignumvtk -input File.xml -output File.vtpc

**Example 3**: Create VTK/VTPC file for all trees for the year 20:

	./lignumvtk -input File.h5 -output File.vtpc -year 20
	
**Example 4**: Create VTK/VTPC file for the dataset Tree_8 in /TreeXML/60/Tree_8:

	./lignumvtk -input File.h5 -output File.vtpc -dataset /TreeXML/60/Tree_8
	
**Example 5**: Create VTK/VTPC file for Tree_8 for all growth years saved in HDF5 file:

	./lignumvtk -input File.h5 -output File.vtpc -dataset Tree_8
	
**Example 6**: Create VTK/VTPC file for trees for all years matching the string Tree_11:

	./lignumvtk -input File.h5 -output File.vtpc -substring Tree_11
	
**Example 7**: Divide tree segments into 10 spline segments. Assign 20 rectangular sides 
to spline tubes approximating tube cylinders.

	./lignumvtk -input File.h5 -output File.vtpc -substring Tree_11 -spline 10 -sides 20

**Example 8**: Create component view:

		./lignumvtk -input File.h5 -output File.vtpc -dataset Tree_8 -view 1
		
The option *-dataset* uses exact match to find the dataset, the option *-substring* searches 
dataset paths for substring match. The options *-year*, *-dataset* and *-substring* are mutually exclusive.
The options *-spline* and *-sides* determine the spline tube segment length and roundness.

The option *-view* decides how VTK datasets are assembled for ParaView. Default is 
tree view (*-view 0*) where spline segment tubes and foliage are assembled in trees 
where they belong. Component view (*-view 1*) groups these geometry models into foliage, 
segment and heartwood data. The former allows easy selection of trees for closer inspection, 
the latter is intended to visualize forest stands. 

The main VTK/VTPC output file (suffix *.vtpc*) is a meta file telling the location and names 
of the actual 3D geometry model files. The location is the directory named after the 
main output file and created automatically when saving the file. Open the main output file 
in ParaView and create ParaView's graphics pipeline for visualization.

> [!IMPORTANT]
> The default values for the options `-spline` and `-sides` should be applicable 
> for a single Lignum tree but for a forest stand it may be necessary 
> reduce these numbers in compliance with the available memory and to shorten the rendering time.

> [!CAUTION]
> Unduly generic dataset path argument to the `-substring` option can retrieve 
> significant number of tree datasets, possibly all of them.

> [!NOTE]
> During reconstruction of trees warning messages related to missing function files can appear. This is due to 
> default file names for tree simulations and does not prevent the creation of VTK/VTPC files.
> If disturbing create dummy files in the working directory to remove the warning messages.

## Voxel spaces
The `vsvtk` program can produce VTK/VTS[^vts] file from HDF5 voxel space dataset file.
Voxel space and voxel data 4D matrix representation is rebuilt, converted
to a VTK structured grid with hexahedra (voxel) elements coupled with voxel data 
as attributes and saved as VTK/VTS file. Default hexahedron edge size is the voxel edge size 
used in a simulation.

### Command line
The `vsvtk` command line is:

	./vsvtk [-help|-h] -input|-i File.h5 [-list] [-output|-o File.vts] [-dataset <path_string>] [-edge <number>]
	
### Examples
**Example 1**: List voxel space datasets:

	./vsvtk -i File.h5 -list
	
**Example 2**: Read voxel space dataset from an HDF5 file and create VTK/VTS file. 
Set hexahedron (voxel) edge size to 3:
	
	./vsvtk -i File.h5 -dataset /VoxelSpaceData60 -o File.vts -edge 3

The `vsvtk` program can create  VTK/VTS file for one voxel space dataset at a time. 
Open the VTK/VTS file in ParaView and create ParaView's graphics pipeline for visualization.

## ParaView settings
The 3D computer graphics geometry models may require a fair amount of computer memory. 
ParaView can visualize single trees and forest stands produced by `lignumvtk` in MacBooks 
using its default rendering options although the rendering process may take a while. 

Change ParaView rendering options from Preferences via RenderView tab.
Lower the values for LOD and Outline Threshold if needed before opening a VTK file. 
These two options decrease memory requirments at the expense of rendering quality. 

If ParaView crashes during a rendering process it can indicate failed memory request 
when computing the graphics pipeline. However, before that happens modern macOS versions 
can usually intervene and display the Force-Quit window presenting processes
having caused problems.

## Software documentation
LignumVTK c++ source files have Doxygen documentation:

	doxygen Doxyfile 2> error.txt
	
Open the HTML index file (macOS Terminal):

	open DoxygenDoc/html/index.html

## Blender
Another 3D computer graphics creation suite is [Blender](https://www.blender.org).
File formats supported by both ParaView and Blender are Alembic (*.abc*), Wavefront OBJ (*.obj*),
Stanford PLY (*.ply*) and STL (*.stl*) each with their pros and cons. Add-ons like SciBlend 
for Blender are also available for file transfer. 

### lignumvtk.py
`lignumvtk.py` is an initial trial to use *vtk* library but mentioned here because it was used
to visualize tree roots with Lignum and ParaView. The tree roots for Lignum model are produced in 
the project FineRoots. The `lignumvtk.py` program reads Lignum XML files and creates VTK/VTM files
for ParaView. `lignumvtk.py` is a Python 3 program that requires *numpy* and *vtk* Python packages. 
Install these with `pip`.

## Notes
> [!NOTE] 
> The creation of the VTK/VTPC tree files can take a while, possibly several minutes 
> depending the number of trees and their dataset sizes. 

> [!NOTE]
> As the total size of VTK data files can quickly add up pay attention to available disk space.
> It may be useful to save VTK files under separate directory structure for easy removals. 

> [!NOTE]
> LignumVTK is work under progress. `lignumvtk` supports kite shaped leaves. Ellipse and triangle leaves 
> will be implemented when needed.

### VTK Examples
[VTK Examples](https://examples.vtk.org/site/) has been pivotal source of information for LignumVTK
implementation.

[^vtpc]: VTK Partitioned Data Set Collection file format metadata header file.

[^vts]: VTK Structured Grid file format.

[^lod]: Level of Detail (LOD) refers to the dynamic complexity of a 3D model representation. 

[^ot]: The Outline Threshold value determines if an object is approximated as a bounding box only.
