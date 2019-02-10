#ifndef __POINT_H
#define __POINT_H
#include <iostream>
#include <util.h>

using Float = double;
using UInt = uint64_t;
using Int = int64_t;

enum Position {
	LEFT, RIGHT, FRONT, BACK, INSIDE, INTERSECT
};

struct Point {
	Point operator-() const;
	bool operator==(const Point& p) const;
	bool operator!=(const Point& p) const;
	Point operator+(const Point& p) const;
	Point operator-(const Point& p) const;
	Float operator*(const Point& p) const;
	Float angle() const;
	Float distOrigin() const;
	Float x, y;
};

Float twiceArea(const Point& p, const Point& q, const Point& r);
Position classify(const Point& p, const Point& q, const Point& r);
Point rotate90(const Point& p);
Float l2dist(const Point& p, const Point& q);
Point operator*(Float alpha, const Point& v);

#endif