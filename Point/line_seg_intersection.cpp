#pragma once
#include <line_seg_intersection.h>

bool ComparePts::operator()(const Point& p, const Point & q) const {
	return p.y - q.y >= ths || ((abs(p.y - q.y) < ths) && p.x < q.x);
}

Point getMin(const Point & p, const Point & q) {
	ComparePts cmp;
	if (cmp(p, q))
		return p;
	return q;
}

Point getMax(const Point & p, const Point & q) {
	ComparePts cmp;
	if (cmp(p, q))
		return q;
	return p;
}