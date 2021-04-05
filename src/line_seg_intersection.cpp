#pragma once
#include <line_seg_intersection.h>
#include <limits>

bool ComparePts::operator()(const Point& p, const Point & q) const {
	return p.y - q.y >= THRESHOLD || ((abs(p.y - q.y) < THRESHOLD) && p.x < q.x);
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
	return getMin(s.origin, s.destination);
}

Point getDown(const Segment & s) {
	return getMax(s.origin, s.destination);
}

bool SegmentComparator::operator()(const Segment& s, const Segment& r) const {
	float xs, xr;
	s.getX(*y, xs);
	r.getX(*y, xr);

	if (std::fabs(xs - xr) >= THRESHOLD)
		return xs < xr;
	else {
		float angleS = atan2(getUp(s).y - getDown(s).y, getUp(s).x - getDown(s).x);
		float angleR = atan2(getUp(r).y - getDown(r).y, getUp(r).x - getDown(r).x);

		return (*above) ? (angleS > angleR) : (angleS <= angleR);
	}
}

SweepLineIter StatusStructure::getIncident(const Point& p) {
	return sweepLine.lower_bound(Segment{ p,p + Point{-1.0,0.0} });
}

void StatusStructure::insertSegment(const Segment& s)
{
	//Compute the angle first
	Point up = getUp(s);
	Point down = getDown(s);

	float deltaY = up.y - down.y;
	float deltaX = up.x - down.x;

	float angle = atan2(deltaY, deltaX); //observe deltaY > 0, strictly
	angleUpperBound = std::max(angleUpperBound, 0.5f*(angle + acosf(-1.0f)));
	angleLowerBound = std::min(angleLowerBound, 0.5f*angle);

	sweepLine.insert(s);
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
	return static_cast<int>(U.size() + C.size() + L.size());
}

static int size(const vector<Segment>& U, const vector<Segment>& C) {
	return static_cast<int>(U.size() + C.size());
}

static bool findEvent(const Segment& l, const Segment& r, Point& p) {
	return l.intersect(r, p);
}

StatusStructure::StatusStructure() {
	segComp.y = &y_line;
	segComp.above = &above;
	sweepLine = SweepLine(segComp);
	nil = sweepLine.end();
	angleUpperBound = std::numeric_limits<float>::min();
	angleLowerBound = std::numeric_limits<float>::max();

}

bool StatusStructure::findLeftNeighboor(float x, Segment& sl) const { //This assumes the flag "above" is false
	Segment tmp{ Point{0.0,0.0},Point{1.0,0.0} };
	SweepLineIter it = sweepLine.lower_bound(tmp);
	while (it != nil && (--it) != nil) {
		float curr_x;
		it->getX(y_line, curr_x);
		if (curr_x != x) {
			sl = *it;
			return true;
		}
	}
	return false;
}

bool StatusStructure::findRightNeighboor(float x, Segment& sr) const {
	Segment tmp{ Point{0.0,0.0},Point{1.0,0.0} };
	SweepLineIter it = sweepLine.lower_bound(tmp);
	while (it != nil && (++it) != nil) {
		float curr_x;
		it->getX(y_line, curr_x);
		if (curr_x != x) {
			sr = *it;
			return true;
		}
	}
	return false;
}

void StatusStructure::findLeftmostAndRightmost(const Point& pt,SweepLineIter& it_l, SweepLineIter& it_r) {
	//Getting the segment whose dot product with e1 is >= to -1 
	it_l = sweepLine.lower_bound({ pt,pt + Point{cosf(angleUpperBound),sinf(angleUpperBound)} }); //this will return the actual iterator to the segment, this must exist
	it_r = sweepLine.upper_bound({ pt,pt + Point{cosf(angleLowerBound),sinf(angleLowerBound)} }); //this potentially might be nil
	it_r--;
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
	float curr_x;
	Segment sl, sr;

	while (!queue.empty()) {

		Point p = queue.begin()->first;
		tau.y_line = p.y;
		tau.above = true;
		std::vector<Segment> U = queue.begin()->second;
		queue.erase(queue.begin());

		SweepLineIter it = tau.getIncident(p);

		//populating L and C
		while (it != tau.nil && it->getX(tau.y_line, curr_x) && (abs(curr_x - p.x) < THRESHOLD)) {
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
			tau.insertSegment(U.back());
			U.pop_back();
		}
		while (!C.empty()) {
			tau.insertSegment(C.back());
			C.pop_back();
		}

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
			it_ll = it_l;
			it_rr = it_r;
			--it_ll;
			++it_rr;

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