#include <line_seg_intersection.h>
#include <dcel.h>
#include <vector>

using std::vector;

int test_1(int argc, char** argv) {

	vector<float> val{
		-50.0, -50.0, 50.0, 50.0,
		-50.0, 50.0, 50.0, -50.0,
		-25.0, -50.0, 75.0, 50.0,
		-25.0, 60.0, 25.0, -60.0,
		//-100.0, -50.0, 100.0, 50.0
	};

	vector<std::string> label{
		"s1",
		"s2",
		"s3",
		"s4",
		"s5"
	};

	vector<Segment> list_s;
	for (auto i = 0; i < val.size()/4; ++i) {
		int offset = 4 * i;
		list_s.push_back(Segment({ val[offset],val[offset + 1] }, { val[offset + 2],val[offset + 3]}, label[i]));
	}

	vector<Point> intersection = computeIntersection(list_s);

	for (auto p : intersection) {
		std::cout << p << std::endl;
	}

	return 0;
}

int test_2(int argc, char** argv) {
	return 0;
}

int main(int argc, char** argv) {

	Point p[4]{
		Point{-1.0,-1.0},
		Point{1.0,-1.0},
		Point{1.0,1.0},
		Point{-1.0,1.0}
	};

	DCEL subdivision;

	auto vh0 = subdivision.addVertex(p[0]);
	auto vh1 = subdivision.addVertex(p[1]);
	auto vh2 = subdivision.addVertex(p[2]);
	auto vh3 = subdivision.addVertex(p[3]);

	auto f0 = subdivision.addPoly(std::vector<VertexIter>({ vh0,vh1,vh2 }));
	//auto f1 = subdivision.addPoly(std::vector<VertexIter>({ vh2,vh3,vh0 }));

	return test_1(argc, argv);
}