#include <dcelIO.h>
#include <fstream>
#include <sstream>

void dumpPly(DCEL& subdivision, const std::string& filename)
{
	std::ofstream outputStream(filename);

	outputStream << "ply" << std::endl;
	outputStream << "format ascii 1.0" << std::endl;
	outputStream << "element vertex " << subdivision.numVertices() << std::endl;
	outputStream << "property float32 x" << std::endl;
	outputStream << "property float32 y" << std::endl;
	outputStream << "property float32 z" << std::endl;
	outputStream << "element face " << subdivision.numFaces() << std::endl;
	outputStream << "property list uint8 int32 vertex_index" << std::endl;
	outputStream << "end_header" << std::endl;
	for (auto vertex = subdivision.vBegin(); vertex != subdivision.vEnd(); ++vertex)
	{
		outputStream << vertex->coords.x << " " << vertex->coords.y << " " << 0.0 << std::endl;
	}

	for (auto face = subdivision.fBegin(); face != subdivision.fEnd(); ++face)
	{
		//Constructing face entry
		int numVertexFace = 0;
		std::stringstream stringStream;

		auto sentinelHalfEdge = face->outer;
		auto currentHalfEdge = sentinelHalfEdge;
		do
		{
			stringStream << currentHalfEdge->origin->idx << " ";
			++numVertexFace;
			currentHalfEdge = currentHalfEdge->next;
		} while (currentHalfEdge != sentinelHalfEdge);
		outputStream << numVertexFace << " " << stringStream.str() << std::endl;
	}

	outputStream.close();
}