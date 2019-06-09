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

	/**
	@brief Splitting the edge start-end into two
	*/
	VertexIter splitEdge(float alpha, HalfEdgeIter innerHalfEdge);

	/**
	@brief joint the origin vertex of two halfedges if they're incident to the same face
	*/
	FaceIter join(HalfEdgeIter halfEdgeStart, HalfEdgeIter halfEdgeEnd);

	/*
	@brief Returns the half edge going from start to end, if exists, nil otherwise
	*/
	HalfEdgeIter getHalfEdge(VertexIter vertexStart, VertexIter vertexEnd);

	/**
	@brief Get half edge whose origin is in vertex and incident face is face
	*/
	HalfEdgeIter DCEL::getHalfEdge(VertexIter vertex, FaceIter face);

	inline VertexIter vBegin() { return mVertex.begin(); }
	inline HalfEdgeIter heBegin() { return mHalfEdge.begin(); }
	inline FaceIter fBegin() { return mFace.begin(); }

	inline VertexIter vEnd() { return mVertex.end(); }
	inline HalfEdgeIter heEnd() { return mHalfEdge.end(); }
	inline FaceIter fEnd() { return mFace.end(); }

	void mergeSubdivisions(DCEL & subdivision1, DCEL & subdivision2);
	std::vector<EventPoint> computeIntersection(std::vector<Edge> & S);
	void planarOverlay(DCEL & subdivision1, DCEL & subdivision2);

	//TODO: Implement circulators

private:

	container<Vertex> mVertex;
	container<HalfEdge> mHalfEdge;
	container<Face> mFace;

};


struct Edge : public Segment {

	Edge() :Segment() { ; }

	Edge(const Point& p, const Point& q) :Segment(p, q) { ; }

	Edge(HalfEdgeIter halfEdgeIter, int code) :
		Segment(halfEdgeIter->origin->coords, halfEdgeIter->twin->origin->coords), mHalfEdge(halfEdgeIter), mCode(code) {
		;
	}

	Edge split(const Point& point)
	{

		Point upPoint = getMin(p, q);
		Point downPoint = getMax(p, q);

		auto alpha = (point - p)*(q - p) / ((q - p)*(q - p));
		VertexIter vHandle = mSubdivision->splitEdge(alpha, mHalfEdge);

		Point newCoords = vHandle->coords;

		Edge edge_new(mHalfEdge->next, mCode);

		if(getDown(edge_new) == newCoords) //need to swap the two segments
		{

			Edge edge_tmp(mHalfEdge, mCode);
			mHalfEdge = edge_new.mHalfEdge;
			p = edge_new.p;
			q = edge_new.q;
			
			edge_new.mHalfEdge = edge_tmp.mHalfEdge;
			edge_new.p = edge_tmp.p;
			edge_new.q = edge_tmp.q;			
		}
		return edge_new;
	}

	HalfEdgeIter getDownwardHalfEdge()
	{
		if (getUp(*this) == mHalfEdge->origin->coords)
		{
			return mHalfEdge;
		}
		else
		{
			return mHalfEdge->twin;
		}
	}

	DCEL * mSubdivision;
	HalfEdgeIter mHalfEdge;
	int mCode;
};


struct EventPoint {

	void adjustEdges() {
		if (incident[0].size() == 0) //Only need to adjust the vertexHandle
		{
			for (auto edge : incident[1])
			{
				if (edge.p == vertexHandle->coords)
				{
					edge.mHalfEdge->origin = vertexHandle;
				}
				else
				{
					edge.mHalfEdge->twin->origin = vertexHandle;
				}
			}
		}
		else if (incident[1].size() == 0) //Only need to adjust the vertex handle
		{
			for (auto edge : incident[0])
			{
				if (edge.p == vertexHandle->coords)
				{
					edge.mHalfEdge->origin = vertexHandle;
				}
				else
				{
					edge.mHalfEdge->twin->origin = vertexHandle;
				}
			}
		}
		else //Merge and set the vertex handle
		{

			int incident0Size = incident[0].size();
			int incident1Size = incident[1].size();
			int i0 = 0, i1 = 1; //Applying merging, merge sort style

			Point coords = vertexHandle->coords;

			std::vector<Edge> edge(incident0Size + incident1Size);
			int i = 0;
			while (i0 < incident0Size && i1 < incident1Size)
			{
				if (compare(incident[0][i0], incident[1][i1]))
				{
					edge[i++] = incident[0][i0++];
				}
				else
				{
					edge[i++] = incident[1][i1++];
				}
			}

			if (i0 == incident0Size)
			{
				while (i1 < incident1Size)
				{
					edge[i++] = incident[1][i1++];
				}
			}
			else {
				while (i0 < incident0Size)
				{
					edge[i++] = incident[0][i0++];
				}
			}
		}
	}

	bool compare(Edge e1, Edge e2) {
		Point p1, p2;
		if (e1.p == vertexHandle->coords)
			p1 = e1.q;
		else
			p1 = e1.p;

		if (e2.p == vertexHandle->coords)
			p2 = e2.q;
		else
			p2 = e2.p;

		auto angle1 = atan2((p1 - vertexHandle->coords).y, (p1 - vertexHandle->coords).x);
		auto angle2 = atan2((p2 - vertexHandle->coords).y, (p2 - vertexHandle->coords).x);

		return angle1 < angle2;
	};


	VertexIter vertexHandle;
	std::array<std::vector<Edge>,2> incident;

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