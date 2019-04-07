#pragma once
#include <line_seg_intersection.h>

bool ComparePts::operator()(const Point& p, const Point & q) const {
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

bool SegmentComparator::operator()(const Segment & s, const Segment & r) const {
	Float xs, xr;
	s.getX(*y, xs);
	r.getX(*y, xr);
	if (xs < xr)
		return true;
	else if (xs > xr)
		return false;
	else {
		Point v;
		if (s.p.y != s.q.y)
			v = getUp(s) - getDown(s);
		else
			v = s.q - s.p;

		Point w;
		if (r.p.y != r.q.y)
			w = getUp(r) - getDown(r);
		else
			w = r.q - r.p;

		Float angle_v = v.angle();
		Float angle_w = w.angle();

		if (*above)
			return angle_v > angle_w;
		else
			return angle_v < angle_w;
	}
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

static bool findLeftNeighboor(const SweepLine& tau, Float curr_y, Float x, Segment& sl) { //This assumes the flag "above" is false
	Segment tmp{ Point{0.0,0.0},Point{1.0,0.0} };
	SweepLineIter it = tau.lower_bound(tmp);
	while (--it != tau.end()) {
		Float curr_x;
		it->getX(curr_y, curr_x);
		if (curr_x != x) {
			sl = *it;
			return true;
		}
	}
	return false;
}

static bool findRightNeighboor(const SweepLine& tau, Float curr_y, Float x, Segment& sr) {
	Segment tmp{ Point{0.0,0.0},Point{1.0,0.0} };
	SweepLineIter it = tau.lower_bound(tmp);
	while (++it != tau.end()) {
		Float curr_x;
		it->getX(curr_y, curr_x);
		if (curr_x != x) {
			sr = *it;
			return true;
		}
	}
	return false;
}

static void findLeftmostAndRightmost(const Point& pt,const SweepLine& tau, SweepLineIter& it_l, SweepLineIter& it_r) {
	const Segment s_l{ pt,pt + Point{-1.0,0.0} };
	const Segment s_r{ pt,pt + Point{1.0,0.0} };
	it_l = tau.upper_bound(s_l); //this will return the actual iterator to the segment
	it_r = tau.upper_bound(s_r); //this needs to be decremented
	if (it_r != tau.end())
		--it_r;
	else {
		it_r = (++tau.rbegin()).base();
	}
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
	Float y_line;
	bool above = true;
	SegmentComparator seg_cmp;
	seg_cmp.y = &y_line;
	seg_cmp.above = &above;
	std::vector<Segment> C, L;
	SweepLine tau(seg_cmp);
	Float curr_x;
	Segment sl, sr;

	while (!queue.empty()) {
		
		Point p = queue.begin()->first;
		y_line = p.y;
		above = true;
		std::vector<Segment> U = queue.begin()->second;
		queue.erase(queue.begin());

		SweepLineIter it = tau.upper_bound(Segment{ p, p + Point{-1.0,0.0} }); //this should always exist

		//populating L and C
		if (it != tau.end() && it->getX(y_line, curr_x)) {
			while (curr_x == p.x) {
				if (getDown(*it) == p)
					L.push_back(*it);
				else
					C.push_back(*it);
				it = tau.erase(it);
			}
		}

		if (size(U, C, L) > 1)
			intersections.push_back(p);

		above = false;
		for (auto seg : U)
			tau.insert(seg);
		for (auto seg : C)
			tau.insert(seg);

		if (size(U, C) == 0) {
			if (findLeftNeighboor(tau, y_line, p.x, sl) && findRightNeighboor(tau,y_line,p.x,sr)) {
				Point new_event_pt;
				if(findEvent(sl,sr, new_event_pt))
					queue[new_event_pt];
			}
		}
		else {
			above = true;
			SweepLineIter it_l, it_r, it_ll, it_rr;
			findLeftmostAndRightmost(p, tau, it_l, it_r);
			it_ll = it_l;
			it_rr = it_r;
			--it_ll;
			++it_rr;
			if (it_ll != tau.end()) {
				Point new_event_pt;
				if (findEvent(*it_ll, *it_l, new_event_pt))
					queue[new_event_pt];
			}
			if (it_rr != tau.end()) {
				Point new_event_pt;
				if (findEvent(*it_r, *it_rr, new_event_pt))
					queue[new_event_pt];
			}
		}
	}

	return intersections;

}