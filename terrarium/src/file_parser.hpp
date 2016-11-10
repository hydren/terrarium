/*
 * file_parser.hpp
 *
 *  Created on: 16/01/2014
 *      Author: felipe
 */

#ifndef FILE_PARSER_HPP_
#define FILE_PARSER_HPP_

#include <vector>
using std::vector;

#include <string>
using std::string;

namespace FileParser
{
	/** Loads the map grid from a raw txt file, based on the old terrarium "SDL" prototype.*/
	vector< vector<int> > parseGridFromRawTxtFile(const string& file_path);

	vector< vector<int> > parseGridFromTMXFile(const string& file_path);

	void saveGridToRawTxtFile(vector< vector<int> > grid);
}

#endif /* FILE_PARSER_HPP_ */
