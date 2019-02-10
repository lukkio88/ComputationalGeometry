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
	bool operator()(const Point& p, const Point & q);
};

Point getMin(const Point & p, const Point & q);
Point getMax(const Point & p, const Point & q);

Point getUp(const Segment &s);
Point getDown(const Segment & s);

struct SegmentComparator {
	bool operator()(const Segment & s, const Segment & r);
	bool * above;
	Float * y;
};

using PriorityQueue = map<Point, vector<Segment>,ComparePts>;
using SweepLine = map<Float, Segment, SegmentComparator>;

vector<Point> computeIntersection(vector<Segment> & S);

#endif