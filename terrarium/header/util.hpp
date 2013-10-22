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
#include <sstream>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>
#include <map>
#include <vector>

using std::string;
using std::map;
using std::vector;
using std::pair;
using std::ifstream;
using std::ofstream;

// * * * * * Useful macros

#define ALLEGRO_MOUSE_BUTTON_LEFT 1
#define ALLEGRO_MOUSE_BUTTON_RIGHT 2
#define ALLEGRO_MOUSE_BUTTON_MIDDLE 3

//Same as std::cout
#define COUT std::cout

//Same as std::endl
#define ENDL std::endl

//Same as std::cerr
#define CERR std::cerr

/* Same as '=0'
 * Used right after method declaration. Ex: int giveNumber(const int x) abstract;
 * Means that this function has no implementation, making the containing class abstract, impossible to instanciate it.
 * The only way to instanciate, is to instanciate a subclass of it that is not abstract.
 * You can, however, declare a pointer to this class, or a reference.
 * Ex: Animal* cat = new Cat();
 * Dog dog;
 * Animal& another_dog = dog;
 */
#define abstract =0

// Same as ':'
#define extends :

// Same as ':'
#define sets :

// Java mode null
#define null NULL

// Just being hipster
typedef ifstream FileInputStream;

typedef ofstream FileOutputStream;

// Same as std::string
typedef string String;

//adds almost java-like abilities to String. It's still not converting char*+int and alike to String
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

namespace Math
{
	const double PI = 3.14159265358979323846;

	inline int abs(int i)
	{
		return i < 0 ? -i : i;
	}
}

//class to provide some old C goodies
namespace Util
{
    FILE* loadCFile(const String& filename, bool abortOnFailure=false);
    FILE* createCFile(const String& filename, bool abortOnFailure=false);
    void closeCFile(FILE* file);
};

/* Class created to behave like the Java's Exception
 * Contains a String msg, with the error message
 */
class Exception
{
    private:

    String msg;

    public:

    Exception(std::string str)
    {
        msg = str;
    }

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


#endif /* UTIL_HPP_ */
