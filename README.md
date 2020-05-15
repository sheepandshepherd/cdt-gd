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
var cdt_vertices : PoolVector2Array = cdt.get_all_vertices()   # all vertices; probably the same as `vertices`?
var cdt_triangles : PoolIntArray = cdt.get_all_triangles()     # triangles as indices into `cdt_vertices`

# Other useful CDT output:
cdt.get_triangle_at_point(point : Vector2) -> int      # index of the triangle a point is inside, or -1 if none

cdt.get_vertex_count() -> int
cdt.get_vertex(vertex_index) -> Vector2
cdt.get_vertex_triangles(vertex_index) -> PoolIntArray # indices of all triangles a vertex is part of

cdt.get_triangle_count() -> int
cdt.get_triangle(triangle_index) -> Vector3            # indices of a triangle's 3 vertices
cdt.get_triangle_neighbors(triangle_index) -> Vector3  # indices of a triangle's 3 neighboring triangles
```


