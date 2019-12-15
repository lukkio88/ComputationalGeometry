#pragma once
#include <iostream>
#include <util.h>

using Float = double;

struct Vector
{
	inline Vector operator+(const Vector& vector) const
	{
		return Vector{ x + vector.x, y + vector.y };
	}
	inline Vector operator-() const
	{
		return Vector{ -x,-y };
	}
	inline Vector operator-(const Vector& vector) const
	{
		return *this + (-vector);
	}
	inline Float operator*(const Vector& vector) const
	{
		return x * vector.x + y * vector.y;
	}
	inline Vector operator/(Float constant) const
	{
		return Vector{ x / constant,y / constant };
	}
	inline Float norm() const
	{
		return sqrt((*this)*(*this));
	}
	inline Float angle() const
	{
		return atan2(y, x);
	}
	inline Vector rotate90() const
	{
		return Vector{ -y,x };
	}
	Float x, y;
};

inline Vector operator*(Float constant, const Vector& vector)
{
	return Vector{ constant*vector.x,constant*vector.y };
}