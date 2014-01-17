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

//adds almost java-like capabilities to String. It's still not converting char*+int and alike to String
string operator + (string a, string b);

string operator + (string a, int b);
string operator + (string a, long b);
string operator + (string a, short b);
string operator + (string a, unsigned b);
string operator + (string a, float b);
string operator + (string a, double b);
string operator + (string a, char b);
string operator + (string a, char* b);


//backwards versions

string operator + (int a, string b);
string operator + (long a, string b);
string operator + (short a, string b);
string operator + (unsigned a, string b);
string operator + (float a, string b);
string operator + (double a, string b);
string operator + (char a, string b);
string operator + (char* a, string b);

namespace String
{
	/** Returns a copy of the string, with leading and trailing whitespace omitted. */
	string trim(const string& str);

	/** Returns true if the given string str ends with the given string ending */
	bool endsWith (string const& str, string const& ending);
}

namespace Math
{
	const double PI = 3.14159265358979323846;

	inline int abs(int i)
	{
		return i < 0 ? -i : i;
	}

	/** Parses the string for matching an integer. */
	int parseInt(const string& str);

	float convertToMeters(float pixels);

	float convertToPixels(float meters);
}

/** Class created to behave like the Java's Exception
 * Contains a String msg, with the error message
 */
class Exception
{
    private:

    string msg;

    public:

    Exception(string str)
    : msg(str)
    {}

    Exception(const char* format, ...);

    string message() const { return msg; }
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
