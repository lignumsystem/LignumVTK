# LignumVTK
`lignumvtk`, `vsvtk` and `cievtk` programs create VTK files for [ParaView](https://www.paraview.org/). 
ParaView is a 2D and 3D computer geometry model builder, data analysis and  visualization software 
for scientific data. The VTK library is the visualization toolkit ParaView relies on.

`lignumvtk` and `vsvtk` map Lignum tree models and voxel spaces to VTK's geometric objects 
and their attributes for the final visualization with ParaView graphics pipelines. This post-processing 
requires neither programming skills nor knowledge of VTK library. Primary purpose of the visualization
is to extract qualitative insight from simulation results.

`cievtk` generates CIE standard overcast skies, mapping radiance values to a VTK hemisphere geometry
for visualization.

To visualize and produce journal figures for tree models and voxel spaces, read the
[Lignum visualization](LIGNUM_VISUALIZATION.md) and [ImageMagick](IMAGE_MAGICK.md) contributing guides.

## Building LignumVTK
### Prerequisites 
Download *lignum-core* and *LignumVTK* from GitHub. Install the dependencies specified in the 
lignum-core [README](https://github.com/lignumsystem/lignum-core/blob/master/README.md).

### CMake
Use CMake to compile `lignumvtk` and `vsvtk`:
	
	mkdir release #In LignumVTK directory
	cd release
	cmake .. -DCMAKE_BUILD_TYPE=Release
	make install

The `lignumvtk`, `vsvtk` and `cievtk` binaries are installed in the LignumVTK directory. 
See the CMakeLists.txt file for details.

## Trees and forest stands
The `lignumvtk` program processes either XML tree files or HDF5 files containing 
the XML formatted tree datasets to produce VTK/VTPC[^vtpc] output. 
It can automatically distinguish between conifer and hardwood tree types.

Trees are reconstructed and converted to VTK spline tube segments for wooden parts 
and conifer foliage, and VTK triangle strips for kite shaped hardwood leaves. 
Visualization properties are defined using spline tube and triangle strip attributes 
from LIGNUM trees.

The main VTK/VTPC file (.vtpc) is a metadata file containing the names and locations
of the 3D geometry models. Saving this file automatically creates a matching directory
to store those models.

### Command line 
The `lignumvtk` command-line is:

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
	
**Example 4**: Create VTK/VTPC file for the dataset /TreeXML/60/Tree_8:

	./lignumvtk -input File.h5 -output File.vtpc -dataset /TreeXML/60/Tree_8
	
**Example 5**: Create VTK/VTPC file for Tree_8 for all growth years in HDF5 file:

	./lignumvtk -input File.h5 -output File.vtpc -dataset Tree_8
	
**Example 6**: Create VTK/VTPC file for trees for all years matching the string Tree_11:

	./lignumvtk -input File.h5 -output File.vtpc -substring Tree_11
	
**Example 7**: Divide each tree segment into 10 spline segments. Assign 20 rectangular sides 
to spline tubes approximating tube cylinders.

	./lignumvtk -input File.h5 -output File.vtpc -substring Tree_11 -spline 10 -sides 20

**Example 8**: Create component view:

		./lignumvtk -input File.h5 -output File.vtpc -dataset Tree_8 -view 1
		
In summary, the option *-dataset* uses exact match to find the dataset, the option *-substring* searches 
dataset paths with substring matches and the option *-year* based on simulation year.
The options *-year*, *-dataset* and *-substring* are mutually exclusive.
The options *-spline* and *-sides* set the spline tube segment length and roundness.

The option *-view* assembles VTK datasets for ParaView. The default is tree view (*-view 0*), which organizes 
spline segments and foliage by individual trees. Alternatively, component view (*-view 1*) groups geometry 
by type: foliage, segments, and heartwood. The former enables detailed inspection of specific trees,
whereas the latter focuses on forest stand visualization. 

## Voxel spaces
The `vsvtk` program processes HDF5 voxel space datasets to produce VTK/VTS[^vts] output.
It reconstructs voxel space and voxel data 4D matrix representation, converting it
to a VTK structured grid where hexahedral elements are mapped to their corresponding
voxel attributes. Default hexahedron edge size is the voxel edge size used in a simulation.

### Command line
The `vsvtk` command-line is:

	./vsvtk [-help|-h] -input|-i File.h5 [-list] [-output|-o File.vts] [-dataset <path_string>] [-edge <number>]
	
### Examples
**Example 1**: List voxel space datasets:

	./vsvtk -i File.h5 -list
	
**Example 2**: Read voxel space dataset from an HDF5 file and create VTK/VTS file. 
Set hexahedron (voxel) edge size to 3:
	
	./vsvtk -i File.h5 -dataset /VoxelSpaceData60 -o File.vts -edge 3

The `vsvtk` program converts single voxel space datasets into VTK/VTS format. 
Therefore, the argument string for `-dataset` requires a full path name.

## CIE sky
The `cievtk` tool generates the 15 standard CIE general skies and exports them as VTK/VTP files. 
Additionally, it supports the older CIE Standard Overcast Sky (Moon-Spencer), commonly designated
as Sky Type 16. The CIE Standard General Sky model itself defines relative radiance distributions
using five variable parameters. Two sets of parameters are available, Darula and Kittler 
2002 and CIE ISO 15469:2004 standard. For implementation steps, see *CIESky.h* and its accompanying
software documentation.

### Command line
The `cievtk` command-line is:

	./cievtk [-help|-h] -azimuth|-a <number> -inclination|-i <number> -totrad |-r <number> -cie|-c <1-15,16> \
	-params|-p <DK2002|ISO2004> -sunpolar|-sp <0-90 degrees> -sunazimuth|-sa <0-360 degrees> -output|-o <file.vtp> 
	
	-params: DK2002  Darula and Kittler 2002 parameter set
		     ISO2004 ISO 15469:2004 standard parameter set
			
### Example
Generate a CIE Sky Type 10 model using the CIE ISO 15469:2004 standard parameters, configured with 8 azimuths,
9 inclinations (including zenith), a peak radiant flux of 1200 at the zenith, a sun polar angle of 30°, and a sun
azimuth of 130°, then export the output to CIE10.vtp.

	./cievtk -a 8 -i 9 -r 1200 -cie 10  -params ISO2004 -sunpolar 30 -sunazimuth 130 -o CIE10.vtp
	
## Software documentation
Produce the Doxygen documentation:

	doxygen Doxyfile 2> error.txt
	
Open the HTML index file (macOS Terminal):

	open DoxygenDoc/html/index.html

## fineroot.py
`fineroot.py` served as an initial trial of the VTK library, mentioned here for its role
in visualizing tree fine roots. The tree fine roots for the LIGNUM model are produced in 
the project FineRoots. The `fineroot.py` program processes tree root XML files and creates VTK/VTP files
for ParaView. `fineroot.py` is a Python 3 program and requires *numpy* and *vtk* Python packages. 

## Notes
LignumVTK is a work in progress. `lignumvtk` supports kite-shaped leaves with plans to implement
elliptical and triangle shapes as needed.

The creation of the VTK/VTPC files can take several minutes depending on the number of trees
and their VTK dataset sizes. 

Using an overly broad path argument to the `-substring` option for `lignumvtk` can inadvertently
return all tree dataset paths. The default values for the options `-spline` and `-sides` should
be feasible for a single Lignum tree. With forest stands reduce the values to fit within
the available memory.

Warning messages regarding missing function files may appear during tree reconstruction. 
These functions were used in simulations and do not prevent the generation of VTK/VTPC files.

> [!CAUTION]
> VTK data files grow rapidly, so monitor your available disk space. It is a best practice to save
> these files in a dedicated directory structure to simplify deletion and management..

## VTK Examples
[VTK Examples](https://examples.vtk.org/site/) has been pivotal source of information for LignumVTK
implementation.

[^vtpc]: VTK Partitioned Data Set Collection file format metadata header file.

[^vts]: VTK Structured Grid file format.

