#pragma once
#ifndef __SEGMENT_H
#define __SEGMENT_H
#include <point2D.h>
#include <string>

using std::ostream;

class Segment {
public:
	Segment();
	Segment(const Point& a, const Point& b);
	Segment(const Point& a, const Point& b, std::string label);
	Float length() const;
	void swap_pts();
	Position classify(const Point& pt) const;
	bool intersect(const Segment& r, Point& pt) const;
	bool getX(Float y, Float& x) const;
	bool getPt(Float y, Point& pt) const;
	bool isHorizontal() const;
private:
	bool does_intersect(const Segment& r) const;
	friend ostream& operator<<(ostream& os, const Segment& s);
public:
	std::string label;
	Point p; //origin
	Point q; //destination
};

#endif