# LIGNUM visualization
While [ParaView](https://www.paraview.org/) and [Blender](https://www.blender.org) are
both open-source, they serve different purposes. ParaView specializes in scientific data
analysis and visualization, whereas Blender is a 3D creation suite designed for animation,
visual effects, and photorealistic rendering.

The two applications are frequently paired together. First, Paraview is used for 
qualitative and quantitative data analysis. Then, Blender builds the final presentation
visuals using its physically based rendering engines to introduce informative contextual
objects like environmental lighting, shadows, and background elements.

Both ParaView and Blender support common 3D file formats. This guide uses
glTF for scene exchange between the two applications.

> [!IMPORTANT]
> The ParaView download page does not auto-detect macOS Silicon. Make the correct manual selection.

## ParaView settings
The 3D computer geometry models may require a fair amount of memory. ParaView can 
visualize `lignumvtk`-produced trees and forest stands with default settings, though 
rendering may take some time.

Change ParaView rendering options from Preferences via RenderView tab. If needed, lower 
the LOD[^lod] and Outline Threshold[^ot] values before opening a VTK file to reduce
memory usage at the cost of rendering quality.

If ParaView exits unexpectedly during rendering, it is often due to a failed memory request. 
However, modern macOS versions may intervene first, displaying the Force-Quit window that 
identifies the problematic processes.

## ParaView user interface overview
ParaView enables users to build data-flow visualization *pipelines* for data import,
make structural changes in datasets, adjust model parameters, and visually explore 3D datasets.
The main workspace organizes these tasks around interconnected *panels* and *views*.

### The core user interface components 

+ **Menu bar and toolbars:** Positioned at the top, these provide quick access
  to common tasks like *File* operations, applying *Filters*, configuring
  *Views*[^panel] and selecting visual *Representation*.

+ **Pipeline browser:** Located left below the toolbars, this panel manages
  the visualization pipeline by displaying open files, active datasets, and applied filters.
  Toggle the eye icon to control dataset visibility and selection during rendering.
    
+ **Properties panel:** Located below the pipeline browser to display and
   adjust settings for active pipeline items. The highlighted *Apply* button 
   indicates you can commit the pending changes and render active datasets.

+ **Information panel:** Paired with Properties panel, it displays for example basic
  statistics, data ranges, and data type for the active pipeline item.
  
+ **Viewport:** The central portion of the ParaView application window and renders 
  the pipeline. The default *Render view* is only one of many ways or *views*
  to visualize pipeline data.
  
+ **Colormap editor:** Usually present by default, this panel is also available
  from the *View* menu.  Located to the right of the Viewport, it allows you
  to select and edit the colormap for the pipeline data.

The highly dynamic user interface automatically adapts to the active pipeline objects. 
For example, the *Filter* menu highlights applicable options while dimming the rest, 
and the Properties panel updates to display only the attributes relevant to the selected 
pipeline object.

### The core filters for LIGNUM

+ **Extract Block:** Selects specific sections (trees, foliage, sapwood, heartwood) 
  from the imported VTK/VTPC tree file (Partitioned Dataset Collection) to
  a separate pipeline entry.
  
+ **Threshold:** Selects portions of an input dataset whose scalar values
  (e.g. foliage mass) fall within a specific, user-defined range.

+ **Merge Block:** Combines distinct datasets in a Partitioned Dataset Collection 
  into a single unified dataset.
  
+ **Resample To Image:** Samples data from a 3D dataset (unstructured grid) 
  onto a uniform, structured evenly spaced 3D voxel grid. This allows highly optimized 
  *volume rendering* to see inside 3D fields, like foliage distribution.
    
## Blender user interface overview
Blender organizes user interface into a modular grid of rectangular windows called *Areas*,
which can be resized, split, or combined to fit the workflow.

### The core user interface components

+ **Top Bar and Menus:** 
  + **Main Menu:** Located in the top-left corner, this menu bar provides standard application
    functions from *File* to *Help*. Notably, the *Render* menu starts the final model rendering. 
  + **Workspaces:** Moving to the right from the main menu, the tabs from *Layout* to *Scripting* adjust
    the Blender's workspace for specific tasks.

+ **3D Viewport (Center):**
  + **Viewport Header** The *View:Align View:Align Active Camera to View* menu option moves and orients
    the active rendering camera to match the current 3D viewport perspective. 
    The *Add* menu inserts new objects into the model — specifically cameras and lights — while the separate
	control tabs adjust the viewport rendering modes.
  + **The Stage:** The large main window where you create, interact and delete your
    3D objects, lights, and cameras.
  + **Toolbar (Left):** Contains object transformation tools.
  + **Sidebar (Right):** By default hidden behind a small arrow, toggle with lower case *n*. 
    Presents tabs for selected *Items*, *Tools* for interactions and *View* settings.

+ **Outliner (Top right):**
  + **Scene Hierachy:** Contains every object in the project.
  + **Visibility Toggles**: Eye and camera symbols control object visibility in the viewport
    and final render, respectively.
	
+ **Properties Editor (Below Outliner):**
  + **Mission Control (Left):** Long narrow vertical tabbed system to select Key Tab settings.
  + **Key Tabs (Right):** Control settings, content depends on the selection in the Mission Control.
  
+ **Animation Hub (Bottom):** Displays controls to playback animations.

+ **Status bar (Bottom):** Located outside the user's primary view and easily overlooked. 
  It also displays error messages.

Blender for macOS bypasses Apple's Human Interface Guidelines, implementing Windows-like user
interface controls rather than adopting standard macOS toolbar architectures.

### Blender renderers
Blender includes three native render engines built directly into the software,
accessible via the *Mission Control:Render* tab. Each renderer is optimized for 
different purposes: Workbench for layout and previews, EEVEE for real-time speed,
and Cycles for photorealism.

+ **Cycles:** True physically based renderer. Traces millions of simulated light photons
as they bounce across a scene, calculating true real-world physics for lighting, reflections,
and refractions.

+ **EEVEE:** Rasterization renderer. Uses modelling shortcuts to imitate complex lighting
  effects in real-time.

+ **Workbench:** Designed for maximum performance during modeling and scene layout.
   Does not calculate realistic lighting or complex texturing.
   
### Blender sky models
Blender features four built-in analytical sky models, accessible via the *Mission Contorl:World* tab.
In the Surface section, click the *yellow dot* next to Color and select Sky Texture.

+ **Nishita multiple scattering:** The most physically accurate model. It accounts
  for light bouncing multiple times through the atmosphere creating realistic outdoor
  illumination for different times of the day. Contains built-in sun model.

+ **Nishita single scattering:** Simplified Nishita modelling as it calculates single light bounce. 

+ **Hosek/Wilkie:** Provides realistic ambient daylight lighting. Requires explicit
  sun object as the light source for shadows.
  
+ **Preetham:** The oldest model from 1999. Works well for basic daylight.

### Blender camera
Blender features a mathematical digital SLR camera model, allowing users
to control lens type, focal length, aperture, and depth of field. It also
replicates physical camera sensors, which enables precise *Exposure* adjustments
by matching lighting to realistic camera ISO values.

## Graphics Library Transmission Format
Graphics Library Transmission Format (glTF) is an open standard designed for
the efficient transmission, loading, and rendering of 3D models and scenes across
various authoring applications. It is optimized for compact file sizes and seamless
compatibility with all modern graphics APIs. The conventional file extension for
this format is *.gltf*.

## Paraview visualization for LIGNUM trees
Follow these steps to visualize single trees or forest stands:

+ Open the `lignumvtk` generated VTK/VTPC file.
+ Adjust tree orientation for the camera view.
+ Apply the Extract Block filter to generate separate datasets for the stem, heartwood and foliage.
+ Apply Merge Block filters to datasets to generate unstructured grids.
+ Apply the Threshold filter to foliage to isolate positive values (exclude zeros).
+ Assign appropriate colors or colormaps to stem, heartwood and foliage.
+ File operations:
  + *Save State* of the project.
  + Select datasets (foliage, stem, heartwood) and apply *Export Scene* for glTF export. 

You may have Outline as default Representation. Select Surface for better rendering quality. 

ParaView renders automatically when you modify the colormap or Representation.
To change this, deselect *Render views automatically* button at the bottom
of the colormap editor. You can then trigger rendering manually using the adjacent
*Render Views* button.

An efficient visualization pipeline minimizes 3D data payload to maximize rendering performance. 
For example, if internal heartwood structures are not of interest and are occluded without 
transparancy settings during a visual inspection, you should strip this data entirely before
rendering or exporting the glTF file.

Be careful in selecting datasets to be exported. Only the selected datasets will be saved
to a glTF file.

## Blender visualization for LIGNUM trees
Follow these steps to visualize single trees or forest stands:

+ Import the glTF file exported from ParaView.
+ Use the Workbench renderer.
+ Orientate the trees in the 3D Viewport (*Mission Control:Object*).
+ Align the active camera to the view:
  + Yellow frame denotes the rendering area.
  + Optionally change the camera to vertical position by flipping Resolution (*Mission Control:Output*).
+ Change to the Cycles renderer.
+ Setup Sky Texture as the light source.
+ Render the trees.
+ Save the project in Blender format (*.blend*).
  
Blender's separate render window may not display progress for the first few minutes.
Forest stand size and rendering quality dictate the complete process time, which can
take up to eight hours. View the progess bar for estimated times.

Mathematically, both ParaView and Blender use a right-handed, Z-up coordinate system. 
However, importing a glTF file into Blender rotates models by 90 degrees. This happens
because the glTF format defaults - like many 3D file formats - to a right-handed, Y-up system.
To fix this, simply rotate the trees by -90 degrees. While Blender defaults to quaternions,
switching to XYZ Euler rotation is often more intuitive.

Minimal rendering in Blender requires three core components: a 3D model, a light source, and a camera
pointed at the model. Note that shadows require a surface to catch them. For example, add a simple
plane to represent the forest floor (via the *Add:Mesh:Plane* menu sequence).

Instead of moving a single camera for different views, position multiple cameras throughout the scene.

When using the Nishita sky model, the camera must point strictly above the horizon. 
Otherwise, Cycles fails to calculate the natural blue daylight correctly. For overhead views
of a forest stand — where the camera is positioned above the canopy looking down — the Hosek/Wilkie
model yields better results. The Nishita sky often overexposes default Blender settings.
Lower the value for sun strength, or search online for optimal configurations. 

## ParaView visualization for voxel space
Follow these steps to visualize a voxel space:

+ Open the `vsvtk`-generated VTK/VTS voxel space file.
+ Adjust the voxel space orientation.
+ Select foliage data (LGAWf) using the Properties panel (deselecting others).
+ Apply Threshold filter to foliage to isolate positive values (exclude zeros)
+ Assign appropriate colormap to foliage.
+ Apply Resample To Image filter to create evenly spaced voxel grid.
  + Properties panel: increase Sampling Dimensions for example to 300 for each X, Y and Z dimensions.
+ File operations:
  + *Save State* of the project.
  + Select the (last) resampled dataset in the pipeline and apply *Export Scene* for glTF export.

Select Surface as Representation for better rendering quality. Increased Sampling Dimensions
has better compatibility with Blender.

## Blender visualization for voxel space
Follow these steps to visualize a voxel space:

+ Import the glTF file exported from ParaView.
+ Adjust the voxel space orientation.
+ Convert the voxel space mesh into Blender voxels:
  + Select the voxel space.
  + Select the *Mission Control:Modifiers* tab:
	+ From Add Modifier select Remesh.
	+ Reduce voxel size (Voxel tab) and adjust Octree depth (Block tab) until desired result.
+ Create transparent voxel material:
  + Select the *Mission Control:Material* tab:
	+ Surface should show Principled BSDF shader.
	+ Lower the Alpha value (e.g. 0.4) to make transparent voxels.
+ Align the active camera to the view.
+ Use Cycles renderer.
+ Setup Sky Texture as the light source.
+ Render the voxel space.
+ Save the project in Blender format (*.blend*).

Cap the Octree depth to 8. Each additional level increases the node count exponentially ($8^{\mathit{depth}}$).

[^lod]: [Level of Detail](https://en.wikipedia.org/wiki/Level_of_detail_(computer_graphics)): dynamic 3D model representation. 

[^ot]: Outline Threshold: object is approximated with bounding boxes only.

[^panel]: The View menu should be rebranded Panels to avoid confusion with Viewport views. 
