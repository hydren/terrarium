/*
 * util_exception.hpp
 *
 *  Created on: 03/03/2015
 *      Author: Visagio
 */

#ifndef UTILS_UTIL_EXCEPTION_HPP_
#define UTILS_UTIL_EXCEPTION_HPP_

#include "util_string.hpp"

/** Class created to behave like the Java's Exception
 * Contains a string message with the error message
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

    inline string message() const { return msg; }
};



#endif /* UTILS_UTIL_EXCEPTION_HPP_ */
