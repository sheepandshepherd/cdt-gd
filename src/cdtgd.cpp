#include "cdtgd.h"

#include <Array.hpp>
#include <PoolArrays.hpp>

using Point = CDT::V2d<float>;
using Vertex = CDT::Vertex<float>;
using Edge = CDT::Edge;

using namespace godot;

void ConstrainedTriangulation::_register_methods()
{
	register_method("insert_vertices", &ConstrainedTriangulation::insert_vertices);
	register_method("insert_edges", &ConstrainedTriangulation::insert_edges);

	register_method("insert_polygon", &ConstrainedTriangulation::insert_polygon);

	register_method("erase_super_triangle", &ConstrainedTriangulation::erase_super_triangle);
	register_method("erase_outer_triangles", &ConstrainedTriangulation::erase_outer_triangles);
	register_method("erase_outer_triangles_and_holes", &ConstrainedTriangulation::erase_outer_triangles_and_holes);

	register_method("get_vertices", &ConstrainedTriangulation::get_vertices);
	register_method("get_indices", &ConstrainedTriangulation::get_indices);
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

PoolVector2Array ConstrainedTriangulation::get_vertices()
{
	PoolVector2Array ret = s2g<Vertex, PoolVector2Array>(triangulation.vertices);
	return ret;
}
PoolIntArray ConstrainedTriangulation::get_indices()
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
