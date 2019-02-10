#pragma once
#include <segment.h>
#include <vector>

using std::vector;
using VertexIter = vector<Vertex>::iterator;
using HalfEdgeIter = vector<HalfEdge>::iterator;
using FaceIter = vector<Face>::iterator;
using VertexCIter = vector<Vertex>::const_iterator;
using HalfEdgeCIter = vector<HalfEdge>::const_iterator;
using FaceCIter = vector<Face>::const_iterator;

class Vertex;
class HalfEdge;
class Face;
class DCEL;

struct Vertex {
	HalfEdgeIter incident;
	Point coords;
};

struct HalfEdge {
	HalfEdgeIter prev, next, twin;
	VertexIter origin;
};

struct Face {
	HalfEdgeIter incident;
};

struct DCEL {
	vector<Vertex> vertex;
	vector<HalfEdge> halfEdge;
	vector<Face> face;
};