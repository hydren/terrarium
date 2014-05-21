/*
 * util.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#include "../headers/util.hpp"

#include <cstdarg>
#include <sstream>

// ====================  adding almost java-like habilities to String

/** Returns a copy of the string, with leading and trailing whitespace omitted. */
string String::trim(const string& str)
{
	string str2 = str;
	size_t found;
	found = str2.find_last_not_of(" ");
	if (found != string::npos)
	  	str2.erase(found+1);
	else
	   	str2.clear();

    return str2;
}

/** Returns true if the given string str ends with the given string ending */
bool String::endsWith (string const& str, string const& ending)
{
    if (str.length() >= ending.length()) {
        return (0 == str.compare (str.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

//just to reduce redundant text
#define CONCATENATION { std::stringstream ss; ss << a << b; return ss.str(); }

string operator + (string a, string b) CONCATENATION;
string operator + (string a, int b) CONCATENATION;
string operator + (string a, long b) CONCATENATION
string operator + (string a, short b) CONCATENATION;
string operator + (string a, unsigned b) CONCATENATION;
string operator + (string a, float b) CONCATENATION;
string operator + (string a, double b) CONCATENATION;
string operator + (string a, char b) CONCATENATION;
string operator + (string a, char* b) CONCATENATION;

//backwards versions

string operator + (int a, string b) CONCATENATION;
string operator + (long a, string b) CONCATENATION;
string operator + (short a, string b) CONCATENATION;
string operator + (unsigned a, string b) CONCATENATION;
string operator + (float a, string b) CONCATENATION;
string operator + (double a, string b) CONCATENATION;
string operator + (char a, string b) CONCATENATION;
string operator + (char* a, string b) CONCATENATION;

//==============================================================================
//================== Exception

Exception::Exception(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsprintf(buffer, format, args);
    msg = string(buffer);
    va_end(args);
}

//==============================================================================
//=================== Math namespace

/** Parses the string for matching an integer. */
int Math::parseInt(const string& str)
{
	int i;
	std::istringstream(str.c_str()) >> i;
	return i;
}

float Math::convertToMeters(float pixels)
{
	return 0.01f * pixels;
}

float Math::convertToPixels(float meters)
{
	return 100.0f * meters;
}

//==============================================================================
//================== Rect

//Returns true if this bounding box intersects with the given Rect.
bool Rect::intersects(const Rect& box )
{
	if
    (   ((int)this->x + (int)this->w) <= (int)box.x || //S1, S4, S7
        (int)this->x  >= ((int)box.x + (int)box.w)  || //S3, S6, S9
        ((int)this->y + (int)this->h) <= (int)box.y || //S1, S2, S3
        (int)this->y  >= ((int)box.y + (int)box.h)     //S7, S8, S9
	) return false;
	else return true;

/*

 * I know it isn't the most straightfoward way to do the check, but it uses less statments.
 * If it is on S1, S2, S3, S4, S6, S7, S8 or S9, it does not intersect, else it does.
     S1   |         S2          |   S3
          |                     |
       TOP|LEFT                 |
   -----------------------------------
          |                     |
          |                     |
          |                     |
     S4   |         S5          |   S6
          |                     |
          |                     |
   -----------------------------------
          |                RIGHT|BOTTOM
          |                     |
	 S7   |         S8          |   S9
*/
}

Rect Rect::intersection(const Rect& box)
{
	int x, y, h, w;
	if(!intersects(box)) throw Exception("Bounding boxes does not intersect!");
	if(this->x <= box.x){
		x = box.x;
		if(this->w >= box.x + box.w)
			w = box.w;
		else
			w = this->w - (box.x - this->x);
	}
	else{
		x = this->x;
		if(box.w >= this->x + this->w)
			w = this->w;
		else
			w = box.w - (this->x - box.x);
	}
	if(this->y <= box.y){
		y = box.y;
		if(this->h >= box.h + box.y)
			h = box.h;
		else
			h = this->h - (box.y - this->y);
	}
	else{
		y = this->y;
		if(box.h >= this->h + this->y)
			h = this->h;
		else
			h = box.h - (this->y - box.y);
	}
	Rect intersectionBox(x,y,w,h);
	return intersectionBox;
}

int Rect::area()
{
	return w*h; //you don't say
}


