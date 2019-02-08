#pragma once
#include <point2D.h>

struct Segment {
	Segment(const Point& a, const Point& b);
	Float length() const;
	void swap_pts();
	Position classify(const Point& p) const;
	bool intersect(const Segment& r) const;
	Point p, q;
};