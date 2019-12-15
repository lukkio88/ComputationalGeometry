#pragma once
#include<weak_dcel.h>

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