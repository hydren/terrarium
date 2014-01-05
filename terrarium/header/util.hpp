/*
 * util.hpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#ifndef UTIL_HPP_
#define UTIL_HPP_

//utilities, multi-purpose things

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <cstdarg>

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::map;
using std::list;
using std::pair;

// * * * * * Useful macros

/** Automatic bridge macro, to encapsulate the implementation and speed up compilation time. It also cleans the global namespace.
 * This technique is called PImpl idiom, Compiler firewall idiom, handle classes, Cheshire Cat, etc... */
#define encapsulated struct Implementation; Implementation *implementation
#define encapsulation(PARENT_CLASS) struct PARENT_CLASS::Implementation

// Just being hipster
#define abstract =0
#define null NULL
#define extends :
#define sets :

typedef ifstream FileInputStream;
typedef ofstream FileOutputStream;
typedef string String;

//adds almost java-like capabilities to String. It's still not converting char*+int and alike to String
String operator + (String a, String b);

String operator + (String a, int b);
String operator + (String a, long b);
String operator + (String a, short b);
String operator + (String a, float b);
String operator + (String a, double b);
String operator + (String a, char b);
String operator + (String a, char* b);

//backwards versions

String operator + (int a, String b);
String operator + (long a, String b);
String operator + (short a, String b);
String operator + (float a, String b);
String operator + (double a, String b);
String operator + (char a, String b);
String operator + (char* a, String b);

/** Returns a copy of the string, with leading and trailing whitespace omitted. */
String trim(const String& str);

int parseInt(const String& str);

namespace Math
{
	const double PI = 3.14159265358979323846;

	inline int abs(int i)
	{
		return i < 0 ? -i : i;
	}
}

/** Class created to behave like the Java's Exception
 * Contains a String msg, with the error message
 */
class Exception
{
    private:

    String msg;

    public:

    Exception(std::string str)
    : msg(str)
    {}

    Exception(const char* format, ...);

    std::string message() const { return msg; }
};

/** Class that contains position and size of a box.
 * It has a method to check for intersection between other Rect's.
 * It would be named Rectangle but there is a internal name conflict in windows' and allegro's headers that prevents from using the name. For more info see: https://www.allegro.cc/forums/thread/612387
 */
struct Rect
{
    float x, y, w, h;

    Rect() :
    x(0),y(0),w(0),h(0) {}

    //Creates a Rect with the given parameters
    Rect(float x1, float y1, float w1, float h1) :
    x(x1), y(y1), w(w1), h(h1) {}

    //Returns true if this bounding box intersects with the given Rect.
    bool intersects(const Rect& box );

    /* Returns a Rect that represents the intersection of this Rect with the given Rect.
     * If there is no intersection, throws an Exception. TODO exchange this behavior with returning null
     * In other words, you should check if the Rects intersects before trying to call this method, with the intersects() method.
     */
    Rect intersection(const Rect& box);

    /* Returns the area of this Rect.
     * The calculation is pretty silly: w*h (width x height)
     */
    int area();
};

float convertToMeters(float pixels);

float convertToPixels(float meters);

namespace Util
{
	template <typename Type>
	vector< vector <Type> > transpose(const vector< vector<Type> >& matrix)
	{
		//if empty, return a new empty
		if(matrix.size() == 0)
			return vector< vector<int> >();

		//safety check
		for(unsigned i = 0, size=matrix[0].size(); i < matrix.size(); i++)
			if(matrix[i].size() != size)
				throw Exception("Matrix with differing row sizes! " + matrix[i].size());

		vector< vector<Type> > matrix_t(matrix[0].size(), vector<Type>(matrix.size()));

		for(unsigned i = 0; i < matrix.size(); i++)
			for(unsigned j = 0; j < matrix[i].size(); j++)
				matrix_t[j][i] = matrix[i][j];

		return matrix_t;
	}
}


#endif /* UTIL_HPP_ */
