#pragma once
#include <line_seg_intersection.h>

bool ComparePts::operator()(const Point& p, const Point & q) const {
	return p.y - q.y >= ths || ((abs(p.y - q.y) < ths) && p.x < q.x);
}

Point getMin(const Point & p, const Point & q) {
	ComparePts cmp;
	if (cmp(p, q))
		return p;
	return q;
}

Point getMax(const Point & p, const Point & q) {
	ComparePts cmp;
	if (cmp(p, q))
		return q;
	return p;
}

Point getUp(const Segment &s) {
	return getMin(s.p, s.q);
}

Point getDown(const Segment & s) {
	return getMax(s.p, s.q);
}

bool SegmentComparator::operator()(const Segment& s, const Segment& r) const {
	Float xs, xr;
	bool 
		sIsHorizontal = s.isHorizontal(),
		rIsHorizontal = r.isHorizontal();
	if (sIsHorizontal)
		xs = s.p.x;
	else
		s.getX(*y, xs);

	if (rIsHorizontal)
		xr = r.p.x;
	else
		r.getX(*y, xr);

	if (xs != xr)
		return xs < xr;
	else {
		Point u = (sIsHorizontal) ?
			normalize(s.q - s.p):
			normalize(getUp(s) - getDown(s));
		
		Point v = (rIsHorizontal) ?
			normalize(r.q - r.p):
			normalize(getUp(r) - getDown(r));

		Point ref{ 1.0,0.0 };

		if (*above) {
			return u * ref < v*ref;
		}
		else {
			return u * ref > v*ref;
		}

	}
}

SweepLineIter StatusStructure::getIncident(const Point& p) {
	return sweepLine.lower_bound(Segment{ p,p + Point{-1.0,0.0} });
}

std::ostream& operator<<(std::ostream& os, const PriorityQueue& p) {
	for (auto el : p) {
		std::cout << el.first << std::endl;
		for (auto seg : el.second) {
			std::cout << seg << std::endl;
		}
	}
	return os;
}

static int size(const vector<Segment>& U, const vector<Segment>& C, const vector<Segment>& L) {
	return U.size() + C.size() + L.size();
}

static int size(const vector<Segment>& U, const vector<Segment>& C) {
	return U.size() + C.size();
}

static bool findEvent(const Segment& l, const Segment& r, Point& p) {
	return l.intersect(r, p);
}

StatusStructure::StatusStructure() {
	segComp.y = &y_line;
	segComp.above = &above;
	sweepLine = SweepLine(segComp);
	nil = sweepLine.end();
}

bool StatusStructure::findLeftNeighboor(Float x, Segment& sl) const { //This assumes the flag "above" is false
	Segment tmp{ Point{0.0,0.0},Point{1.0,0.0} };
	SweepLineIter it = sweepLine.lower_bound(tmp);
	while (it != nil && (--it) != nil) {
		Float curr_x;
		it->getX(y_line, curr_x);
		if (curr_x != x) {
			sl = *it;
			return true;
		}
	}
	return false;
}

bool StatusStructure::findRightNeighboor(Float x, Segment& sr) const {
	Segment tmp{ Point{0.0,0.0},Point{1.0,0.0} };
	SweepLineIter it = sweepLine.lower_bound(tmp);
	while (it != nil && (++it) != nil) {
		Float curr_x;
		it->getX(y_line, curr_x);
		if (curr_x != x) {
			sr = *it;
			return true;
		}
	}
	return false;
}

/*
This function will find the leftMost and the rightMost in a star of segments
passing through pt
*/
void StatusStructure::findLeftmostAndRightmost(const Point& pt,SweepLineIter& it_l, SweepLineIter& it_r) {
	Float x;
	//Getting the segment whose dot product with e1 is >= to -1 
	it_l = sweepLine.lower_bound({ pt,pt + Point{-1.0,0.0} }); //this will return the actual iterator to the segment, this must exist
	it_r = sweepLine.upper_bound({ pt,pt + Point{1.0,0.0} }); //this potentially might be nil
	it_r--;
}

int n_iteration = 0;

ostream& operator<<(ostream & os, const StatusStructure & tau)
{
	std::string curr_str;
	for (auto& seg : tau.sweepLine)
		os << seg.label << " ";
	return os;
}

vector<Point> computeIntersection(vector<Segment> & S) {
	PriorityQueue queue;
	while (!S.empty()) {
		Segment s = S.back();
		queue[getUp(s)].push_back(s);
		queue[getDown(s)];
		S.pop_back();
	}

	vector<Point> intersections;

	//Init status structure
	StatusStructure tau;
	std::vector<Segment> C, L;
	Float curr_x;
	Segment sl, sr;

	while (!queue.empty()) {

		std::cout << "Status : " << tau << std::endl;

		n_iteration++;

		if (n_iteration == 7) {
			std::cout << "here it is, where this shit crashes..." << std::endl;
		}



		Point p = queue.begin()->first;
		tau.y_line = p.y;
		tau.above = true;
		std::vector<Segment> U = queue.begin()->second;
		queue.erase(queue.begin());

		SweepLineIter it = tau.getIncident(p);

		//populating L and C
		while (it != tau.nil && it->getX(tau.y_line, curr_x) && (abs(curr_x - p.x) < 0.0001)) {
			if (getDown(*it) == p)
				L.push_back(*it);
			else
				C.push_back(*it);
			it = tau.sweepLine.erase(it);
		}

		if (size(U, C, L) > 1)
			intersections.push_back(p);

		while (!L.empty())
			L.pop_back();

		tau.above = false;
		int size_UC = size(U, C);

		while (!U.empty()) {
			tau.sweepLine.insert(U.back());
			U.pop_back();
		}
		while (!C.empty()) {
			tau.sweepLine.insert(C.back());
			C.pop_back();
		}

		std::cout << "After reinsertion : " << tau << std::endl;

		if (size_UC == 0) {
			if (tau.findLeftNeighboor(p.x, sl) && tau.findRightNeighboor(p.x, sr)) {
				Point new_event_pt;
				if (findEvent(sl, sr, new_event_pt))
					queue[new_event_pt];
			}
		}
		else {
			tau.above = true;
			SweepLineIter it_l, it_r, it_ll, it_rr;
			tau.findLeftmostAndRightmost(p, it_l, it_r);
			std::cout << "sl,sr : " << *it_l << "," << *it_r << std::endl;
			it_ll = it_l;
			it_rr = it_r;
			--it_ll;
			++it_rr;

			if (it_ll != tau.nil)
				std::cout << "it_ll : " << *it_ll << " ";
			if (it_rr != tau.nil)
				std::cout << "it_rr : " << *it_rr << " ";
			std::cout << std::endl;

			if (it_ll != tau.nil) {
				Point new_event_pt;
				if (findEvent(*it_ll, *it_l, new_event_pt))
					queue[new_event_pt];
			}
			if (it_rr != tau.nil) {
				Point new_event_pt;
				if (findEvent(*it_r, *it_rr, new_event_pt))
					queue[new_event_pt];
			}
		}
	}

	return intersections;

}