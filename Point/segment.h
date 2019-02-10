#pragma once
#ifndef __SEGMENT_H
#define __SEGMENT_H
#include <point2D.h>

struct Segment {
	Segment(const Point& a, const Point& b);
	Float length() const;
	void swap_pts();
	Position classify(const Point& p) const;
	bool does_intersect(const Segment& r) const;
	Point intersect(const Segment& r) const;
	Point p, q;
};

Float getX(const Segment& s, Float y);
Point getPt(const Segment& s, Float y);

#endif