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

	PoolVector2Array get_vertices();
	PoolIntArray get_indices();
};

}
