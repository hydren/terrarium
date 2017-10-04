/*
 * map_io.cpp
 *
 *  Created on: 22 de jun de 2017
 *      Author: carlosfaruolo
 */

#include "map.hpp"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

#include "futil/language.hpp"
#include "futil/string_extra_operators.hpp"
#include "futil/string_actions.hpp"
#include "futil/string_parse.hpp"

#include <stdexcept>

#include <iostream>
using std::cout; using std::endl;

void Map::transpose(vector< vector<int> >& matrix)
{
	//if empty, do nothing
	if(matrix.size() == 0)
		return;

	//safety check
	for(unsigned i = 0, size=matrix[0].size(); i < matrix.size(); i++)
		if(matrix[i].size() != size)
			throw string("Matrix with differing row sizes! " + matrix[i].size());

	const vector< vector<int> > matrixBackup = matrix;

	matrix.clear();
	matrix.resize(matrixBackup[0].size(), vector<int>(matrixBackup.size()));

	for(unsigned i = 0; i < matrixBackup.size(); i++)
		for(unsigned j = 0; j < matrixBackup[i].size(); j++)
			matrix[j][i] = matrixBackup[i][j];
}

// static
void Map::parseGridFromFileTxt(vector< vector<int> >& matrix, const string& filename)
{
	FileInputStream stream(filename.c_str());

	matrix.clear();
	if(stream.is_open())
	{
		string str;
		while(stream.good())
		{
			getline(stream, str);
			if(str.size() > 0 && futil::trim(str).size() > 0) //safety
			{
				matrix.push_back(vector<int>(str.length()/2));
				for(unsigned int i = 0; i < str.length()/2; i++)
				{
					const char typeIdChar = str.c_str()[2*i];
					if(typeIdChar >= '0' and typeIdChar <= '9')
					{
						matrix.back()[i] = (typeIdChar - '0');
					}
				}
			}
		}
	}
	else throw std::runtime_error("File not found! "+filename);

	stream.close();

	transpose(matrix);
}

// static
void Map::parseGridFromFileTmx(vector< vector<int> >& grid, const string& filename)
{
	rapidxml::file<> *file = new rapidxml::file<>(filename.c_str());
	rapidxml::xml_document<> doc;

	cout << "parsing " << filename << "..." << endl;

	doc.parse<0>( (char*)file->data() );

	rapidxml::xml_node<>* nodeMap = doc.first_node("map");

	if (nodeMap == NULL)
	{
//		cout << "failure!" << endl;
		throw std::runtime_error("Can't read node map from file "+filename);
	}

	int w = futil::parse<int>(nodeMap->first_attribute("width")->value());
	int h = futil::parse<int>(nodeMap->first_attribute("height")->value());

	rapidxml::xml_node<>* nodeLayer = nodeMap->first_node("layer");

	rapidxml::xml_node<>* n = nodeLayer->first_node("data");

	vector<int> row;

	int count=0;

	for (rapidxml::xml_node<> *tile = n->first_node("tile"); tile; tile = tile->next_sibling())
	{
		row.push_back( futil::parse<int>(tile->first_attribute("gid")->value()) );
		if (++count == w)
		{
			grid.push_back(row);
			count = 0;
			row.clear();
		}
	}
	if(grid.size() < (unsigned) h)
		throw std::runtime_error("Error while generating map "+filename+"!");

	/*
	// debug
	for(unsigned i = 0; i < grid.size(); i ++)
	{
		for(unsigned j = 0; j < grid[i].size(); j++)
			cout << grid[i][j];
		cout << endl;
	}

	cout << w << endl << h << endl;
	*/

	transpose(grid);
}

// static
void Map::saveGridToFileTxt(const vector< vector<int> >& grid, const string& filename)
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
	else throw std::runtime_error("Could not save map to txt file \""+filename+"\"");
	stream.close();
}
