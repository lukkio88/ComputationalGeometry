#pragma once
#include <segment.h>
#include <vector>
#include <algorithm>

using std::vector;

class Vertex;
class HalfEdge;
class Face;
class DCEL;

using VertexIter = vector<Vertex>::iterator;
using HalfEdgeIter = vector<HalfEdge>::iterator;
using FaceIter = vector<Face>::iterator;
using VertexCIter = vector<Vertex>::const_iterator;
using HalfEdgeCIter = vector<HalfEdge>::const_iterator;
using FaceCIter = vector<Face>::const_iterator;

/**
* @brief Represents a vertex
*/
class Vertex {
public:
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
struct DCEL {
public:
	DCEL();

	//boolean predicates

	/**
	@brief Check if the vertex is either isolated or incident to the unbounded face
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

	inline VertexIter vNil() { return mVertex.end(); }
	inline HalfEdgeIter heNil() { return mHalfEdge.end(); }
	inline FaceIter fNil() { return mFace.end(); }

private:
	vector<Vertex> mVertex;
	vector<HalfEdge> mHalfEdge;
	vector<Face> mFace;
};