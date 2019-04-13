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

using PriorityQueue = map<Point, vector<Segment>, ComparePts>;
std::ostream& operator<<(std::ostream& os, const PriorityQueue& p);

Point getMin(const Point & p, const Point & q);
Point getMax(const Point & p, const Point & q);

Point getUp(const Segment &s);
Point getDown(const Segment & s);

struct SegmentComparator {
	bool operator()(const Segment & s, const Segment & r) const;
	bool * above;
	Float * y;
};

using SweepLine = set<Segment, SegmentComparator>;
using SweepLineIter = SweepLine::iterator;
constexpr Float ths = 0.0001;

struct StatusStructure {

	StatusStructure();

	SweepLineIter getIncident(const Point& p);
	bool findLeftNeighboor(Float x, Segment& sl) const;
	bool findRightNeighboor(Float x, Segment& sr) const;
	void findLeftmostAndRightmost(const Point& pt, SweepLineIter& it_l, SweepLineIter& it_r);
	
	bool above;
	Float y_line;
	SegmentComparator segComp;
	SweepLine sweepLine;
	SweepLineIter nil;
};

std::ostream& operator<<(std::ostream& os, const StatusStructure& tau);

vector<Point> computeIntersection(vector<Segment> & S);

#endif