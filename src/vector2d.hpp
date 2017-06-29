/*
 * vector2d.hpp
 *
 *  Created on: 25/06/2014
 *      Author: Carlos Faruolo <5carlosfelipe5@gmail.com>
 */

#ifndef VECTOR2D_HPP_
#define VECTOR2D_HPP_
#include <ciso646>

#include <string>

struct Vector2D
{
	double x, y;

	// ------ constructors

	/** Creates a Vector2D with the given coordinates */
	Vector2D(double x, double y)
	: x(x), y(y) {}

	/** Creates a Vector2D with the (0, 0) coordinates */
	Vector2D()
	: x(0), y(0) {}

	/** Copy constructor */
	Vector2D(const Vector2D& v)
	: x(v.x), y(v.y) {}

	/** Creates a vector with the given magnitude and random direction. */
	Vector2D(double magnitude);

	// ------ equality

	bool operator ==(const Vector2D& v) const;
	bool operator !=(const Vector2D& v) const;

	/** Returns true if this vector coordinates equals the ones from the given vector */
	bool equals(const Vector2D& v) const;

	/** Creates and returns a copy of this vector via copy constructor */
	Vector2D clone() const;

	// ------- utils

	/** Creates a string with this vector coordinates (x, y) */
	std::string toString() const;

	/** Creates and returns an array with this Vectors coordinates, in correct order.  */
	double* getCoordinates() const;

	// ------- magnitude/length

	double operator ~() const;

	/** Returns the length/magnitude of this vector. */
	double magnitude() const;

	/** \see magnitude() */
	inline double length() const { return magnitude(); }

	/** Return true if x = 0 and y = 0; */
	inline bool isZero() const
	{
		return x==0 and y==0;
	}

	// ------- normalization

	Vector2D operator !() const;

	/** Creates a vector with length 1 and same direction as this vector. In other words, a new vector that is a normalized version of this vector. Note that <b>the original vector remains unchanged</b>. */
	Vector2D unit() const;

	/** Divides this vector's coordinates by its length/magnitude, normalizing it.
	The returned object is the vector instance itself after normalization. */
	Vector2D& normalize();

	// ------- reflection

	Vector2D operator -() const;

	/** Creates and returns the opposite of this vector. In other words, returns a vector with same coordinates as this, but with changed signal. Note that <b>the original vector remains unchanged</b>. */
	Vector2D opposite() const;

	Vector2D& operator --();

	/** Changes the signal of this vector coordinates, effectively reflecting it.
	<br> The returned object is <b>the vector instance itself</b> after reflection. */
	Vector2D& reflect();

	/** Changes the signal of this vector x coordinate.
	<br> The returned object is <b>the vector instance itself</b> after reflection. */
	Vector2D& reflectX();

	/** Changes the signal of this vector y coordinate.
	<br> The returned object is <b>the vector instance itself</b> after reflection. */
	Vector2D& reflectY();

	// ------- basic arithmetic

	Vector2D operator +(const Vector2D& v) const;

	/** Creates and returns a vector that represents the sum of this vector and the given vector. Note that <b>the original vector remains unchanged</b>.*/
	Vector2D sum(const Vector2D& v) const;

	Vector2D& operator +=(const Vector2D& v);

	/** Adds to this vector the given vector. In other words, it performs an addition to this vector coordinates.
	<br> The returned object is <b>the vector instance itself</b> after summation. */
	Vector2D& add(const Vector2D& v);

	Vector2D operator -(const Vector2D& v) const;

	/** Creates a vector that represents the difference/displacement of this vector and the given vector, in this order. It's useful to remember that vector subtraction is <b>not commutative</b>: a-b != b-a.
	<br> Note that <b>the original vector remains unchanged</b>. */
	Vector2D difference(const Vector2D& v) const;

	Vector2D& operator -=(const Vector2D& v);

	/** Subtracts from this vector the given vector. In other words, it performs an subtraction to this vector coordinates.
	It's useful to remember that vector subtraction is <b>not commutative</b>: a-b != b-a.
	<br> The returned object is the <b>the vector instance itself</b> after subtraction. */
	Vector2D& subtract(const Vector2D& v);

	Vector2D operator *(const double& factor) const;

	/** Creates a vector that represents the scalar multiplication of this vector by the given factor. Note that <b>the original vector remains unchanged</b>.*/
	Vector2D times(const double factor) const;

	Vector2D& operator *=(const double& factor);

	/** Multiply this vectors coordinates by the given factor. The returned object is <b>the vector instance itself</b> after multiplication.*/
	Vector2D& scale(double factor);

	// ------- miscellaneous operations

	double operator %(const Vector2D& v) const;

	/** Compute the distance between this vector and the given vector. In other words, returns the length/magnitude of the displacement between this and the given vector. */
	double distance(const Vector2D& v) const;

	double operator ^(const Vector2D& v) const;

	/** Compute the inner/dot product between this and the given vector. */
	double innerProduct(const Vector2D& v) const;

	// ------- projection operations (XXX not tested)

	Vector2D operator ||(const Vector2D& v) const;

	/* Creates a vector that represents the projection of this vector on the given vector v. */
	Vector2D projection(const Vector2D& v) const;

	/* Creates a vector that represents the rejection of this vector on the given vector v. The rejection is defined as rej(u, v) = u - proj(u, v) */
	Vector2D rejection(const Vector2D& v) const;

	Vector2D operator |(const Vector2D& v) const;

	/* Creates a vector that represents the reflection of this vector in the axis represented by the given vector v. */
	Vector2D reflection(const Vector2D& v) const;

	// ------- rotation operations (XXX not tested)

	Vector2D operator <(const double& radians) const;

	Vector2D rotation(const double& radians) const;

	Vector2D& operator <<(const double& radians);

	Vector2D& rotate(const double& radians);

	Vector2D perpendicular() const;

	//-----------------------------------------------------------------

	/** Represents the null/zero vector. It has coordinates (0, 0). */
	const static Vector2D NULL_VECTOR;

	/** A vector codirectional with the X axis, with length 1. */
	const static Vector2D X_VERSOR;

	/** A vector codirectional with the Y axis, with length 1. */
	const static Vector2D Y_VERSOR;
};

#endif /* VECTOR2D_HPP_ */
