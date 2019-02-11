#ifndef __POINT_H
#define __POINT_H
#include <iostream>
#include <util.h>

using Float = double;
using UInt = uint64_t;
using Int = int64_t;

enum Position {
	LEFT, RIGHT, FRONT, BACK, INSIDE
};

class Point {
public:
	Point operator-() const;
	bool operator==(const Point& p) const;
	bool operator!=(const Point& p) const;
	Point operator+(const Point& p) const;
	Point operator-(const Point& p) const;
	Float operator*(const Point& p) const;
	Float angle() const;
	Float distOrigin() const;
	Position classify(const Point& p, const Point& q) const;
private:
	bool left(const Point& a, const Point& b) const;
	bool right(const Point& a, const Point& b) const;
	bool collinear(const Point& a, const Point& b) const;
	bool front(const Point& a, const Point& b) const;
	bool back(const Point& a, const Point& b) const;
	bool between(const Point& a, const Point& b) const;
public:
	Float x, y;
};

Float twiceArea(const Point& p, const Point& q, const Point& r);
Point rotate90(const Point& p);
Float l2dist(const Point& p, const Point& q);
Point operator*(Float alpha, const Point& v);

#endif