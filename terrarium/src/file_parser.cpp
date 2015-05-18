/*
 * file_parser.cpp
 *
 *  Created on: 16/01/2014
 *      Author: felipe
 */

#include "file_parser.hpp"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"


namespace FileParser
{
	/** Loads the map grid from a raw txt file, based on the old terrarium "SDL" prototype.*/
	vector< vector<int> > parseGridFromRawTxtFile(const string& filename)
	{
		vector< vector<int> > matrix;

		FileInputStream stream(filename.c_str());

		if(stream.is_open())
		{
			string str;
			while(stream.good())
			{
				getline(stream, str);
				if(str.size() > 0 && String::trim(str).size() > 0) //safety
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
		else throw Exception("File not found! "+filename);

		stream.close();

		return Util::transpose(matrix);
	}

	vector< vector<int> > parseGridFromTMXFile(const string& filename)
	{
		rapidxml::file<> *file = new rapidxml::file<>(filename.c_str());
		rapidxml::xml_document<> doc;

		cout << "parsing " << filename << "..." << endl;

		doc.parse<0>( (char*)file->data() );

		rapidxml::xml_node<>* nodeMap = doc.first_node("map");

		if (nodeMap == NULL)
		{
//			cout << "failure!" << endl;
			throw Exception("Can't read node map from file "+filename);
		}

		int w = Math::parseInt(nodeMap->first_attribute("width")->value());
		int h = Math::parseInt(nodeMap->first_attribute("height")->value());

		rapidxml::xml_node<>* nodeLayer = nodeMap->first_node("layer");

		rapidxml::xml_node<>* n = nodeLayer->first_node("data");

	//	generateGrid(w, h, nodeLayer->first_node("data"));

		vector< vector<int> > grid;
		vector<int> row;

		int count=0;

		for (rapidxml::xml_node<> *tile = n->first_node("tile"); tile; tile = tile->next_sibling())
		{
			row.push_back( Math::parseInt(tile->first_attribute("gid")->value()) );
			if (++count == w)
			{
				grid.push_back(row);
				count = 0;
				row.clear();
			}
		}
		if(grid.size() < (unsigned) h)
			throw Exception("Error while generating map "+filename+"!");

//		for(unsigned i = 0; i < grid.size(); i ++)
//		{
//			for(unsigned j = 0; j < grid[i].size(); j++)
//				cout << grid[i][j];
//			cout << endl;
//		}
//
//		cout << w << endl << h << endl;

		return Util::transpose(grid);
	}

	void saveGridToRawTxtFile(vector< vector<int> > grid, const string& filename)
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
}


