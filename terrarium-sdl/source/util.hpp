#ifndef UTIL_HPP_INCLUDED
#define UTIL_HPP_INCLUDED


//utilities, multi-purpose things

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>

// * * * * * Useful macros

//Same as std::cout
#define COUT std::cout

//Same as std::endl
#define ENDL std::endl

//Same as std::cerr
#define CERR std::cerr

/* Same as '= 0'
 * Used right after method declaration. Ex: int giveNumber(const int x) abstract;
 * Means that this function has no implementation, making the containing class abstract, impossible to instanciate it.
 * The only way to instanciate, is to instanciate a subclass of it that is not abstract.
 * You can, however, declare a pointer to this class, or a reference.
 * Ex: Animal* cat = new Cat();
 * Dog dog;
 * Animal& another_dog = dog;
 */
#define abstract = 0

// Same as ':'.
#define extends :

// Same as std::string
typedef std::string String;

//adds almost java-like habilities to String. it still not converting char*+int to String and alike
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

//class to provide some goodies
class Util
{
    public:
    inline static int abs(int i)
    {
        return i < 0 ? -i : i;
    }

    static FILE* loadFile(const String& filename, bool abortOnFailure=false);
    static FILE* createFile(const String& filename, bool abortOnFailure=false);
    static void closeFile(FILE* file);
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

/* Class that contains position and size of a box.
 * It has a method to check for intersection between other BoundingBox's.
 * Useful to simulate collision between bodies.
 */
class Rectangle
{
    public:

    float x, y, w, h;

    Rectangle() :
        x(0),y(0),w(0),h(0)
    {}

    //Creates a bounding box with the given parameters
    Rectangle(int x1, int y1, int w1, int h1) :
    x(x1), y(y1), w(w1), h(h1) {}

    //Returns true if this bounding box intersects with the given bounding box.
    bool intersects(const Rectangle& box );

    /* Returns a bounding box that represents the intersection of this bounding box with the given bounding box.
     * If there is no intersection, throws an Exception.
     * In other words, you should check if the bounding boxes intersects before trying to call this method, with the intersects() method.
     */
    Rectangle intersection(const Rectangle& box);

    /* Returns the area of this bounding box.
     * The calculation is pretty silly: w*h (width x height)
     */
    int area();
};

/*NOTES:
 * A "pure" virtual member is a virtual method that is declared to be = 0. Example:
 *
 * virtual void foo() = 0;
 *
 * To make the code more readable, there is a macro named "abstract" that is defined as "= 0".
 * With this you can declare pure virtual methods like this:
 *
 * virtual void foo() abstract; //cool huh?
 *
 * You can even make CodeLite recognize "abstract" as a keyword, making it appear with the same color as ther others keywords!
 * To do this go to Settings -> Syntax Highlighting and Fonts -> Customize -> C++ keyword -> Set 0, and add "abstract" to the list.
*/

#endif // UTIL_HPP_INCLUDED
