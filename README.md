CDT for Godot Engine
====================

[CDT](https://github.com/artem-ogre/CDT) is a robust constrained Delaunay triangulation library.

### Usage

#### Installation

1. Download binary releases from here: <https://github.com/sheepandshepherd/cdt-gd/releases>
2. Extract into Godot project root

#### Triangulation

```gdscript
var cdt = ConstrainedTriangulation.new()

var vertices : PoolVector2Array = ... # array of all vertices
var edges : PoolIntArray = ...        # pairs of indices into `vertices`

# Vertices must be added before edges.
cdt.insert_vertices(vertices)
cdt.insert_edges(edges)

# Call one of these to complete the triangulation:
cdt.erase_super_triangle()            # no triangles are removed; result is a convex hull
cdt.erase_outer_triangles()           # triangles outside the outermost polygon are removed
cdt.erase_outer_triangles_and_holes() # holes are also removed

# Get CDT output
var cdt_vertices : PoolVector2Array = cdt.get_vertices() # all vertices; probably the same as `vertices`?
var cdt_indices : PoolIntArray = cdt.get_indices()       # triangles as indices into `cdt_vertices`
```


