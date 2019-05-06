#pragma once
#include <segment.h>
#include <vector>
#include <algorithm>
#include <list>

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
};

/**
* @brief This represents a face, not that compared to a classic dcel we don't have inner components
*/
class Face {
public:
	HalfEdgeIter outer;
};

/**
 * @brief this class represents a doubly connected edge list.
 */
class DCEL {
public:
	/**
	@brief return true if the incident half edge is nil, false otherwise
	*/
	bool isIsolated(VertexIter vertexIter);

	/**
	@brief Check if the ingoing half edge is boundary
	*/
	bool isBoundary(VertexIter vertexIter);

	/**
	@brief Check if incident to the unbounded face
	*/
	bool isBoundary(HalfEdgeIter halfEdgeIter);

	/**
	@brief Check if at least one of the incident half edges has a twin whose incident
	face is the unbounded face
	*/
	bool isBoundary(FaceIter faceIter);

	/**
	@brief Return true if different from nil
	*/
	bool isValid(HalfEdgeIter halfEdgeIter);

	/**
	@brief Adjusting vertex, so that the incident half edge is the twin of a boundary one
	*/
	HalfEdgeIter adjustVertex(VertexIter vertexIter);

	/**
	@brief add a single isolated vertex
	*/
	VertexIter addVertex(Point coords);
	/**
	@brief Creating an half-edge (and its twin) going from vertex v to w
	*/
	HalfEdgeIter addHalfEdge(VertexIter vertexStart, VertexIter vertexEnd);

	/**
	@brief Creates a new polygon and return the incident faceIter
	*/
	FaceIter addPoly(std::vector<VertexIter> vertexIter);

	/*
	@brief Returns the half edge going from start to end, if exists, nil otherwise
	*/
	HalfEdgeIter getHalfEdge(VertexIter vertexStart, VertexIter vertexEnd);

	inline VertexIter vBegin() { return mVertex.begin(); }
	inline HalfEdgeIter heBegin() { return mHalfEdge.begin(); }
	inline FaceIter fBegin() { return mFace.begin(); }

	inline VertexIter vEnd() { return mVertex.end(); }
	inline HalfEdgeIter heEnd() { return mHalfEdge.end(); }
	inline FaceIter fEnd() { return mFace.end(); }

	//TODO: Implement circulators

private:

	container<Vertex> mVertex;
	container<HalfEdge> mHalfEdge;
	container<Face> mFace;
};