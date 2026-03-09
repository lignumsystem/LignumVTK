# LignumVTK
Create VTK/VTPC files for ParaView. ParaView is a 3D builder, post-processing and visualization engine for scientific data
with computer geometry models.

The design principle is that the binary `lignumvtk` creates VTK geometric objects and attributes 
for the Lignum tree models but the final visualization will be done in ParaView by implementing 
appropriate graphics pipelines. This post-processing should be feasible to accomplish 
without programming and knowledge about VTK library. 

> [!NOTE]
> LignumVTK is work under progress. Currently kite shaped leaves are supported for hardwoods. 
> Ellipse and triangle leaves will be implemented when needed.

## VTK Examples
[VTK Examples](https://examples.vtk.org/site/) were the pivotal source of information in the `lignumvtk` implementation. 

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

The `lignumvtk` binary should be available in the LignumVTK directory. See CMakeLists.txt for details.

## Usage
The `lignumvtk` program can produce VTK/VTPC[^vtpc] files from Lignum XML and HDF5 files. Note that `lignumvtk` can implicitely 
recognize between conifers and hardwood trees. After simulations such as in LignumForest the results can be found in two HDF5 files,
one for aggregated data and one for the trees in XML format. The input data for `lignumvtk` is either an XML tree file or 
an HDF5 file containing the tree datasets in XML format.

### Command line 
The `lignumvtk` command line is:

	./lignumvtk -input|-i path/to/file.[h5|xml] [-list] [-output|-o path/to/file.vtpc] [-year <number>] [-dataset <string>] [-substring <string>] [-spline <number>]
	
### Examples 
Example 1: List datasets for trees in Lignum HDF5 file:

	./lignumvtk -input File.h5 -list
	
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
	
Example 7: Adjust the spline accuracy to 10. Line segments denoting tree segments 
are divided into 10 spline segments:

	./lignumvtk -input File.h5 -output VTKFile.vtpc -substring Tree_11 -spline 10

In general, the argument string for *-dataset* finds exact match for the dataset name, 
the option *-substring* uses the argument string to search the complete dataset path
in the HDF5 input file. The options *-year*, *-dataset* and *-substring* are mutually exclusive.
	
`lignumvtk`, i.e. the VTK toolkit library, will create additional subdirectory named after the *.vtpc*
output file to save the actual 3D geometry models in a set of reasonably sized files. 
Upload the main *.vtpc* output file to ParaView and finish by post-editing trees manually 
in ParaView graphics pipelines for final visualization.

> [!NOTE]
> During reconstruction of trees warning messages regarding missing function files can appear. This is due to 
> hard coded default file names for tree simulations and does not prevent the creation of VTK/VTPC files.

> [!NOTE] 
> The creation of the VTK/VTPC files can take a while, possibly several minutes 
> depending the number of trees and their dataset sizes. 

> [!CAUTION]
> Unduly generic dataset path argument to the -substring option can retrieve significant number of tree datasets,
> possibly all of them.

> [!TIP]
> It may be preferable to save VTK 3D data models under one directory. This directory can be excluded for example
> from Doxygen search paths.

## ParaView settings
ParaView can visualize single trees produced by `lignumvtk` with default rendering values 
in MacBooks although the rendering process may take a while. But for example for forest stands
(see LignumForest) it is mandatory to lower the values for LOD[^lod] and Outline Threshold[^ot].

Adjust ParaView rendering options from Preferences via RenderView tab.
Lower the values (for example by factor of 10) for LOD and Outline Threshold if needed.
These two options decrease memory requirments at the expense of rendering quality. 

If ParaView crashes during a rendering process it may indicate failed memory request 
when computing the graphics pipeline. 

## Blender
Another 3D computer graphics creation suite is [Blender](https://www.blender.org). ParaView can export its 
3D geometry models in file formats supported by Blender. 

## Software documentation
LignumVTK files are commented for Doxygen, the Terminal command line example for `zsh` and `bash`:

	doxygen Doxyfile 2>error.txt
	
Open the HTML index file, for example on macOS Terminal type:

	open DoxygenDoc/html/index.html

### lignumvtk.py
Initial trial to use vtk library to visualize tree roots with Lignum and ParaView. The tree roots are produced in 
the project FineRoots. The `lignumvtk.py` program creates VTK/VTM file that can be imported to ParaView.

lignumvtk.py is a python3 program that requires numpy and vtk python packages. Install these with `pip`.

#### Usage

	python lignumvtk.py -h
	Usage: lignumvtk.py [options]
	Options:

	-h, --help        show this help message and exit
	-i F1, --fxml=F1  Read Lignum xml file
	-o F2, --fvtp=F2  VTP output file
	-c, --cylinder    Use segment base radius as segment top radius (pure
                      cylinder)
[^vtpc]: VTK Partitioned Data Set Collection format metadata header file.

[^lod]: Level of Detail (LOD) refers to the dynamic complexity of a 3D model representation. 
The increasing LOD threshold value tend to render objects in full and lower value will give better interactivity. 

[^ot]: The Outline Threshold value determines if an object is approximated as a bounding box only.




