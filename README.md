# LignumVTK
Create VTK/VTPC files for ParaView.

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
In the command line type:
	
	./lignumvtk lignumtree.xml outputfile.vtpc 
	
Upload *outputfile.vtpc* to Paraview and finish the editing for final visualization.

## Software documentation
LignumVTK files are commented for Doxygen:

	doxygen Doxyfile
	
Open the HTML index file, for example on macOS Terminal type:

	open DoxygenDoc/html/index.html
	

## Notes
This is work under progress. Currently broadleaved Lignum trees with kite shaped leaves
can be tentatively operated.

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

   



