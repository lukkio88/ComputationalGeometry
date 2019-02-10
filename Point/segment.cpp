#include <segment.h>

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

Position Segment::classify(const Point & p) const
{
	
}

bool Segment::does_intersect(const Segment & r) const
{
	const Segment & s = *this;
	const Point & a = s.p;
	const Point & b = s.q;
	const Point & c = r.p;
	const Point & d = r.q;
	
	Float a2_abc = twiceArea(a, b, c);
	Float a2_abd = twiceArea(a, b, d);
	Float a2_cda = twiceArea(c, d, a);
	Float a2_cdb = twiceArea(c, d, b);

	if (a2_abc <= 0.0 && a2_abd >= 0.0 && a2_cda <= 0.0 && a2_cdb >= 0.0)
		return true;
	else
		return false;
}

Point Segment::intersect(const Segment & r) const
{
	const Segment & s = *this;
	const Point & p = s.p -r.p;
	const Point & v = r.q - r.p;
	const Point & w = s.p - s.q;

	Float alpha = p * rotate90(v)/(w*rotate90(v)); //is segments intersect then 0.0 <= alpha <= 1.0
	return s.p - alpha * w;
}

Float getX(const Segment & s, Float y)
{
	Float alpha = (y - s.p.y) / (s.q.y - s.p.y);
	return s.p.x + alpha * (s.q.x - s.p.x);
}

Point getPt(const Segment & s, Float y)
{
	Float alpha = (y - s.p.y) / (s.q.y - s.p.y);
	return s.p + alpha * (s.q - s.p);
}
