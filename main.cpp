#include <line_seg_intersection.h>
#include <vector>

using std::vector;

int main(int argc, char** argv) {

	vector<float> val{
		-50.0, -50.0, 50.0, 50.0,
		-50.0, 50.0, 50.0, -50.0,
		-25.0, -50.0, 75.0, 50.0,
		-25.0, 60.0, 25.0, -60.0,
		-100.0, -50.0, 100.0, 50.0
	};

	vector<Segment> list_s;
	for (auto i = 0; i < 5; ++i) {
		int offset = 4 * i;
		list_s.push_back(Segment({ val[offset],val[offset + 1] }, { val[offset + 2],val[offset + 3] }));
	}

	vector<Point> intersection = computeIntersection(list_s);

	for (auto p : intersection) {
		std::cout << p << std::endl;
	}

	return 0;
}