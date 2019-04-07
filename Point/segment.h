#pragma once
#ifndef __SEGMENT_H
#define __SEGMENT_H
#include <point2D.h>

class Segment {
public:
	Segment();
	Segment(const Point& a, const Point& b);
	Float length() const;
	void swap_pts();
	Position classify(const Point& pt) const;
	bool intersect(const Segment& r, Point& pt) const;
	bool getX(Float y, Float& x) const;
	bool getPt(Float y, Point& pt) const;
	bool isHorizontal() const;
private:
	bool does_intersect(const Segment& r) const;
public:
	Point p; //origin
	Point q; //destination
};

std::ostream& operator<<(std::ostream& os, const Segment& s);

#endif