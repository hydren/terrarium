/*
 * util_string.hpp
 *
 *  Created on: 03/03/2015
 *      Author: Visagio
 */

#ifndef UTIL_STRING_HPP_
#define UTIL_STRING_HPP_

#include <sstream>
#include <string>

class Exception;

using std::string;

#include "util_exception.hpp"

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

    template <typename T>
    T parse(const string& str)
    {
    	std::stringstream convertor(str);
    	T value;
    	convertor >> value;
    	if(convertor.fail())
    		throw Exception("Failed to convert " + str);
    	return value;

    }

    template <typename T>
	bool parseable(const string& str)
	{
		std::stringstream convertor(str);
		T value;
		convertor >> value;
		return not convertor.fail();
	}
}

#endif /* UTIL_STRING_HPP_ */
