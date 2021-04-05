#include <line_seg_intersection.h>
#include <dcelIO.h>
#include <vector>

using std::vector;

int test_1(int argc, char** argv) {

	vector<float> val{
		-50.0, -50.0, 50.0, 50.0,
		-50.0, 50.0, 50.0, -50.0,
		25.0, 50.0, 25.0, -50.0
		-25.0, 50.0, -25.0, -50.0,
		0.0, 50.0, 0.0, -50.0,
		-50.0, -50.0, 0.0, -50.0
	};

	vector<Segment> list_s;
	for (auto i = 0; i < val.size()/4; ++i) {
		int offset = 4 * i;
		list_s.push_back(Segment{ { val[offset],val[offset + 1] }, { val[offset + 2],val[offset + 3]} });
	}

	vector<Point> intersection = computeIntersection(list_s);

	for (auto p : intersection) {
		std::cout << p << std::endl;
	}

	return 0;
}

static void print_inner_boundary(FaceIter f)
{
	std::cout << "Printing inner boundary : " << std::endl;
	auto he_sentinel = f->outer;
	auto curr_he = he_sentinel;
	do
	{
		std::cout << curr_he->origin->idx << " ";
		curr_he = curr_he->next;
	} while (curr_he != he_sentinel);
	std::cout << std::endl;
}

static void print_outer_boundary(FaceIter f)
{
	std::cout << "Printing outer boundary! : " << std::endl;
	auto he_sentinel = f->outer->twin;
	auto curr_he = he_sentinel;
	do
	{
		std::cout << curr_he->origin->idx << " ";
		curr_he = curr_he->next;
	} while (curr_he != he_sentinel);
	std::cout << std::endl;
}

int main(int argc, char** argv) {

	test_1(argc,argv);

	/*std::vector<Point> point{
		{ 1.0,0.0 },
		{ 0.0,0.0 },
		{-1.0,0.0},
		{-0.5,1.0},
		{0.5,1.0},
		{-0.5,-1.0},
		{0.5,-1.0},
		{0.0,-2.0}
	};

	std::vector<VertexIter> vh(point.size());

	DCEL subdivision;

	for (int i = 0; i < point.size(); ++i)
	{
		vh[i] = subdivision.addVertex(point[i]);
	}

	std::cout << "Insert f0" << std::endl;
	auto f0 = subdivision.addPoly(std::vector<VertexIter>({ vh[0],vh[4],vh[1] }));
	print_outer_boundary(f0);

	std::cout << "Insert f1" << std::endl;
	auto f1 = subdivision.addPoly(std::vector<VertexIter>({ vh[1],vh[3],vh[2] }));
	print_outer_boundary(f1);

	std::cout << "Insert f2" << std::endl;
	auto f2 = subdivision.addPoly(std::vector<VertexIter>({ vh[6],vh[1],vh[5] }));
	print_outer_boundary(f2);

	std::cout << "Insert f3" << std::endl;
	auto f3 = subdivision.addPoly(std::vector<VertexIter>({ vh[0],vh[1],vh[2],vh[7] }));
	print_outer_boundary(f3);

	auto he1 = subdivision.getHalfEdge(vh[1], vh[2]);
	auto he2 = subdivision.getHalfEdge(vh[7], vh[0]);
	subdivision.join(he1, he2);

	dumpPly(subdivision, "test.ply");

	//Iterating through all the faces and printing all the indices
	*/
	return 0;

}