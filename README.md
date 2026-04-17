# LignumVTK
The tools `lignumvtk`and `vsvtk`in the LignumVTK project can create VTK files  for [ParaView](https://www.paraview.org/) 
visualizations. ParaView is a 2D and 3D computer geometry model builder, data analysis and  visualization software 
for scientific data.  The VTK library is the visualization toolkit ParaView relies on. Primary purpose of the tools
implemented in LignumVTK is to extract qualitative insight from the simulations results.

The `lignumvtk` and `vsvtk` binaries create VTK geometric objects and their attributes  representing
Lignum tree models and voxel spaces for the final visualization with ParaView graphics pipelines.
This post-processing does not require programming skills or knowledge of VTK library. 

> [!NOTE]
> LignumVTK is work under progress. lignumvtk supports kite shaped leaves. Ellipse and triangle leaves 
> will be implemented when needed.

## Building LignumVTK
### Prerequisites 
Download *LignumVTK* and *lignum-core* from GitHub. Install the VTK library package.
Using MacPorts type:

	sudo port install vtk
	
See also generic comments in *lignum-core*.

Download [ParaView](https://www.paraview.org). For macOS be careful to download for the right 
processor architecture. Do not use MacPorts version that reports poor port health, 
i.e. it is likely not in working order. 

### Compilation
Use CMake to compile `lignumvtk` and `vsvtk`:
	
	mkdir release
	cd release
	cmake .. -DCMAKE_BUILD_TYPE=Release
	make install

The `lignumvtk` and `vsvtk` binaries are installed in the LignumVTK directory. 
See the CMakeLists.txt file for details.

### VTK Examples
[VTK Examples](https://examples.vtk.org/site/) has been the pivotal source of information in implementing LignumVTK.

## lignumvtk
The `lignumvtk` program can produce VTK/VTPC[^vtpc] files from Lignum XML and HDF5 files.
More precisly input data for `lignumvtk` is either a single XML tree file or an HDF5 file containing 
the tree datasets in XML format. Trees are reconstructed and converted to VTK spline tube segments for
wooden parts and conifer foliage and triangle strips for Kite shaped hardwood leaves. 
Attributes are assigned from Lignum trees to describe relevant properties in visualization. 
Note that `lignumvtk` can implicitely recognize between conifers and hardwood trees.

### Command line 
The `lignumvtk` command line is:

	./lignumvtk -input|-i path/to/file.[h5|xml] [-list] [-output|-o path/to/file.vtpc] [-year <number>] \
	[-dataset <string>] [-substring <string>] [-spline <number>] [-sides] -number
	
### Examples 
Example 1: List datasets for trees in Lignum HDF5 file:

	./lignumvtk -input File.h5 -list #All trees
	./lignumvtk -input File.h5 -list -year 80 #Trees for year 80
	
Example 2: Create VTK/VTPC file from Lignum XML file:
	
	./lignumvtk -input File.xml -output File.vtpc

Example 3: Create VTK/VTPC file for all trees of age 20:

	./lignumvtk -input File.h5 -output File.vtpc -year 20
	
Example 4: Create VTK/VTPC file for the dataset Tree_8 in /TreeXML/60/Tree_8:

	./lignumvtk -input File.h5 -output File.vtpc -dataset /TreeXML/60/Tree_8
	
Example 5: Create VTK/VTPC file for Tree_8 for all growth years saved in HDF5 file:

	./lignumvtk -input File.h5 -output File.vtpc -dataset Tree_8
	
Example 6: Create VTK/VTPC file for trees for all years matching the string Tree_11:

	./lignumvtk -input File.h5 -output File.vtpc -substring Tree_11
	
Example 7: Adjust the spline accuracy to 10, i.e. line segments denoting tree segments 
are divided into 10 spline segments. Adjust side accuracy to 20, i.e. the spline tube will
have 20 rectangular sides to represent tree segment cylinders:

	./lignumvtk -input File.h5 -output File.vtpc -substring Tree_11 -spline 10 -sides 20

The argument string for *-dataset* finds exact match for the dataset name, 
the option *-substring* uses the argument string to search the complete dataset path
in the HDF5 input file. The options *-year*, *-dataset* and *-substring* are mutually exclusive.

The options *-spline* and *-sides* determine the spline tube segment length and roundness.

The main VTK/VTPC output file (suffix *.vtpc*) is a meta file telling the location and names 
of the actual 3D geometry model files. The location is a subdirectory named after the main *.vtpc*
created automatically when saving the model. Upload the main *.vtpc* output file in ParaView and 
finish by post-editing trees manually in ParaView's graphics pipelines for final visualization.

> [!IMPORTANT]
> The default values for the `-spline` option 20 and for the `-sides` option 30 respectively
> should be applicable for a single Lignum tree but for a forest stand it may be necessary 
> reduce these numbers in compliance with the available memory and to shorten the rendering time.

> [!CAUTION]
> Unduly generic dataset path argument to the `-substring` option can retrieve 
> significant number of tree datasets, possibly all of them.

> [!NOTE]
> During reconstruction of trees warning messages related to missing function files can appear. This is due to 
> hard coded default file names for tree simulations and does not prevent the creation of VTK/VTPC files.
> If disturbing create dummy files in the working directory to remove the warning messages.

## vsvtk
The `vsvtk` program can produce VTK/VTS[^vts] file from HDF5 voxel space dataset file.
More precisely 4D matrix representation of voxel space data is rebuilt, converted
to a VTK structured grid with hexahedra voxel (grid) elements coupled with voxel space data 
as attributes and saved as VTK/VTS file. Default hexahedra edge size is the voxel edge size 
used in a simulation.

### Command line
The `vsvtk` command line is:

	./vsvtk [-help|-h] -input|-i File.h5 [-list] [-output|-o File.vts] [-dataset <path_string>] [-edge <number>]
	
### Examples
Example 1: List voxel space datasets:

	./vsvtk -i File.h5 -list
	
Example 2: Read voxel space dataset from an HDF5 file and create VTK/VTS file. 
Set hexahedra (voxel) edge size to 3:
	
	./vsvtk -i File.h5 -dataset /VoxelSpaceData60 -o File.vts -edge 3

The `vsvtk` program can create one voxel space dataset VTK/VTS file at a time. 

## ParaView settings
The 3D computer geometry models can require a fair amount of computer memory. 
ParaView can visualize single trees and forest stands produced by `lignumvtk` with MacBooks 
using its default rendering options although the rendering process may take a while. 

Change ParaView rendering options from Preferences via RenderView tab.
Lower the values for LOD and Outline Threshold if needed. These two options decrease 
memory requirments at the expense of rendering quality. 

If ParaView crashes during a rendering process it can indicate failed memory request 
when computing the graphics pipeline. However, before that happens modern macOS versions 
can usually intervene and dispay so called Force-Quit window presenting processes
having caused problems.

## Software documentation
LignumVTK c++ source files are documented for Doxygen:

	doxygen Doxyfile 2>error.txt
	
Open the HTML index file, for example on macOS Terminal type:

	open DoxygenDoc/html/index.html

## Notes
> [!NOTE] 
> The creation of the VTK/VTPC tree files can take a while, possibly several minutes 
> depending the number of trees and their dataset sizes. 

> [!NOTE]
> As the total size of VTK data files can quickly add up pay attention to available disk space.
> It may be useful to save VTK files under separate directory structure for easy removals. 

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

[^vtpc]: VTK Partitioned Data Set Collection file format metadata header file.

[^vts]: VTK Structured Grid file format.

[^lod]: Level of Detail (LOD) refers to the dynamic complexity of a 3D model representation. 

[^ot]: The Outline Threshold value determines if an object is approximated as a bounding box only.
