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
using std::endl;
using std::cerr;
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::map;
using std::list;
using std::pair;

// * * * * * Useful macros

/* Automatic bridge macro, to encapsulate the implementation and speed up compilation time. It also cleans the global namespace.
 * This technique is called PImpl idiom, Compiler firewall idiom, handle classes, Cheshire Cat, etc... */
#define encapsulated struct Implementation; Implementation *implementation
#define encapsulation(PARENT_CLASS) struct PARENT_CLASS::Implementation

//foreach useful macro
#define foreach(TYPE, ELEMENT, COLLECTION_TYPE, COLLECTION)\
for(COLLECTION_TYPE::iterator ELEMENT##MACRO_TEMP_IT = (COLLECTION).begin(); ELEMENT##MACRO_TEMP_IT != (COLLECTION).end(); ++ELEMENT##MACRO_TEMP_IT)\
for(bool ELEMENT##MACRO_TEMP_B = true; ELEMENT##MACRO_TEMP_B;)\
for(TYPE ELEMENT = *(ELEMENT##MACRO_TEMP_IT); ELEMENT##MACRO_TEMP_B; ELEMENT##MACRO_TEMP_B = false)

// Just being hipster
#define abstract =0
#define null NULL
#define extends :
#define sets :

#include "utils/util_string.hpp"
#include "utils/util_exception.hpp"
#include "utils/util_math.hpp"

typedef ifstream FileInputStream;
typedef ofstream FileOutputStream;

template<typename T>
T max(T a, T b)
{
	return (a > b)? a : b;
}

template<typename T>
T min(T a, T b)
{
	return (a < b)? a : b;
}

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
