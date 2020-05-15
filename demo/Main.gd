extends Node2D

onready var poly = $Polygon2D
var holes = []
var cdt = ConstrainedTriangulation.new()

var verts
var tris

func _draw():
	for tri in tris.size() / 3:
		for i in 3:
			var from = verts[tris[3*tri + i]]
			var to = verts[tris[3*tri + (i+1)%3]]
			draw_line(from, to, Color(1,0,0), 1.0, true )
	for vert in verts:
		draw_circle(vert, 2.5, Color(0,1,0))

func _ready():
	var edge_count = poly.polygon.size()
	var edges = []
	var v = poly.polygon
	
	# insert outer polygon
	# FIXME: insert_polygon breaks CDT if called more than once because it adds
	#  new vertices after edges have been added. So we need to add edges later
	#cdt.insert_polygon(poly.polygon)
	for i in poly.polygon.size():
		edges.append(i)
		edges.append((i+1)%(poly.polygon.size()))
	
	# insert each hole polygon
	for c in poly.get_children():
		if c is Polygon2D:
			#cdt.insert_polygon(c.polygon)
			v.append_array(c.polygon)
			for i in c.polygon.size():
				edges.append(i + edge_count)
				edges.append((i+1)%(c.polygon.size()) + edge_count)
			edge_count += c.polygon.size()
	
	# insert all vertices before any edges
	cdt.insert_vertices(v)
	cdt.insert_edges(edges)
	
	#cdt.erase_super_triangle()
	#cdt.erase_outer_triangles()
	cdt.erase_outer_triangles_and_holes()
	
	
	verts = (cdt.get_vertices())
	tris = (cdt.get_indices())
	print("verts: ", verts)
	print("tris: ", tris)

