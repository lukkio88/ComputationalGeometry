#include<weak_dcel.h>

using std::cout;
using std::endl;

VertexIter WeakDCEL::addVertex(Point coords) {
	int idx = mVertex.size();
	return mVertex.insert(mVertex.end(), Vertex{ idx, coords,heEnd() });
}

HalfEdgeIter WeakDCEL::addHalfEdge(VertexIter vertexStart, VertexIter vertexEnd) {
	HalfEdgeIter inner = mHalfEdge.insert(mHalfEdge.end(), HalfEdge{ vertexStart,heEnd(),heEnd(),heEnd(),fEnd() });
	HalfEdgeIter outer = mHalfEdge.insert(mHalfEdge.end(), HalfEdge{ vertexEnd, heEnd(),heEnd(),heEnd(),fEnd() });
	inner->twin = outer;
	outer->twin = inner;
	return inner;
}

VertexIter WeakDCEL::splitEdge(VertexIter vertexHandle, HalfEdgeIter innerHalfEdge)
{
	vertexHandle->incident = innerHalfEdge;
	HalfEdgeIter outerHalfEdge = innerHalfEdge->twin;
	HalfEdgeIter innerNext = innerHalfEdge->next;
	HalfEdgeIter outerPrev = outerHalfEdge->prev;
	VertexIter startVertex = innerHalfEdge->origin;
	VertexIter endVertex = outerHalfEdge->origin;

	HalfEdgeIter newInner = mHalfEdge.insert(heEnd(), HalfEdge{});
	HalfEdgeIter newOuter = mHalfEdge.insert(heEnd(), HalfEdge{});

	newInner->origin = vertexHandle;
	newInner->prev = innerHalfEdge;
	newInner->next = innerNext;
	newInner->twin = newOuter;

	newOuter->origin = endVertex;
	newOuter->prev = outerPrev;
	newOuter->next = outerHalfEdge;
	newOuter->twin = newInner;

	outerPrev->next = newOuter;
	innerNext->prev = newInner;
	innerHalfEdge->next = newInner;
	outerHalfEdge->prev = newOuter;

	return vertexHandle;

}