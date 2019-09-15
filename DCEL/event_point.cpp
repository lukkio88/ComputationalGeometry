#include <dcel.h>

static HalfEdgeIter findOutgoing(VertexIter & vertexHandle, Edge& edge) {
	if (edge.p == vertexHandle->coords) return edge.mHalfEdge;
	else return edge.mHalfEdge->twin;
}

static HalfEdgeIter findIngoing(VertexIter & vertexHandle, Edge& edge) {
	if (edge.p == vertexHandle->coords) return edge.mHalfEdge->twin;
	else return edge.mHalfEdge;
}

static void adjust(Edge & edge_prev, Edge & edge_curr, VertexIter& vertexHandle) {
	//Getting the relevant half edges
	HalfEdgeIter outgoing = findOutgoing(vertexHandle, edge_curr);
	HalfEdgeIter ingoing = findIngoing(vertexHandle, edge_prev);
	
	//Adjusting the halfedges
	outgoing->origin = vertexHandle;
	outgoing->prev = ingoing;
	ingoing->next = outgoing;
}

void EventPoint::adjustEdges() {

	std::array<int, 2> incidentSize = { incident[0].size(), incident[1].size() };
	int numIncidentEdges = incidentSize[0] + incidentSize[1];
	std::vector<Edge> incidentEdge;
	incidentEdge.reserve(numIncidentEdges);

	//First create the list of incident edges, sorted
	if (incident[0].size() == 0) {
		for (auto & edge : incident[1]) {
			incidentEdge.push_back(edge);
		}
	}
	else if (incident[1].size() == 0) {
		for (auto & edge : incident[0]) {
			incidentEdge.push_back(edge);
		}
	}
	else { //Merge and set the vertex handle
		int i0 = 0, i1 = 0; //Applying merging, merge sort style

		int i = 0;
		while (i0 < incidentSize[0] && i1 < incidentSize[1]) {
			if (compare(incident[0][i0], incident[1][i1])) { incidentEdge.push_back(incident[0][i0++]); }
			else { incidentEdge.push_back(incident[1][i1++]); }
		}

		if (i0 == incidentSize[0]) {
			while (i1 < incidentSize[1]) { incidentEdge.push_back(incident[1][i1++]); }
		}
		else {
			while (i0 < incidentSize[0]) { incidentEdge.push_back(incident[0][i0++]); }
		}
	}

	//Now traverse the list of edges and adjust the pointers
	for (int i = 0, ii = numIncidentEdges - 1; i < numIncidentEdges; ++i, ii = (ii + 1) % numIncidentEdges)
	{
		adjust(incidentEdge[ii], incidentEdge[i], vertexHandle);
	}
}

bool EventPoint::compare(Edge e1, Edge e2) {
	//This might not work everytime
	std::array<Point, 3> point = {
		vertexHandle->coords,
		findIngoing(vertexHandle, e1)->origin->coords,
		findIngoing(vertexHandle, e2)->origin->coords
	};
	return twiceArea(point[0], point[1], point[2]) > 0;
}