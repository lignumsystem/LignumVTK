# LignumVTK
Create VTK/VTPC files for ParaView. ParaView is a 3D builder, post-processing and visualization engine for computer geometry models.

## Prerequisites 
Download *LignumVTK* and *lignum-core* from GitHub. Install VTK package.
With MacPorts type:

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
The `lignumvtk` program can produce VTK/VTPC files from Lignum XML and HDF5 files. The command line is:

	./lignumvtk -input file.[h5|xml] [-list] [-output file.vtpc] [-year <number>] [-dataset <path_string>]
	
The first example creates VTK/VTPC file from Lignumm XML file:
	
	./lignumvtk -input File.xml -output VTKFile.vtpc

The second example lists datasets for trees in Lignum HDF5 file:

	./lignumvtk -input File.h5 -list
	
The third example creates VTK/VTPC file for growth year 20:

	./lignumvtk -input File.h5 -output VTKFile.vtpc -year 20
	
The fourth example createss VTK/VTPC file for the dataset Tree_8 in /TreeXML/60/Tree_8:

	./lignumvtk -input File.h5 -output VTKFile.vtpc -dataset /TreeXML/60/Tree_8
	
The fifth example creates VTK/VTPC file for Tree_8 for all growth years saved in HDF5 file:

	./lignumvtk -input File.h5 -output VTKFile.vtpc -dataset Tree_8
	
In general the argument string for the option *-dataset* is matched as a substring to all
dataset paths in the HDF5 input file. 
	
Upload the *.vtpc* output file to Paraview and finish by postediting trees for final visualization.

> [!NOTE] 
> The creation of the VTK/VTPC files can take a while, possibly several minutes depending the number of trees
> and their sizes. 

> [!CAUTION]
> Overly generic dataset path argument to the -dataset option can retrieve significant number of tree datasets,
> possibly all of them.

## ParaView
From  ParaView Preferences and further via RenderView tab it is possible to adjust rendering options.
Lower the values (for example by factor of 10) for LOD Threshold and Outline Threshold if needed.
This decreases memory requirments at the expense of rendering quality. If ParaView crashes during rendering process 
it may indicate failed memory request when processing the graphics pipeline. 

It seems that it is possible to visualize single trees produced by `lignumvtk` in ParaView with 
default rendering values with consumer products like MacBooks (although the ParaView rendering may take a while). 
But for example for forest plots (see LignumForest) it is mandatory to lower the threshold values for LOD and Outline.

### Blender
Another 3D creation suite is [Blender](https://www.blender.org). ParaView can export graphics models
in file formats supported by Blender. 

## Software documentation
LignumVTK files are commented for Doxygen, the Terminal command line example for `zsh` and `bash`:

	doxygen Doxyfile 2>error.txt
	
Open the HTML index file, for example on macOS Terminal type:

	open DoxygenDoc/html/index.html
	
## Notes
LignumVTK is work under progress.

### lignumvtk.py: 

Initial proposition to use vtk library
to visualize tree roots with Lignum and ParaView. The `lignumvtk.py`
program creates *vtp* xml file that can be imported
to ParaView.

lignumvtk.py is a python3 program that requires numpy 
and vtk python packages. Install these with `pip`.

#### Usage:

	python lignumvtk.py -h
	Usage: lignumvtk.py [options]
	Options:

	-h, --help        show this help message and exit
	-i F1, --fxml=F1  Read Lignum xml file
	-o F2, --fvtp=F2  VTP output file
	-c, --cylinder    Use segment base radius as segment top radius (pure
                      cylinder)

   



