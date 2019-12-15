#pragma once
#include <segment.h>
#include <vector>
#include <algorithm>
#include <list>
#include <array>
#include <cmath>
#include <line_seg_intersection.h>

class Vertex;
class HalfEdge;
class Face;
class DCEL;

template<typename T>
using container = std::list<T>;

using VertexIter = container<Vertex>::iterator;
using HalfEdgeIter = container<HalfEdge>::iterator;
using FaceIter = container<Face>::iterator;
using VertexCIter = container<Vertex>::const_iterator;
using HalfEdgeCIter = container<HalfEdge>::const_iterator;
using FaceCIter = container<Face>::const_iterator;

/**
* @brief Represents a vertex
*/
class Vertex {
public:
	int idx;
	Point coords;
	HalfEdgeIter incident;
};

/**
* @brief Representing half-edge, i.e. a directed edge.
*/
class HalfEdge {
public:
	VertexIter origin;
	HalfEdgeIter prev, next, twin;
	FaceIter incident;
	void* data;
};

/**
* @brief This represents a face, not that compared to a classic dcel we don't have inner components
*/
class Face {
public:
	HalfEdgeIter outer;
};

struct EventPoint;
struct Edge;

//This class doesn't take into account faces, it's a "weaker" dcel, useful if we have to build one incrementally
class WeakDCEL
{
public:
	/**
	@brief add a single isolated vertex
	*/
	VertexIter addVertex(Point coords);

	/**
	@brief Creating an half-edge (and its twin) going from vertex v to w
	*/
	HalfEdgeIter addHalfEdge(VertexIter vertexStart, VertexIter vertexEnd);

	/**
	@brief Splitting the edge start-end into two. The second argument represents an
	half edge incident to the vertex handle
	*/
	VertexIter splitEdge(VertexIter vertexHandle, HalfEdgeIter innerHalfEdge);

	/**
	@brief Return num vertices
	*/
	inline int numVertices() const { return mVertex.size(); }

	/**
	@brief Return num edges
	*/
	inline int numEdges() const { return mHalfEdge.size() / 2; }

	/**
	@brief Return num faces
	*/
	inline int numFaces() const { return mFace.size(); }


	inline VertexIter vBegin() { return mVertex.begin(); }
	inline HalfEdgeIter heBegin() { return mHalfEdge.begin(); }
	inline FaceIter fBegin() { return mFace.begin(); }

	inline VertexIter vEnd() { return mVertex.end(); }
	inline HalfEdgeIter heEnd() { return mHalfEdge.end(); }
	inline FaceIter fEnd() { return mFace.end(); }

public:
	container<Vertex> mVertex;
	container<HalfEdge> mHalfEdge;
	container<Face> mFace;
};