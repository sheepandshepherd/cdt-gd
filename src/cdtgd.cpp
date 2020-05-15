#include "cdtgd.h"

#include <Array.hpp>
#include <PoolArrays.hpp>

using Point = CDT::V2d<float>;
using Vertex = CDT::Vertex<float>;
using Edge = CDT::Edge;

using namespace godot;

#define BIND_METHOD(Name) register_method(#Name, &ConstrainedTriangulation::Name);

void ConstrainedTriangulation::_register_methods()
{
	BIND_METHOD(insert_vertices)
	BIND_METHOD(insert_edges)

	BIND_METHOD(insert_polygon)

	BIND_METHOD(erase_super_triangle)
	BIND_METHOD(erase_outer_triangles)
	BIND_METHOD(erase_outer_triangles_and_holes)

	BIND_METHOD(get_triangle_at_point)

	BIND_METHOD(get_vertex_count)
	BIND_METHOD(get_vertex)
	BIND_METHOD(get_vertex_triangles)
	BIND_METHOD(get_all_vertices)

	BIND_METHOD(get_triangle_count)

	BIND_METHOD(get_triangle)
	BIND_METHOD(get_triangle_neighbors)
	BIND_METHOD(get_all_triangles)
	BIND_METHOD(get_all_triangle_neighbors)
}

void ConstrainedTriangulation::_init()
{
}

Point g2s(Vector2 v)
{
	return Point::make(v.x, v.y);
}

Vector2 s2g(Point p)
{
	return Vector2(p.x, p.y);
}

Vector2 s2g(Vertex p)
{
	return Vector2(p.pos.x, p.pos.y);
}

template<class PA, class T>
std::vector<T> g2s(const PA &input)
{
	std::vector<T> output;
	output.resize(input.size());
	{
		typename PA::Read read = input.read();
		for(int i = 0; i < input.size(); ++i)
		{
			output[i] = g2s(read[i]);
		}
	}
	return output;
}

template<class T, class PA>
PA s2g(const std::vector<T> &input)
{
	PA output;
	output.resize(input.size());
	{
		typename PA::Write write = output.write();
		for(int i = 0; i < input.size(); ++i)
		{
			write[i] = s2g(input[i]);
		}
	}
	return output;
}

void ConstrainedTriangulation::insert_vertices(PoolVector2Array vertices)
{
	std::vector<Point> v = g2s<PoolVector2Array, Point>(vertices);
	triangulation.insertVertices(v);
	vert_count += v.size();
}

void ConstrainedTriangulation::insert_edges(PoolIntArray edges)
{
	int new_edges = edges.size()/2;
	std::vector<Edge> e;
	e.reserve(new_edges);
	PoolIntArray::Read read = edges.read();
	for(int i = 0; i < new_edges; ++i)
	{
		e.push_back(Edge(read[2*i], read[2*i+1]));
	}
	triangulation.insertEdges(e);
}

void ConstrainedTriangulation::insert_polygon(PoolVector2Array polygon)
{
	int base = vert_count;
	std::vector<Point> v = g2s<PoolVector2Array, Point>(polygon);
	triangulation.insertVertices(v);

	std::vector<Edge> e;
	int l = v.size();
	e.reserve(l);
	for(int i = 0; i < l; ++i)
	{
		e.push_back(Edge(i + base, (i+1)%l + base));
	}
	triangulation.insertEdges(e);
}

void ConstrainedTriangulation::erase_super_triangle()
{
	triangulation.eraseSuperTriangle();
}
void ConstrainedTriangulation::erase_outer_triangles()
{
	triangulation.eraseOuterTriangles();
}
void ConstrainedTriangulation::erase_outer_triangles_and_holes()
{
	triangulation.eraseOuterTrianglesAndHoles();
}

int ConstrainedTriangulation::get_triangle_at_point(Vector2 point)
{
	Point p = g2s(point);
	for(int t = 0; t < triangulation.triangles.size(); ++t)
	{
		CDT::VerticesArr3 verts = triangulation.triangles[t].vertices;
		Point a = triangulation.vertices[verts[0]].pos;
		Point b = triangulation.vertices[verts[1]].pos;
		Point c = triangulation.vertices[verts[2]].pos;
		if(CDT::locatePointTriangle<float>(p, a, b, c) != CDT::PtTriLocation::Outside)
		{
			return t;
		}
	}
	return -1;
}

int ConstrainedTriangulation::get_vertex_count()
{
	return triangulation.vertices.size();
}
Vector2 ConstrainedTriangulation::get_vertex(int vertex_index)
{
	return s2g(triangulation.vertices[vertex_index].pos);
}
PoolIntArray ConstrainedTriangulation::get_vertex_triangles(int vertex_index)
{
	PoolIntArray ret;
	PoolIntArray::Write write = ret.write();

	CDT::TriIndVec tris = triangulation.vertices[vertex_index].triangles;

	ret.resize(tris.size());
	for(int i = 0; i < tris.size(); ++i) write[i] = tris[i];
	return ret;
}
PoolVector2Array ConstrainedTriangulation::get_all_vertices()
{
	PoolVector2Array ret = s2g<Vertex, PoolVector2Array>(triangulation.vertices);
	return ret;
}

int ConstrainedTriangulation::get_triangle_count()
{
	return triangulation.triangles.size();
}
// TODO: change to Vector3i in 4.0
Vector3 ConstrainedTriangulation::get_triangle(int triangle_index)
{
	CDT::VerticesArr3 verts = triangulation.triangles[triangle_index].vertices;
	return Vector3(verts[0], verts[1], verts[2]);
}
Vector3 ConstrainedTriangulation::get_triangle_neighbors(int triangle_index)
{
	CDT::NeighborsArr3 neighbors = triangulation.triangles[triangle_index].neighbors;
	return Vector3(neighbors[0], neighbors[1], neighbors[2]);
}
PoolIntArray ConstrainedTriangulation::get_all_triangles()
{
	PoolIntArray ret;
	int tris = triangulation.triangles.size();
	ret.resize(tris*3);
	PoolIntArray::Write write = ret.write();
	for(int i = 0; i < tris; ++i)
	{
		write[3*i] = triangulation.triangles[i].vertices[0];
		write[3*i+1] = triangulation.triangles[i].vertices[1];
		write[3*i+2] = triangulation.triangles[i].vertices[2];
	}
	return ret;
}
PoolIntArray ConstrainedTriangulation::get_all_triangle_neighbors()
{
	PoolIntArray ret;
	int tris = triangulation.triangles.size();
	ret.resize(tris*3);
	PoolIntArray::Write write = ret.write();
	for(int i = 0; i < tris; ++i)
	{
		write[3*i] = triangulation.triangles[i].neighbors[0];
		write[3*i+1] = triangulation.triangles[i].neighbors[1];
		write[3*i+2] = triangulation.triangles[i].neighbors[2];
	}
	return ret;
}
