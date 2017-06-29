/*
 * vector2d.cpp
 *
 *  Created on: 01/03/2015
 *      Author: Carlos Faruolo <5carlosfelipe5@gmail.com>
 */

#include "vector2d.hpp"

#include <cmath>
#include <string>

#include "futil/math/constants.h"
#include "futil/math/more_random.h"
#include "futil/string/more_operators.hpp"

using std::string;

/** Represents the null/zero vector. It has coordinates (0, 0). */
const Vector2D Vector2D::NULL_VECTOR (0, 0);

/** A vector codirectional with the X axis, with length 1. */
const Vector2D Vector2D::X_VERSOR (1, 0);

/** A vector codirectional with the Y axis, with length 1. */
const Vector2D Vector2D::Y_VERSOR (0, 1);

Vector2D::Vector2D(double magnitude)
: x(), y()
{
	double angle = random_decimal_between(0.0, 2.0*M_PI);
	x = magnitude*cos(angle);
	y = magnitude*sin(angle);
}

// ------ equality & utils

bool Vector2D::operator ==(const Vector2D& v) const
{
	return v.x == x and v.y == y;
}

bool Vector2D::operator !=(const Vector2D& v) const
{
	return v.x != x or v.y != y;
}

/** Returns true if this vector coordinates equals the ones from the given vector */
bool Vector2D::equals(const Vector2D& vector) const
{
	return (*this) == vector;
}

/** Creates and returns a copy of this vector via copy constructor */
Vector2D Vector2D::clone() const
{
	return Vector2D(x, y);
}

// ------- utils

/** Creates a string with this vector coordinates (x, y) */
string Vector2D::toString() const
{
	return string() + "(" + x + ", " + y + ")";
}

/** Creates and returns an array with this Vectors coordinates, in correct order.  */
double* Vector2D::getCoordinates() const
{
	double* coord = new double[2];
	coord[0] = x;
	coord[1] = y;
	return coord;
}

// ------- magnitude/length

double Vector2D::operator ~() const
{
	return sqrt( x*x + y*y );
}

/** Returns the length/magnitude of this vector. */
double Vector2D::magnitude() const
{
	return ~(*this);
}

// ------- normalization

Vector2D Vector2D::operator !() const
{
	return isZero()? Vector2D() : Vector2D(x/magnitude(), y/magnitude());
}

/** Creates a vector with length 1 and same direction as this vector. In other words, a new vector that is a normalized version of this vector. Note that <b>the original vector remains unchanged</b>. */
Vector2D Vector2D::unit() const
{
	return !(*this);
}

/** Divides this vector's coordinates by its length/magnitude, normalizing it.
The returned object is the vector instance itself after normalization. */
Vector2D& Vector2D::normalize()
{
	if(not isZero())
	{
		double length = magnitude();
		if(length > 0)
		{
			x /= length;
			y /= length;
		}
	}
	return *this;
}

// ------- reflection

Vector2D Vector2D::operator -() const
{
	return Vector2D(-x, -y);
}

/** Creates and returns the opposite of this vector. In other words, returns a vector with same coordinates as this, but with changed signal. Note that <b>the original vector remains unchanged</b>. */
Vector2D Vector2D::opposite() const
{
	return -(*this);
}

Vector2D& Vector2D::operator --()
{
	x = -x; y = -y;
	return *this;
}

/** Changes the signal of this vector coordinates, effectively reflecting it.
<br> The returned object is <b>the vector instance itself</b> after reflection. */
Vector2D& Vector2D::reflect()
{
	return --(*this);
}

Vector2D& Vector2D::reflectX()
{
	x = -x;
	return *this;
}

Vector2D& Vector2D::reflectY()
{
	y = -y;
	return *this;
}

// ------- basic arithmetic

Vector2D Vector2D::operator +(const Vector2D& v) const
{
	return Vector2D(x + v.x, y + v.y);
}

/** Creates and returns a vector that represents the sum of this vector and the given vector. Note that <b>the original vector remains unchanged</b>.*/
Vector2D Vector2D::sum(const Vector2D& v) const
{
	return *this + v;
}

Vector2D& Vector2D::operator +=(const Vector2D& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

/** Adds to this vector the given vector. In other words, it performs an addition to this vector coordinates.
<br> The returned object is <b>the vector instance itself</b> after summation. */
Vector2D& Vector2D::add(const Vector2D& v)
{
	(*this) += v;
	return *this;
}

Vector2D Vector2D::operator -(const Vector2D& v) const
{
	return Vector2D(x - v.x, y - v.y);
}

/** Creates a vector that represents the difference/displacement of this vector and the given vector, in this order. It's useful to remember that vector subtraction is <b>not commutative</b>: a-b != b-a.
<br> Note that <b>the original vector remains unchanged</b>. */
Vector2D Vector2D::difference(const Vector2D& v) const
{
	return  (*this) - v;
}

Vector2D& Vector2D::operator -=(const Vector2D& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

/** Subtracts from this vector the given vector. In other words, it performs an subtraction to this vector coordinates.
It's useful to remember that vector subtraction is <b>not commutative</b>: a-b != b-a.
<br> The returned object is the <b>the vector instance itself</b> after subtraction. */
Vector2D& Vector2D::subtract(const Vector2D& v)
{
	(*this) -= v;
	return *this;
}

Vector2D Vector2D::operator *(const double& factor) const
{
	return Vector2D(x * factor, y * factor);
}

/** Creates a vector that represents the scalar multiplication of this vector by the given factor. Note that <b>the original vector remains unchanged</b>.*/
Vector2D Vector2D::times(const double factor) const
{
	return (*this) * factor;
}

Vector2D& Vector2D::operator *=(const double& factor)
{
	x *= factor;
	y *= factor;
	return *this;
}

/** Multiply this vectors coordinates by the given factor. The returned object is <b>the vector instance itself</b> after multiplication.*/
Vector2D& Vector2D::scale(double factor)
{
	(*this) *= factor;
	return *this;
}

// ------- miscellaneous operations

double Vector2D::operator %(const Vector2D& v) const
{
	return ~((*this) - v);
}

/** Compute the distance between this vector and the given vector. In other words, returns the length/magnitude of the displacement between this and the given vector. */
double Vector2D::distance(const Vector2D& v) const
{
	return (*this)%v;
}

double Vector2D::operator ^(const Vector2D& v) const
{
	return x*v.x + y*v.y;
}

/** Compute the inner/dot product between this and the given vector. */
double Vector2D::innerProduct(const Vector2D& v) const
{
	return (*this) ^ v;
}

// ------- projection operations (XXX not tested)

Vector2D Vector2D::operator ||(const Vector2D& v) const  // FIXME projection operator is incorrect, not passing tests
{
	return (*this) * (((*this)^v)/(v^v));
}

/* Creates a vector that represents the projection of this vector on the given vector v. */
Vector2D Vector2D::projection(const Vector2D& v) const
{
	return (*this) || v;
}

/* Creates a vector that represents the rejection of this vector on the given vector v. The rejection is defined as rej(u, v) = u - proj(u, v) */
Vector2D Vector2D::rejection(const Vector2D& v) const
{
	return (*this) - ((*this) || v);
}

Vector2D Vector2D::operator |(const Vector2D& v) const
{
	return (*this) - (this->rejection(v)*2);
}

/* Creates a vector that represents the reflection of this vector in the axis represented by the given vector v. */
Vector2D Vector2D::reflection(const Vector2D& v) const
{
	return (*this)|v;
}

// ------- rotation operations

Vector2D Vector2D::operator <(const double& radians) const
{
	return Vector2D(x*cos(radians) - y*sin(radians), x*sin(radians) + y*cos(radians));
}

Vector2D Vector2D::rotation(const double& radians) const
{
	return (*this) < radians;
}

Vector2D& Vector2D::operator <<(const double& radians)
{
	double
	nx = x*cos(radians) - y*sin(radians),
	ny = x*sin(radians) + y*cos(radians);
	x = nx; y = ny;
	return *this;
}

Vector2D& Vector2D::rotate(const double& radians)
{
	return (*this) << radians;
}

Vector2D Vector2D::perpendicular() const
{
	return (*this) < M_PI_2; // rotate by pi/2 radians
}

