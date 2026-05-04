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
Download *lignum-core* and *LignumVTK* from GitHub. Software installed as in 
README for lignum-core.

### CMake
Use CMake to compile `lignumvtk` and `vsvtk`:
	
	mkdir release #In LignumVTK directory
	cd release
	cmake .. -DCMAKE_BUILD_TYPE=Release
	make install

The `lignumvtk` and `vsvtk` binaries are installed in the LignumVTK directory. 
See the CMakeLists.txt file for details.

## Trees and forest stands
The `lignumvtk` program processes either XML tree files or HDF5 files containing 
the XML formatted tree datasets to produce VTK/VTPC[^vtpc] output. 
It can automatically distinguish between conifer and hardwood tree types.

Trees are reconstructed and converted to VTK spline tube segments for wooden parts 
and conifer foliage, and VTK triangle strips for kite shaped hardwood leaves. 
Visualization properties are defined using spline tube and triangle strip attributes 
from Lignum trees.

The main VTK/VTPC output file (.vtpc extension) is a metadata file that specifies names and 
locations of the 3D geometry model files. These files are stored in a directory automatically 
created and named after the main output file upon saving. To visualize them, open
the main file in ParaView and set up the graphics pipeline.

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

> [!CAUTION]
> Using an overly broad path argument to the `-substring` option can inadvertently return
> all dataset paths.

> [!IMPORTANT]
> The default values for the options `-spline` and `-sides` should be feasible
> for a single Lignum tree. With forest stands reduce the values to fit 
> within the available memory.

> [!NOTE]
> Warning messages regarding missing function files may appear during tree reconstruction. 
> These functions were used in simulations and do not prevent the generation of VTK/VTPC files.

## Voxel spaces
The `vsvtk` program processes HDF5 voxel space datasets to produce VTK/VTS[^vts] output.
It reconstructs voxel space and voxel data 4D matrix representation, converting it
to a VTK structured grid where hexahedral elements are mapped to their corresponding
voxel attributes. Default hexahedron edge size is the voxel edge size used in a simulation.

### Command line
The `vsvtk` command line is:

	./vsvtk [-help|-h] -input|-i File.h5 [-list] [-output|-o File.vts] [-dataset <path_string>] [-edge <number>]
	
### Examples
**Example 1**: List voxel space datasets:

	./vsvtk -i File.h5 -list
	
**Example 2**: Read voxel space dataset from an HDF5 file and create VTK/VTS file. 
Set hexahedron (voxel) edge size to 3:
	
	./vsvtk -i File.h5 -dataset /VoxelSpaceData60 -o File.vts -edge 3

The `vsvtk` program converts one voxel space dataset at a time into VTK/VTS format. 
Open the resulting file in ParaView to configure the graphics pipeline.

## ParaView settings
The 3D computer geometry models may require a fair amount of memory. ParaView can 
visualize `lignumvtk`-produced trees and forest stands with default settings, though 
rendering may take some time.

Change ParaView rendering options from Preferences via RenderView tab. If needed, lower 
the LOD[^lod] and Outline Threshold[^ot] values before opening a VTK file to reduce
memory usage at the cost of rendering quality.

If ParaView exits unexpectedly during rendering, it is often due to a failed memory request. 
However, modern macOS versions may intervene first, displaying a Force-Quit window that 
identifies the problematic processes.

## Software documentation
Produce the Doxygen documentation:

	doxygen Doxyfile 2> error.txt
	
Open the HTML index file (macOS Terminal):

	open DoxygenDoc/html/index.html

## Blender
Another 3D computer graphics creation suite is [Blender](https://www.blender.org).
File formats supported by both ParaView and Blender are Alembic (*.abc*), Wavefront OBJ (*.obj*),
Stanford PLY (*.ply*) and STL (*.stl*) each with their pros and cons. Add-ons like SciBlend 
for Blender are also available for file transfer. 

## lignumvtk.py
`lignumvtk.py` served as an initial trial of the *vtk* library, documented here for its role
in visualizing tree roots. The tree roots for the LIGNUM model are produced in 
the project FineRoots. The `lignumvtk.py` program processes tree root XML files and creates VTK/VTM files
for ParaView. `lignumvtk.py` is a Python 3 program and requires *numpy* and *vtk* Python packages. 

## Notes
> [!CAUTION]
> The total size of VTK data files can quickly add up. Pay attention to available disk space.
> It may be useful to save VTK files under separate directory structure for straightforward removals. 

> [!NOTE] 
> The creation of the VTK/VTPC files, as well as ParaView rendering, can take
> several minutes depending on the number of trees and their VTK dataset sizes. 

> [!NOTE]
> LignumVTK is a work in progress. `lignumvtk` supports kite-shaped leaves with plans
> to implement elliptical and triangle shapes as needed.

## VTK Examples
[VTK Examples](https://examples.vtk.org/site/) has been pivotal source of information for LignumVTK
implementation.

[^vtpc]: VTK Partitioned Data Set Collection file format metadata header file.

[^vts]: VTK Structured Grid file format.

[^lod]: [Level of Detail](https://en.wikipedia.org/wiki/Level_of_detail_(computer_graphics): dynamic 3D model representation. 

[^ot]: Outline Threshold: object is approximated with bounding boxes only.
