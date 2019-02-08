#include <point2D.h>

Point Point::operator-() const
{
	return Point{ -x,-y };
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

Float twiceArea(const Point & p, const Point & q, const Point & r)
{
	const Point v = q - p;
	const Point w = r - p;
	return det2x2(v.x,v.y,w.x,w.y);
}

Position classify(const Point & p, const Point & q, const Point& r)
{
	Float det = twiceArea(p,q,r);
	if (det > 0.0) return LEFT;
	if (det < 0.0) return RIGHT;
	
	Point v = q - p;
	Point w = r - p;

	Int vx = *reinterpret_cast<Int*>(&v.x);
	Int vy = *reinterpret_cast<Int*>(&v.y);
	Int wx = *reinterpret_cast<Int*>(&w.x);
	Int wy = *reinterpret_cast<Int*>(&w.y);

	if ((vx^wx) & (vy^wy))
		return BACK;
	Float dist_v = v.distOrigin();
	Float dist_w = w.distOrigin();
	if (dist_v <= dist_w)
		return INSIDE;
	return FRONT;

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
