#include <segment.h>

Segment::Segment()
{
	p = Point{ 0.0,0.0 };
	q = Point{ 0.0,0.0 };
}

Segment::Segment(const Point & a, const Point & b)
{
	p = a;
	q = b;
}

Float Segment::length() const
{
	return l2dist(p,q);
}

void Segment::swap_pts()
{
	Point tmp = p;
	p = q;
	q = tmp;
}

Position Segment::classify(const Point & pt) const
{
	return pt.classify(p, q);
}

bool Segment::does_intersect(const Segment & r) const
{
	const Segment & s = *this;
	const Point & a = s.p;
	const Point & b = s.q;
	const Point & c = r.p;
	const Point & d = r.q;

	return (twiceArea(c, d, a)*twiceArea(c, d, b) <= 0.0) && (twiceArea(a, b, c)*twiceArea(a, b, d) <= 0.0);

	//return ((a.classify(c, d) == LEFT) && (b.classify(c, d) == RIGHT) && (c.classify(a, b) == LEFT) && (d.classify(a, b) == RIGHT));
}

bool Segment::intersect(const Segment& r, Point& pt) const
{
	if (!does_intersect(r))
		return false;

	const Segment & s = *this;
	const Point & p = s.p -r.p;
	const Point & v = r.q - r.p;
	const Point & w = s.p - s.q;

	Float alpha = p * rotate90(v)/(w*rotate90(v)); //formula comes from linear algebra
	pt = s.p - alpha * w;
	return true;
}

bool Segment::getX(Float y, Float& x) const
{
	if (p.y == q.y && q.y == y) { //pick origin in this case of the segment in this case
		x = p.x;
		return true;
	}
	else {
		Point pt;
		bool inside = getPt(y, pt);
		x = pt.x;
		return inside;
	}
}

bool Segment::getPt(Float y, Point& pt) const
{
	Float alpha = (y - p.y) / (q.y - p.y);
	pt = p + alpha * (q - p);
	if (classify(pt) == INSIDE)
		return true;
	return false;
}

bool Segment::isHorizontal() const {
	return p.y == q.y;
}

std::ostream& operator<<(std::ostream& os, const Segment& s) {
	return (os << "{ " << s.p << " ," << s.q << "}");
}