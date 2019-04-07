#include <util.h>

double det2x2(double a, double b, double c, double d) {
	return a * d - b * c;
}

double det2x2(double v[]) {
	return v[0] * v[2] - v[1] * v[3];
}