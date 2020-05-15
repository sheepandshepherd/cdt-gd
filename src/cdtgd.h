#pragma once

#include <Godot.hpp>
#include <Reference.hpp>
#include <vector>

#define CDT_DONT_USE_BOOST_RTREE
#include <CDT.h>

using Triangulation = CDT::Triangulation<float>;

namespace godot {

class ConstrainedTriangulation : public Reference {
	GODOT_CLASS(ConstrainedTriangulation, Reference)
	Triangulation triangulation = Triangulation((CDT::FindingClosestPoint::Enum)0, 10);
	//std::vector<int> queued_edges;
	int vert_count = 0;

public:
	static void _register_methods();

	ConstrainedTriangulation()
	{
	}
	~ConstrainedTriangulation()
	{
	}

	void _init();

	void insert_vertices(PoolVector2Array vertices);
	void insert_edges(PoolIntArray edges);

	void insert_polygon(PoolVector2Array polygon);

	void erase_super_triangle();
	void erase_outer_triangles();
	void erase_outer_triangles_and_holes();

	int get_triangle_at_point(Vector2 point);

	int get_vertex_count();
	Vector2 get_vertex(int vertex_index);
	PoolIntArray get_vertex_triangles(int vertex_index);
	PoolVector2Array get_all_vertices();

	int get_triangle_count();
	// TODO: change to Vector3i in 4.0
	Vector3 get_triangle(int triangle_index);
	Vector3 get_triangle_neighbors(int triangle_index);
	PoolIntArray get_all_triangles();
	PoolIntArray get_all_triangle_neighbors();
};

}
