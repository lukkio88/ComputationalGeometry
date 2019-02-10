#pragma once
#include <line_seg_intersection.h>

bool ComparePts::operator()(const Point& p, const Point & q) {
	return p.y > q.y || (p.y == q.y && p.x < q.x);
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

bool SegmentComparator::operator()(const Segment & s, const Segment & r) {
	if (getX(s, *y) < getX(r, *y))
		return true;
	else if (getX(s, *y) > getX(r, *y))
		return false;
	else {
		Point v = getUp(s) - getDown(s);
		Point w = getUp(r) - getDown(r);

		double angle_v = v.angle();
		double angle_w = w.angle();

		if (above)
			return angle_v < angle_w;
		else
			return angle_v >= angle_w;
	}
}


using PriorityQueue = map<Point, vector<Segment>, ComparePts>;
using SweepLine = set<Segment, SegmentComparator>;
using SweepLineIter = SweepLine::iterator;

static int size(const vector<Segment>& U, const vector<Segment>& C, const vector<Segment>& L) {
	return U.size() + C.size() + L.size();
}

static bool findEvent(const Segment& l, const Segment& r, Point& p) {
	if (l.does_intersect(r)) {
		p = l.intersect(r);
		return true;
	}
	return false;
}

vector<Point> computeIntersection(vector<Segment> & S) {
	PriorityQueue queue;
	while (!S.empty()) {
		Segment s = S.front();
		queue[getUp(s)].push_back(s);
		queue[getDown(s)];
		S.pop_back();
	}

	vector<Point> intersections;
	Float y_line;
	bool above = true;
	SegmentComparator seg_cmp;
	seg_cmp.y = &y_line;
	seg_cmp.above = &above;
	std::vector<Segment> C, L;
	SweepLine tau(seg_cmp);

	while (!queue.empty()) {
		
		Point p = queue.begin()->first;
		y_line = p.y;
		above = true;
		std::vector<Segment> U = queue.begin()->second;
		queue.erase(queue.begin());

		SweepLineIter it = tau.lower_bound(Segment{ Point{0.0,0.0},Point{-1.0,0.0} });
		//populating L and C
		while (getX(*it,y_line) == p.x) {
			if (getDown(*it) == p)
				L.push_back(*it);
			else
				C.push_back(*it);
			it = tau.erase(it);
		}

		if (size(U, C, L) > 1)
			intersections.push_back(p);

		above = false;
		for (auto seg : U)
			tau.insert(seg);
		for (auto seg : C)
			tau.insert(seg);

	}

}