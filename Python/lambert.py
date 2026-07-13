##\file lambert.py
#\brief Lambert Azimuthal Equal-Area Projection.
#Preserve the sector surface area when flattening a sphere to a circular disk.
#
#ParaView ProgramableFilter script. Create ProgrammableFilter and copy paste
#the file content the Script section. Use with the CIE hemispheres created with `cievtk`.

import numpy as np

# Get input and output data structures
input_data = self.GetInputDataObject(0, 0)
output_data = self.GetOutputDataObject(0)

# Deep copy geometry and topology to preserve everything
output_data.ShallowCopy(input_data)

# Fetch 3D coordinates
pts = input_data.GetPoints()
pts_data = vtk.util.numpy_support.vtk_to_numpy(pts.GetData())

x = pts_data[:, 0]
y = pts_data[:, 1]
z = pts_data[:, 2]

# Calculate sphere radius from coordinates dynamically
r_3d = np.sqrt(x**2 + y**2 + z**2)
# Prevent division by zero at the absolute center if it exists
r_3d = np.where(r_3d == 0, 1.0, r_3d)

# Calculate spherical mapping angles
phi = np.arccos(np.clip(z / r_3d, -1.0, 1.0))
theta = np.arctan2(y, x)

# Lambert Azimuthal Equal-Area radius formula
# (Assumes sphere centered at 0,0,0 with uniform radius)
R = np.max(r_3d)
r_flat = 2.0 * R * np.sin(phi / 2.0)

# Overwrite coordinates directly in place
flat_pts = np.zeros_like(pts_data)
flat_pts[:, 0] = r_flat * np.cos(theta) # New X
flat_pts[:, 1] = r_flat * np.sin(theta) # New Y
flat_pts[:, 2] = 0.0                    # Flattened Z

# Push coordinates back to the pipeline output
new_points = vtk.vtkPoints()
new_points.SetData(vtk.util.numpy_support.numpy_to_vtk(flat_pts, deep=1))
output_data.SetPoints(new_points)
