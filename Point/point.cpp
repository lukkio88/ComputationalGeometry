#include <point.h>

Point Point::operator-() const
{
	return Point{ -x,-y };
}

bool Point::operator==(const Point & p) const
{
	return x == p.x && y == p.y;
}

bool Point::operator!=(const Point & p) const
{
	return !(*this == p);
}

Point Point::operator+(const Point & p) const
{
	return Point{ x + p.x, y + p.y };
}

Point Point::operator-(const Point & p) const
{
	return Point{ x - p.x,y - p.y };
}

Float Point::operator*(const Point & p) const
{
	return x*p.x + y*p.y;
}

Float Point::angle() const
{
	return atan2(y,x);
}

Float Point::distOrigin() const
{
	return sqrt((*this)*(*this));
}

Position Point::classify(const Point & p, const Point & q) const
{
	if (left(p, q)) return LEFT;
	if (right(p, q)) return RIGHT;
	if (front(p, q)) return FRONT;
	if (back(p, q)) return BACK;
	if (between(p, q)) return INSIDE;
}

bool Point::left(const Point& a, const Point& b) const {
	const Point & c = *this;
	return twiceArea(a, b, c) > 0.0;
}

bool Point::right(const Point& a, const Point& b)  const {
	const Point & c = *this;
	return twiceArea(a, b, c) < 0.0;
}

bool Point::collinear(const Point& a, const Point& b) const {
	const Point & c = *this;
	return twiceArea(a, b, c) == 0.0;
}

bool Point::front(const Point & a, const Point & b) const
{
	if (collinear(a, b)) {
		Point u = *this - a;
		Point v = b - a;
		if (v.x != 0.0) //v isn't vertical
			return abs(u.x) > abs(v.x);
		else
			return abs(u.y) > abs(v.y);
	}
	return false;
}

bool Point::back(const Point & a, const Point & b) const
{
	if (collinear(a, b)) {
		Point u = *this - a;
		Point v = b - a;
		if (v.x != 0.0) //v isn't vertical
			return u.x < 0.0 && v.x > 0.0 || u.x > 0.0 && v.x < 0.0;
		else
			return u.y < 0.0 && v.y > 0.0 || u.y > 0.0 && v.y < 0.0;;
	}
	return false;
}

bool Point::between(const Point & a, const Point & b) const
{
	if (collinear(a, b)) {
		Point u = *this - a;
		Point v = b - a;
		return !back(a, b) && !front(a, b);
	}
	else
		return false;
}

/*
Test if r against the segment pq,
	this implies construct the segment pr and pq and compute the determinant
*/
Float twiceArea(const Point & p, const Point & q, const Point & r)
{
	const Point v = q - p;
	const Point w = r - p;
	return det2x2(v.x,v.y,w.x,w.y);
}

Point rotate90(const Point & p)
{
	return Point{ p.y,-p.x };
}

double l2dist(const Point & p, const Point & q)
{
	Point w = p - q;
	return sqrt(w*w);
}

Point operator*(Float alpha, const Point & p)
{
	return Point{ alpha*p.x,alpha*p.y };
}

Point normalize(const Point & p)
{
	Float norm = p.distOrigin();
	return Point{ p.x / norm,p.y / norm };
}


std::ostream& operator<<(std::ostream& os, const Point& p) {
	return (os << "(" << p.x << " ," << p.y << ")");
}