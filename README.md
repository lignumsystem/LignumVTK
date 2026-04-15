# LignumVTK
Create VTK/VTPC and VTK/VTS files for ParaView visualization. ParaView is a 3D builder, post-processing 
and visualization engine for scientific data.

The design principle is that the binaries `lignumvtk` and `vsvtk` create VTK geometric objects and their 
attributes  representing Lignum tree models and voxel spaces for the final visualization 
to be done with ParaView graphics pipelines. This post-processing should be feasible to accomplish 
without programming knowledge about VTK library. 

> [!NOTE]
> LignumVTK is work under progress. lignumvtk supports kite shaped leaves. Ellipse and triangle leaves 
> will be implemented when needed.

## Building LignumVTK
### Prerequisites 
Download *LignumVTK* and *lignum-core* from GitHub. Install the VTK library package.
Using MacPorts type:

	sudo port install vtk
	
See also generic comments in *lignum-core*.

Download [ParaView](https://www.paraview.org) from their site.
MacPorts version reports poor port health, i.e. it is likely 
not in working order.

### Compilation
Use CMake to compile `lignumvtk`:
	
	mkdir release
	cd release
	cmake .. -DCMAKE_BUILD_TYPE=Release
	make install

The `lignumvtk` and `vsvtk` binaries are installed in the LignumVTK directory. 
See the CMakeLists.txt file for details.

## VTK Examples
[VTK Examples](https://examples.vtk.org/site/) has been the pivotal source of information in LignumVTK.

## lignumvtk
The `lignumvtk` program can produce VTK/VTPC[^vtpc] files from Lignum XML and HDF5 files. Note that `lignumvtk` can implicitely 
recognize between conifers and hardwood trees. After simulations such as in LignumForest the results can be found in two HDF5 files,
one for aggregated data and one for the trees in XML format. The input data for `lignumvtk` is either an XML tree file or 
an HDF5 file containing the tree datasets in XML format.

### Command line 
The `lignumvtk` command line is:

	./lignumvtk -input|-i path/to/file.[h5|xml] [-list] [-output|-o path/to/file.vtpc] [-year <number>] \
	[-dataset <string>] [-substring <string>] [-spline <number>] [-sides] -number
	
### Examples 
Example 1: List datasets for trees in Lignum HDF5 file:

	./lignumvtk -input File.h5 -list #All trees
	./lignumvtk -input File.h5 -list -year 80 #Trees for year 80
	
Example 2: Create VTK/VTPC file from Lignum XML file:
	
	./lignumvtk -input File.xml -output VTKFile.vtpc

Example 3: Create VTK/VTPC file for all trees of age 20:

	./lignumvtk -input File.h5 -output VTKFile.vtpc -year 20
	
Example 4: Create VTK/VTPC file for the dataset Tree_8 in /TreeXML/60/Tree_8:

	./lignumvtk -input File.h5 -output VTKFile.vtpc -dataset /TreeXML/60/Tree_8
	
Example 5: Create VTK/VTPC file for Tree_8 for all growth years saved in HDF5 file:

	./lignumvtk -input File.h5 -output VTKFile.vtpc -dataset Tree_8
	
Example 6: Create VTK/VTPC file for trees for all years matching the string Tree_11:

	./lignumvtk -input File.h5 -output VTKFile.vtpc -substring Tree_11
	
Example 7: Adjust the spline accuracy to 10, i.e. line segments denoting tree segments 
are divided into 10 spline segments. Adjust side accuracy to 20, i.e. the VTK tube will
have 20 rectangular sides to represent tree segment cylinders:

	./lignumvtk -input File.h5 -output VTKFile.vtpc -substring Tree_11 -spline 10 -sides 20

In general, the argument string for *-dataset* finds exact match for the dataset name, 
the option *-substring* uses the argument string to search the complete dataset path
in the HDF5 input file. The options *-year*, *-dataset* and *-substring* are mutually exclusive.
	
`lignumvtk`, i.e. the VTK toolkit library, will create additional subdirectory named after the *.vtpc*
output file to save the actual 3D geometry models in a set of reasonably sized files. 
Upload the main *.vtpc* output file in ParaView and finish by post-editing trees manually 
in ParaView's graphics pipelines for final visualization.

> [!WARNING]
> The default values for the `-spline` option 20 and for the `-sides` option 30 respectively
> should be applicable for a single Lignum tree but for a forest stand it may be necessary 
> reduce these numbers in compliance with the available memory and to shorten the rendering time.

> [!CAUTION]
> Unduly generic dataset path argument to the -substring option can retrieve significant number of tree datasets,
> possibly all of them.

> [!NOTE]
> During reconstruction of trees warning messages related to missing function files can appear. This is due to 
> hard coded default file names for tree simulations and does not prevent the creation of VTK/VTPC files.
> If disturbing create dummy files in the working directory to remove the warning messages.


## vsvtk
The `vsvtk` program can produce VTK/VTS[^vts] file from HDF5 voxel space data files.
More precisely voxel space data, as a 4D matrix in an HDF5 file, is rebuilt, converted
to a VTK structured grid with hexahedra voxel (grid) elements and saved as VTK/VTS file.

### Command line
The `vsvtk` command line is:

	./vsvtk -input|-i file.h5 [-list] [-output|-o file.vts] [-dataset <path_string>] [-side <integer>]
	
### Examples
Example 1: List voxel space datasets:

	./vsvtk -i File.h5 -list
	
Example 2: Read voxel space dataset and create VTK/VTS file:
	
	./vsvtk -i File.h5 -dataset /VoxelSpace/VoxelSpaceData60 -o VSFile.vts

The `vsvtk` program can create one voxel space VTK/VTS file at a time. 

## ParaView settings
ParaView can visualize single trees produced by `lignumvtk` with default rendering values 
in MacBooks although the rendering process may take a while. But for example for forest stands
(see LignumForest) it is mandatory to lower the values for LOD[^lod] and Outline Threshold[^ot].

Adjust ParaView rendering options from Preferences via RenderView tab.
Lower the values (for example by factor of 10) for LOD and Outline Threshold if needed.
These two options decrease memory requirments at the expense of rendering quality. 

If ParaView crashes during a rendering process it may indicate failed memory request 
when computing the graphics pipeline. 

## Software documentation
LignumVTK files are commented for Doxygen, the Terminal command line example for `zsh` and `bash`:

	doxygen Doxyfile 2>error.txt
	
Open the HTML index file, for example on macOS Terminal type:

	open DoxygenDoc/html/index.html

## Notes
> [!NOTE] 
> The creation of the VTK/VTPC files can take a while, possibly several minutes 
> depending the number of trees and their dataset sizes. 

> [!TIP]
> It may be useful to save VTK 3D geometry data models under separate directories. 

## Blender
Another 3D computer graphics creation suite is [Blender](https://www.blender.org). ParaView can export its 
3D geometry models in file formats supported by Blender. Use *.obj*, *.ply* or *.x3d* formats or employ 
add-ons like SciBlend for Blender. 

### lignumvtk.py
Initial trial to use vtk library to visualize tree roots with Lignum and ParaView. The tree roots are produced in 
the project FineRoots. The `lignumvtk.py` program creates VTK/VTM file that can be imported to ParaView.
lignumvtk.py is a python3 program that requires numpy and vtk python packages. Install these with `pip`.

[^vtpc]: VTK Partitioned Data Set Collection file format metadata header file.

[^vts]: VTK Structured Grid file format.

[^lod]: Level of Detail (LOD) refers to the dynamic complexity of a 3D model representation. 

[^ot]: The Outline Threshold value determines if an object is approximated as a bounding box only.





