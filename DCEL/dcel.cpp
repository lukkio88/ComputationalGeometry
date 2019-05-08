#include <dcel.h>

using std::cout;
using std::endl;

VertexIter DCEL::addVertex(Point coords) {
	int idx = mVertex.size();
	return mVertex.insert(mVertex.end(), Vertex{ idx, coords,heEnd() });
}

HalfEdgeIter DCEL::addHalfEdge(VertexIter vertexStart, VertexIter vertexEnd) {
	HalfEdgeIter inner = mHalfEdge.insert(mHalfEdge.end(), HalfEdge{vertexStart,heEnd(),heEnd(),heEnd(),fEnd()});
	HalfEdgeIter outer = mHalfEdge.insert(mHalfEdge.end(), HalfEdge{vertexEnd, heEnd(),heEnd(),heEnd(),fEnd()});
	inner->twin = outer;
	outer->twin = inner;
	return inner;
}

FaceIter DCEL::addPoly(std::vector<VertexIter> vertex) {

	std::vector<HalfEdgeIter> halfEdge(vertex.size());
	std::vector<bool> halfEdgeNew(vertex.size());
	std::vector<std::pair<HalfEdgeIter, HalfEdgeIter>> cacheConsecutiveHalfEdge;

	int numVertices = vertex.size();

	for (auto i = 0; i < vertex.size(); ++i)
	{
		if (!isBoundary(vertex[i]))
		{
			std::cout << "Error in addPoly, one of the vertices isn't boundary" << std::endl;
			return fEnd();
		}

		int iPlus1 = (i + 1) % numVertices;
		halfEdge[i] = getHalfEdge(vertex[i], vertex[iPlus1]);
		halfEdgeNew[i] = !isValid(halfEdge[i]);
		if ((!halfEdgeNew[i]) && !isBoundary(halfEdge[i])) //true if NOT incident to the unbounded face 
		{
			std::cout << "Error in addPoly, one of the half edges isn't incident to the unbounded face" << std::endl;
			return fEnd();
		}
	}

	//Pre-compute what half edges that need both to be relinked (if not new) and created (if new) 
	for (auto i = 0; i < vertex.size(); ++i)
	{

		int iPlus1 = (i + 1) % numVertices;
		if (halfEdgeNew[i])
		{
			halfEdge[i] = addHalfEdge(vertex[i], vertex[iPlus1]);
		}
		
		if (!halfEdgeNew[i] && !halfEdgeNew[iPlus1])
		{

			auto innerPrevHalfEdge = halfEdge[i];
			auto innerNextHalfEdge = halfEdge[iPlus1];

			if (innerPrevHalfEdge != innerNextHalfEdge)
			{
				auto outerNextHalfEdge = innerPrevHalfEdge->twin;
				auto outerPrevHalfEdge = innerNextHalfEdge->twin;
				auto boundaryPrevHalfEdge = outerPrevHalfEdge;
				do 
				{
					boundaryPrevHalfEdge = boundaryPrevHalfEdge->next->twin;
				} while (!isBoundary(boundaryPrevHalfEdge));

				if (boundaryPrevHalfEdge == innerPrevHalfEdge) 
				{
					std::cout << "Topology error when executing addPoly method" << std::endl;
					return fEnd();
				}

				auto boundaryNextHalfEdge = boundaryPrevHalfEdge->next;

				cacheConsecutiveHalfEdge.push_back(
					std::make_pair(boundaryPrevHalfEdge, innerPrevHalfEdge->next)
				);
				cacheConsecutiveHalfEdge.push_back(
					std::make_pair(innerNextHalfEdge->prev,boundaryNextHalfEdge)
				);
				cacheConsecutiveHalfEdge.push_back(
					std::make_pair(innerPrevHalfEdge,innerNextHalfEdge)
				);
			}
		}
	}

	auto faceIter = mFace.insert(mFace.end(), Face{ halfEdge.back() });

	for (int i = 0; i < vertex.size(); ++i)
	{

		int iPlus1 = (i + 1) % numVertices;

		auto ithVertexIter = vertex[i];
		auto ithP1vertexIter = vertex[iPlus1];

		int idCase = 0;
		if (halfEdgeNew[i]) idCase |= 0x1;
		if (halfEdgeNew[iPlus1]) idCase |= 0x2;

		auto innerPrevHalfEdge = halfEdge[i];
		auto innerNextHalfEdge = halfEdge[iPlus1];

		if (idCase)
		{

			auto outerNextHalfEdge = innerPrevHalfEdge->twin;
			auto outerPrevHalfEdge = innerNextHalfEdge->twin;

			if (idCase == 0x1) //current is new, next is old
			{
				auto boundaryPrevHalfEdge = innerNextHalfEdge->prev;
				cacheConsecutiveHalfEdge.push_back(std::make_pair(boundaryPrevHalfEdge, outerNextHalfEdge));
				ithP1vertexIter->incident = innerPrevHalfEdge;
			}
			else if (idCase == 0x2) //current is old, next is new
			{
				auto boundaryNextHalfEdge = innerPrevHalfEdge->next;
				cacheConsecutiveHalfEdge.push_back(std::make_pair(outerPrevHalfEdge, boundaryNextHalfEdge));
				ithP1vertexIter->incident = boundaryNextHalfEdge->twin;
			}
			else if (idCase == 0x3) //both new
			{
				if (isIsolated(ithP1vertexIter))
				{
					ithP1vertexIter->incident = innerPrevHalfEdge;
					cacheConsecutiveHalfEdge.push_back(std::make_pair(outerPrevHalfEdge, outerNextHalfEdge));
				}
				else
				{
					auto boundaryPrevHalfEdge = ithP1vertexIter->incident;
					auto boundaryNextHalfEdge = boundaryPrevHalfEdge->next;
					cacheConsecutiveHalfEdge.push_back(std::make_pair(boundaryPrevHalfEdge, outerNextHalfEdge));
					cacheConsecutiveHalfEdge.push_back(std::make_pair(outerPrevHalfEdge, boundaryNextHalfEdge));
				}
			}
			cacheConsecutiveHalfEdge.push_back(std::make_pair(innerPrevHalfEdge, innerNextHalfEdge));
		}
		halfEdge[i]->incident = faceIter;
	}

	//perform actual linking of the polygon boundary
	for (auto & pair : cacheConsecutiveHalfEdge)
	{
		pair.first->next = pair.second;
		pair.second->prev = pair.first;
	}

	//Final re-adjust of the ingoing halfedges
	for (auto & currentVertex : vertex)
		adjustVertex(currentVertex);

	return faceIter;

}
//Todo: test this method
VertexIter DCEL::splitEdge(float alpha, VertexIter start, VertexIter end) 
{
	HalfEdgeIter start2End = getHalfEdge(start, end);
	HalfEdgeIter end2Start = start2End->twin;
	HalfEdgeIter halfEdgeInner = start2End->next;
	HalfEdgeIter halfEdgeOuter = end2Start->prev;
	FaceIter faceInner = start2End->incident;
	FaceIter faceOuter = end2Start->incident;

	VertexIter vertexIter = addVertex((1.0f - alpha)*(start->coords) + alpha * (end->coords));
	HalfEdgeIter new2End = mHalfEdge.insert(heEnd(), 
		HalfEdge{vertexIter,start2End,heEnd(),start2End->next,faceInner});
	HalfEdgeIter end2New = mHalfEdge.insert(heEnd(), 
		HalfEdge{end,halfEdgeOuter,heEnd(),end2Start,faceOuter});

	new2End->twin = end2New;
	end2New->twin = new2End;

	halfEdgeInner->prev = new2End;
	end2Start->origin = vertexIter;
	end2Start->prev = end2New;

	return vertexIter;
}

//Todo: test this method
FaceIter DCEL::join(HalfEdgeIter halfEdgeStart, HalfEdgeIter halfEdgeEnd)
{
	if (halfEdgeStart->incident != halfEdgeEnd->incident)
		return fEnd();

	if (halfEdgeStart != halfEdgeEnd->prev)
		return halfEdgeStart->incident;

	HalfEdgeIter startPrev = halfEdgeStart->prev;
	HalfEdgeIter endPrev = halfEdgeEnd->prev;
	VertexIter startVertex = halfEdgeStart->origin;
	VertexIter endVertex = halfEdgeEnd->origin;
	FaceIter face = halfEdgeStart->incident;
	FaceIter faceNew = mFace.insert(fEnd(), Face{ halfEdgeEnd });

	HalfEdgeIter start2End = mHalfEdge.insert(heEnd(),
		HalfEdge{ startVertex,startPrev,heEnd(),halfEdgeEnd,faceNew }
		);

	HalfEdgeIter end2Start = mHalfEdge.insert(heEnd(),
		HalfEdge{ endVertex,endPrev,heEnd(),halfEdgeStart,face}
	);

	start2End->twin = end2Start;
	end2Start->twin = start2End;

	halfEdgeEnd->prev = start2End;
	halfEdgeStart->prev = end2Start;
	startPrev->next = start2End;
	endPrev->next = end2Start;

	//Adjusting related half edges to point to the new face
	HalfEdgeIter current = start2End;
	do
	{
		current->incident = faceNew;
		current = current->next;
	} while (current != start2End);

}

bool DCEL::isIsolated(VertexIter vertexIter)
{
	return vertexIter->incident == heEnd();
}

bool DCEL::isBoundary(VertexIter vertexIter) 
{
	return (isIsolated(vertexIter) || isBoundary(vertexIter->incident));
}

bool DCEL::isBoundary(HalfEdgeIter he) 
{
	return (he->incident == fEnd());
}

bool DCEL::isBoundary(FaceIter f) 
{
	auto he = f->outer;
	auto curr_he = he;
	do {
		if (isBoundary(curr_he->twin))
			return true;
		curr_he = curr_he->next;
	} while (curr_he != he);
}

bool DCEL::isValid(HalfEdgeIter halfEdgeIter) 
{
	return (halfEdgeIter != heEnd());
}

HalfEdgeIter DCEL::adjustVertex(VertexIter vertexIter) {
	if (isIsolated(vertexIter))
		return heEnd();

	auto halfEdgeSentinel = vertexIter->incident;
	auto currHalfEdge = halfEdgeSentinel;
	do {
		if(isBoundary(currHalfEdge))
		{
			vertexIter->incident = currHalfEdge;
			return currHalfEdge;
		}
		currHalfEdge = currHalfEdge->next->twin;
	} while (currHalfEdge != halfEdgeSentinel);

	return vertexIter->incident;

}

HalfEdgeIter DCEL::getHalfEdge(VertexIter vertexStart, VertexIter vertexEnd) 
{
	
	if (!isIsolated(vertexStart))
	{
		auto incidentHalfEdge = vertexStart->incident;
		auto currentHalfEdge = incidentHalfEdge;
		do {
			if (currentHalfEdge->origin == vertexEnd)
			{
				return currentHalfEdge->twin;
			}
			currentHalfEdge = currentHalfEdge->next->twin;
		} while (currentHalfEdge != incidentHalfEdge);
	}

	return heEnd();
}

HalfEdgeIter DCEL::getHalfEdge(VertexIter vertex, FaceIter face)
{
	if (isIsolated(vertex))
		return heEnd();

	auto incidentHalfEdge = vertex->incident;
	auto currentHalfEdge = incidentHalfEdge;
	do
	{
		if (currentHalfEdge->incident == face)
			return currentHalfEdge;
	} while (currentHalfEdge != incidentHalfEdge);

	return heEnd();

}