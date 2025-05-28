# LignumVTK
Create VTK/VTPC files for ParaView. ParaView is a 3D builder, post-processing and visualization engine for scientific data
with computer geometry models.

The design principle is that the LignumVTK binary `lignumvtk` creates VTK geometric objects and attributes for the Lignum tree models but the 
final visualization will be done in ParaView by implementing appropriate graphics pipelines. This post-processing should be 
feasible to accomplish without programming and knowledge about VTK library. 


## Prerequisites 
Download *LignumVTK* and *lignum-core* from GitHub. Install VTK package.
Using MacPorts type:

	sudo port install vtk
	
See also generic comments in *lignum-core*.

Download [ParaView](https://www.paraview.org) from their site.
MacPorts version reports poor port health, i.e. it is likely 
not in working order.

## Building LignumVTK
Use CMake to compile:
	
	mkdir release
	cd release
	cmake .. -DCMAKE_BUILD_TYPE=Release
	make install

See CMakeLists.txt for details.

## Usage
The `lignumvtk` program can produce VTK/VTPC files from Lignum XML and HDF5 files. Note that `lignumvtk` can recognize between 
conifers and hardwood trees. The command line is:

	./lignumvtk -input file.[h5|xml] [-list] [-output file.vtpc] [-year <number>] [-dataset <path_string>] [-substring <path_string>] [-spline <number>]
	
The first example creates VTK/VTPC file from Lignum XML file:
	
	./lignumvtk -input File.xml -output VTKFile.vtpc

The second example lists datasets for trees in Lignum HDF5 file:

	./lignumvtk -input File.h5 -list
	
The third example creates VTK/VTPC file for all trees in growth year 20:

	./lignumvtk -input File.h5 -output VTKFile.vtpc -year 20
	
The fourth example creates VTK/VTPC file for the dataset Tree_8 in /TreeXML/60/Tree_8:

	./lignumvtk -input File.h5 -output VTKFile.vtpc -dataset /TreeXML/60/Tree_8
	
The fifth example creates VTK/VTPC file for Tree_8 for all growth years saved in HDF5 file:

	./lignumvtk -input File.h5 -output VTKFile.vtpc -dataset Tree_8
	
The sixth example creates VTK/VTPC file for trees for all years matching Tree_11:

	./lignumvtk -input File.h5 -output VTKFile.vtpc -substring Tree_11
	
The final example adjust the spline accuracy to 10, i.e. a line segment denoting a tree segment is divided into 10 spline segments:

	./lignumvtk -input File.h5 -output VTKFile.vtpc -substring Tree_11 -spline 10

In general, the argument string for *-dataset* finds exact match for the dataset name, the option *-substring* uses the argument string 
aa a substring in all dataset paths in the HDF5 input file. The options *-year*, *-dataset* and *-substring* are mutually exclusive.
	
Upload the *.vtpc* output file to Paraview and finish by post-editing trees for final visualization.

> [!NOTE] 
> The creation of the VTK/VTPC files can take a while, possibly several minutes depending the number of trees
> and their sizes. 

> [!CAUTION]
> Overly generic dataset path argument to the -substring option can retrieve significant number of tree datasets,
> possibly all of them.

## ParaView settings
From  ParaView Preferences and further via RenderView tab it is possible to adjust rendering options.
Lower the values (for example by factor of 10) for LOD Threshold[^lod] and Outline Threshold if needed.
These two options decrease memory requirments at the expense of rendering quality. If ParaView crashes during rendering process 
it may indicate failed memory request when processing the graphics pipeline. 

It seems that it is possible to visualize single trees produced by `lignumvtk` in ParaView with 
default rendering values with consumer products like MacBooks (although the ParaView rendering may take a while). 
But for example for forest plots (see LignumForest) it is mandatory to lower the threshold values for LOD and Outline.

### Blender
Another 3D creation suite is [Blender](https://www.blender.org). ParaView can export computer graphics models
in file formats supported by Blender. 

## Software documentation
LignumVTK files are commented for Doxygen, the Terminal command line example for `zsh` and `bash`:

	doxygen Doxyfile 2>error.txt
	
Open the HTML index file, for example on macOS Terminal type:

	open DoxygenDoc/html/index.html
	
## Notes
LignumVTK is work under progress. Currently kite shaped leaves are supported for hardwoods. Ellipse and triangle
leaves will be implemented when needed.

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

[^lod]: In computer graphics Level of Detail (LOD) refers to the dynamic complexity of a 3D model representation. 
The LOD of an object is increased or decreased according to some metrics, for example observing the distance from the camera 
or viewer in a 3D scene or designating the importance of the object.



