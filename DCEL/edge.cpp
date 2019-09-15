#include <dcel.h>

Edge::Edge() :Segment() { ; }

Edge::Edge(const Point& p, const Point& q) :Segment(p, q) { ; }

Edge::Edge(HalfEdgeIter halfEdgeIter, int code) :
	Segment(halfEdgeIter->origin->coords, halfEdgeIter->twin->origin->coords), mHalfEdge(halfEdgeIter), mCode(code) {
	;
}

Edge::Edge(DCEL * subdivision, HalfEdgeIter halfEdgeIter, int code) :
	Segment(halfEdgeIter->origin->coords, halfEdgeIter->twin->origin->coords), mHalfEdge(halfEdgeIter), mCode(code) {
	mSubdivision = subdivision;
}

Edge Edge::split(const VertexIter& vertexHandle)
{

	Point upPoint = getMin(p, q);
	Point downPoint = getMax(p, q);

	mSubdivision->splitEdge(vertexHandle, mHalfEdge);

	Point newCoords = vertexHandle->coords;

	Edge edge_new(mSubdivision, mHalfEdge->next, mCode);

	if (getDown(edge_new) == newCoords) //need to swap the two segments
	{

		Edge edge_tmp(mSubdivision, mHalfEdge, mCode);
		mHalfEdge = edge_new.mHalfEdge;
		p = edge_new.p;
		q = edge_new.q;

		edge_new.mHalfEdge = edge_tmp.mHalfEdge;
		edge_new.p = edge_tmp.p;
		edge_new.q = edge_tmp.q;
	}
	return edge_new;
}

HalfEdgeIter Edge::getDownwardHalfEdge()
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
