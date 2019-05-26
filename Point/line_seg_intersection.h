#pragma once
#ifndef __LINE_SEG_INTERSECTION_H
#define __LINE_SEG_INTERSECTION_H
#include <segment.h>
#include <map>
#include <set>
#include <vector>

using std::map;
using std::set;
using std::pair;
using std::vector;

struct ComparePts {
	bool operator()(const Point& p, const Point & q) const;
};

template<typename SegmentType>
using PriorityQueue = map<Point, vector<SegmentType>, ComparePts>;

template<typename SegmentType>
std::ostream& operator<<(std::ostream& os, const PriorityQueue<SegmentType>& p);

Point getMin(const Point & p, const Point & q);
Point getMax(const Point & p, const Point & q);

template<typename SegmentType>
inline Point getUp(const SegmentType &s) {
	return getMin(s.p, s.q);
}

template<typename SegmentType>
inline Point getDown(const SegmentType& s) {
	return getMax(s.p, s.q);
}

template<typename SegmentType>
inline int size(const vector<SegmentType>& U, const vector<SegmentType>& C, const vector<SegmentType>& L) {
	return U.size() + C.size() + L.size();
}

template<typename SegmentType>
inline int size(const vector<SegmentType>& U, const vector<SegmentType>& C) {
	return U.size() + C.size();
}

template<typename SegmentType>
inline bool findEvent(const SegmentType& l, const SegmentType& r, Point& p) {
	return l.intersect(r, p);
}

template<typename SegmentType>
struct SegmentComparator {
	bool operator()(const SegmentType & s, const SegmentType & r) const;
	bool * above;
	Float * y;
};

template<typename SegmentType>
inline bool SegmentComparator<SegmentType>::operator()(const SegmentType& s, const SegmentType& r) const {
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
			normalize(s.q - s.p) :
			normalize(getUp(s) - getDown(s));

		Point v = (rIsHorizontal) ?
			normalize(r.q - r.p) :
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

template<typename SegmentType>
using SweepLine = typename set<SegmentType, SegmentComparator<SegmentType>>;

template<typename SegmentType>
using SweepLineIter = typename SweepLine<SegmentType>::iterator;

constexpr Float ths = 0.0001;

template<typename SegmentType>
struct StatusStructure {

	StatusStructure();

	SweepLineIter<SegmentType> getIncident(const Point& p);
	bool findLeftNeighboor(Float x, SegmentType& sl) const;
	bool findRightNeighboor(Float x, SegmentType& sr) const;
	void findLeftmostAndRightmost(const Point& pt, SweepLineIter<SegmentType>& it_l, SweepLineIter<SegmentType>& it_r);
	
	bool above;
	Float y_line;
	SegmentComparator<SegmentType> segComp;
	SweepLine<SegmentType> sweepLine;
	SweepLineIter<SegmentType> nil;
};

template<typename SegmentType>
inline StatusStructure<SegmentType>::StatusStructure() {
	segComp.y = &y_line;
	segComp.above = &above;
	sweepLine = SweepLine<SegmentType>(segComp);
	nil = sweepLine.end();
}

template<typename SegmentType>
inline SweepLineIter<SegmentType> StatusStructure<SegmentType>::getIncident(const Point& p) {
	return sweepLine.lower_bound(SegmentType{ p,p + Point{ -1.0,0.0 } });
}

template<typename SegmentType>
inline std::ostream& operator<<(std::ostream& os, const PriorityQueue<SegmentType>& p) {
	for (auto el : p) {
		std::cout << el.first << std::endl;
		for (auto seg : el.second) {
			std::cout << seg << std::endl;
		}
	}
	return os;
}

template<typename SegmentType>
inline bool StatusStructure<SegmentType>::findLeftNeighboor(Float x, SegmentType& sl) const { //This assumes the flag "above" is false
	SegmentType tmp{ Point{ 0.0,0.0 },Point{ 1.0,0.0 } };
	SweepLineIter<SegmentType> it = sweepLine.lower_bound(tmp);
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

template<typename SegmentType>
inline bool StatusStructure<SegmentType>::findRightNeighboor(Float x, SegmentType& sr) const {
	SegmentType tmp{ Point{ 0.0,0.0 },Point{ 1.0,0.0 } };
	SweepLineIter<SegmentType> it = sweepLine.lower_bound(tmp);
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
template<typename SegmentType>
inline void StatusStructure<SegmentType>::findLeftmostAndRightmost(const Point& pt, SweepLineIter<SegmentType>& it_l, SweepLineIter<SegmentType>& it_r) {
	Float x;
	//Getting the segment whose dot product with e1 is >= to -1 
	it_l = sweepLine.lower_bound({ pt,pt + Point{ -1.0,0.0 } }); //this will return the actual iterator to the segment, this must exist
	it_r = sweepLine.upper_bound({ pt,pt + Point{ 1.0,0.0 } }); //this potentially might be nil
	it_r--;
}

template<typename SegmentType>
inline std::ostream& operator<<(std::ostream& os, const StatusStructure<SegmentType>& tau)
{
	std::string curr_str;
	for (auto& seg : tau.sweepLine)
		os << seg.label << " ";
	return os;
}

template<typename SegmentType>
vector<Point> computeIntersection(vector<SegmentType> & S) {
	PriorityQueue<SegmentType> queue;
	while (!S.empty()) {
		SegmentType s = S.back();
		queue[getUp(s)].push_back(s);
		queue[getDown(s)];
		S.pop_back();
	}

	vector<Point> intersections;

	//Init status structure
	StatusStructure<SegmentType> tau;
	std::vector<SegmentType> C, L;
	Float curr_x;
	SegmentType sl, sr;

	while (!queue.empty()) {

		Point p = queue.begin()->first;
		tau.y_line = p.y;
		tau.above = true;
		std::vector<SegmentType> U = queue.begin()->second;
		queue.erase(queue.begin());

		SweepLineIter<SegmentType> it = tau.getIncident(p);

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

		tau.above = false;
		int size_UC = size(U, C);

		tau.sweepLine.insert(U.begin(), U.end());
		tau.sweepLine.insert(C.begin(), C.end());

		L.clear();
		U.clear();
		C.clear();

		if (size_UC == 0) {
			if (tau.findLeftNeighboor(p.x, sl) && tau.findRightNeighboor(p.x, sr)) {
				Point new_event_pt;
				if (findEvent(sl, sr, new_event_pt))
					queue[new_event_pt];
			}
		}
		else {
			tau.above = true;
			SweepLineIter<SegmentType> it_l, it_r, it_ll, it_rr;
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

#endif