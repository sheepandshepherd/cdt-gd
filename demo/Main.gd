extends Node2D

onready var poly = $Polygon2D
var hovered_tri_index = -1
onready var hovered_tri = $HoveredTriangle
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
	
	
	verts = (cdt.get_all_vertices())
	tris = (cdt.get_all_triangles())
	print("verts: ", verts)
	print("tris: ", tris)

func _input(event):
	if event is InputEventMouseMotion:
		var tri = cdt.get_triangle_at_point(event.position)
		if tri != hovered_tri_index:
			hovered_tri_index = tri
			hovered_tri.visible = tri != -1
			if tri != -1:
				var indices = cdt.get_triangle(tri)
				var a = cdt.get_vertex(indices.x)
				var b = cdt.get_vertex(indices.y)
				var c = cdt.get_vertex(indices.z)
				hovered_tri.polygon = [a,b,c]
