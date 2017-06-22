/*
 * map_io.cpp
 *
 *  Created on: 22 de jun de 2017
 *      Author: carlosfaruolo
 */

#include "map.hpp"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

#include "futil/general/language.hpp"
#include "futil/string/more_operators.hpp"
#include "futil/string/actions.hpp"
#include "futil/math/parse_number.hpp"

#include <stdexcept>

#include <iostream>
using std::cout; using std::endl;

static vector< vector <int> > transpose(const vector< vector<int> >& matrix)
{
	//if empty, return a new empty
	if(matrix.size() == 0)
		return vector< vector<int> >();

	//safety check
	for(unsigned i = 0, size=matrix[0].size(); i < matrix.size(); i++)
		if(matrix[i].size() != size)
			throw string("Matrix with differing row sizes! " + matrix[i].size());

	vector< vector<int> > matrix_t(matrix[0].size(), vector<int>(matrix.size()));

	for(unsigned i = 0; i < matrix.size(); i++)
		for(unsigned j = 0; j < matrix[i].size(); j++)
			matrix_t[j][i] = matrix[i][j];

	return matrix_t;
}

/** Loads the map grid from a raw txt file, based on the old terrarium "SDL" prototype.*/
vector< vector<int> > Map::parseGridFromRawTxtFile(const string& filename)
{
	vector< vector<int> > matrix;

	FileInputStream stream(filename.c_str());

	if(stream.is_open())
	{
		string str;
		while(stream.good())
		{
			getline(stream, str);
			if(str.size() > 0 && trim(str).size() > 0) //safety
			{
				matrix.push_back(vector<int>(str.length()/2));
				for(unsigned int i = 0; i < str.length()/2; i++)
				{
					switch(str.c_str()[2*i])
					{
						case '0':
						{
							matrix.back()[i] = 0;
							break;
						}
						case '1':
						{
							matrix.back()[i] = 1;
							break;
						}
						case '2':
						{
							matrix.back()[i] = 2;
							break;
						}
						case '3':
						{
							matrix.back()[i] = 3;
							break;
						}
						case '4':
						{
							matrix.back()[i] = 1;//TODO remove this dirty fix
							break;
						}
						default:
						{
							matrix.back()[i] = 0;
							break;
						}
					}
				}
			}
		}
	}
	else throw std::runtime_error("File not found! "+filename);

	stream.close();

	return transpose(matrix);
}

vector< vector<int> > Map::parseGridFromTMXFile(const string& filename)
{
	rapidxml::file<> *file = new rapidxml::file<>(filename.c_str());
	rapidxml::xml_document<> doc;

	cout << "parsing " << filename << "..." << endl;

	doc.parse<0>( (char*)file->data() );

	rapidxml::xml_node<>* nodeMap = doc.first_node("map");

	if (nodeMap == NULL)
	{
//			cout << "failure!" << endl;
		throw std::runtime_error("Can't read node map from file "+filename);
	}

	int w = parse<int>(nodeMap->first_attribute("width")->value());
	int h = parse<int>(nodeMap->first_attribute("height")->value());

	rapidxml::xml_node<>* nodeLayer = nodeMap->first_node("layer");

	rapidxml::xml_node<>* n = nodeLayer->first_node("data");

//	generateGrid(w, h, nodeLayer->first_node("data"));

	vector< vector<int> > grid;
	vector<int> row;

	int count=0;

	for (rapidxml::xml_node<> *tile = n->first_node("tile"); tile; tile = tile->next_sibling())
	{
		row.push_back( parse<int>(tile->first_attribute("gid")->value()) );
		if (++count == w)
		{
			grid.push_back(row);
			count = 0;
			row.clear();
		}
	}
	if(grid.size() < (unsigned) h)
		throw std::runtime_error("Error while generating map "+filename+"!");

//		for(unsigned i = 0; i < grid.size(); i ++)
//		{
//			for(unsigned j = 0; j < grid[i].size(); j++)
//				cout << grid[i][j];
//			cout << endl;
//		}
//
//		cout << w << endl << h << endl;

	return transpose(grid);
}

void Map::saveGridToRawTxtFile(vector< vector<int> > grid, const string& filename)
{
	FileOutputStream stream(filename.c_str());

	if(stream.is_open())
	{
		for(unsigned int j = 0; j < grid[0].size() ; j++)
		{
			for(unsigned int i = 0; i < grid.size() ; i++)
			{
				stream << grid[i][j];
				if(i != grid.size() - 1)
					stream << ",";
			}
			stream << ":\n";
		}
	}

	stream.close();
}
