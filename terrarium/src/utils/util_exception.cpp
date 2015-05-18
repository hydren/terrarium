/*
 * util_exception.cpp
 *
 *  Created on: 03/03/2015
 *      Author: Visagio
 */

#include "utils/util_exception.hpp"

#include <cstdarg>
#include <cstdio>

Exception::Exception(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsprintf(buffer, format, args);
    msg = string(buffer);
    va_end(args);
}

