/*
 * util.cpp
 *
 *  Created on: 15 de jul de 2017
 *      Author: felipe
 */

#include "util.hpp"

#include "futil/string_split.hpp"
#include "futil/string_parse.hpp"
#include "futil/string_actions.hpp"

#include <iostream>

using fgeal::Color;
using std::string;
using std::vector;
using futil::split;
using futil::parse;
using futil::trim;

Color parseColorProperty(const string& property)
{
	vector<string> tokens = split(property, ',');
	try
	{
		if(tokens.size() == 4)
		{
			return Color(parse<unsigned char>(trim(tokens[0])),
						 parse<unsigned char>(trim(tokens[1])),
						 parse<unsigned char>(trim(tokens[2])),
						 parse<unsigned char>(trim(tokens[3])));
		}

		if(tokens.size() == 3)
		{
			return Color(parse<unsigned char>(trim(tokens[0])),
						 parse<unsigned char>(trim(tokens[1])),
						 parse<unsigned char>(trim(tokens[2])));
		}
		if(tokens.size() == 1)
		{
			return Color::fromArgbHex(parse<unsigned long>(trim(property), 16));
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "error while parse color property" << std::endl;
	}
	return Color::_TRANSPARENT;
}
