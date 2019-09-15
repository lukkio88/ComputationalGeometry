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

/**
 * @brief this class represents a doubly connected edge list.
 */
class DCEL : public WeakDCEL {
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
	@brief Creates a new polygon and return the incident faceIter
	*/
	FaceIter addPoly(std::vector<VertexIter> vertexIter);

	/**
	@brief joint the origin vertex of two halfedges if they're incident to the same face
	*/
	FaceIter join(HalfEdgeIter halfEdgeStart, HalfEdgeIter halfEdgeEnd);

	/*
	Same as the base class, but it updates the face data as well
	*/
	VertexIter splitEdge(VertexIter vertexHandle, HalfEdgeIter innerHalfEdge);

	/*
	@brief Returns the half edge going from start to end, if exists, nil otherwise
	*/
	HalfEdgeIter getHalfEdge(VertexIter vertexStart, VertexIter vertexEnd);

	/**
	@brief Get half edge whose origin is in vertex and incident face is face
	*/
	HalfEdgeIter DCEL::getHalfEdge(VertexIter vertex, FaceIter face);

	std::vector<Edge> mergeSubdivisions(DCEL & subdivision1, DCEL & subdivision2);
	std::vector<EventPoint> computeIntersection(std::vector<Edge> & S);
	void planarOverlay(DCEL & subdivision1, DCEL & subdivision2);

	//TODO: Implement circulators

};

struct Edge : public Segment {

	Edge();
	Edge(const Point& p, const Point& q);
	Edge(HalfEdgeIter halfEdgeIter, int code);
	Edge(DCEL * subdivision, HalfEdgeIter halfEdgeIter, int code);
	Edge split(const VertexIter& point);
	HalfEdgeIter getDownwardHalfEdge();

	DCEL * mSubdivision;
	HalfEdgeIter mHalfEdge;
	int mCode;
};

struct EventPoint {

	void adjustEdges();
	bool compare(Edge e1, Edge e2);

	VertexIter vertexHandle;
	std::array<std::vector<Edge>, 2> incident;

	/*
	According to deBerg et al, for each event point
	we need to store the immediate left half edge. This is done
	in order to find arcs in the graph representing the boundary cycles.

	Still according to deBerg for each boundary cycle we define a graph node,
	an arc is drawn between to nodes V and W if one of the two (say V) is the boundary of
	a hole and the other (say W) has an half edge immediately to the left of the left most
	vertex of the first (say V).

	In otherwords in the very general representation of half edge data structures where
	each face has one outer component (by definition) and possible multiple inner components
	we need to find this out by constructing this graph.

	In my implementation at the moment no face can have inner components therefore the graph would
	simply consist of N vertices, where N is the number of nodes, and N connected components,
	where each component has simply one node.

	Therefore we could skip this computation probably. However for sake of completeness
	I'll implement the graph construction anyway, with the expected result as the one mentioned above.
	*/

	bool hasImmediateLeft;
	HalfEdgeIter immediateLeft;
};