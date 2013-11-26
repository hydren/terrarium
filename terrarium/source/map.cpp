/*
 * map.cpp
 *
 *  Created on: 24/06/2013
 *      Author: carlosfaruolo
 */

#include "../header/map.hpp"

#include "../header/rapidxml/rapidxml_utils.hpp"

Map::Map(AnimationSet* bg, int columns, int lines, Rect* visibleArea) :
grid(), background(bg), visibleArea(visibleArea)
{
	grid.resize(columns, vector<Block*>(lines)); //fill the matrix with null
}

Map::Map(Image* bg, int columns, int lines, Rect* visibleArea) :
grid(), background(null), visibleArea(visibleArea)
{
	grid.resize(columns, vector<Block*>(lines));//fill the matrix with null
	AnimationSet* staticAnim = new AnimationSet(bg); //lets create an animation with a single image, given as parameter
	staticAnim->add("bg"); //setting the static animation with the whole image
	staticAnim->setCurrent("bg");
	background = staticAnim;
}

/** Loads old raw text map files, based on the terrarium "SDL" prototype.
 *  Deprecated */
Map* Map::loadRawMapFromFile(const String& filename, World* world)
{
	Map* map=null;

	vector< vector<int> > matrix;

	FileInputStream stream(filename.c_str());


	if(stream.is_open())
	{
		String str;
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

	vector< vector<int> > matrix_corrected = Util::transpose(matrix);

	Image* img = new Image("resource/tileset-dirt.png");

	cout << "map dimensions: " << matrix_corrected.size() << " " << matrix_corrected[0].size() << endl;
	map = new Map(new Image("resource/background.jpg"), matrix_corrected.size(), matrix_corrected[0].size(), NULL);

	for(unsigned int i = 0; i < matrix_corrected.size() ; i++)
	{
		for(unsigned int j = 0; j < matrix_corrected[0].size() ; j++)
		{
			if(matrix_corrected[i][j] == 1)
			{
				map->grid[i][j] = new Block( img, i, j);
				world->addBody(map->grid[i][j]->body);
				map->retile(map->grid[i][j]);
			}
		}
	}
	return map;
}

Map* Map::loadRawMapFromFile(const char* filename, World* world)
{
	return loadRawMapFromFile(String(filename), world);
}

void Map::saveRawMapToFile(const String& filename, Map* map)
{
	FileOutputStream stream(filename.c_str());

	if(stream.is_open())
	{
		for(unsigned int j = 0; j < map->grid[0].size() ; j++)
		{
			for(unsigned int i = 0; i < map->grid.size() ; i++)
			{
				stream << (map->grid[i][j] != null);
				if(i != map->grid.size() - 1)
					stream << ",";
			}
			stream << ":\n";
		}
	}

	stream.close();
}

//rapidxml::xml_node<>* MapParser::getNode(const string name, rapidxml::xml_node<>* parentNode) {
//	return parentNode==NULL ? doc.first_node(name.c_str()) : parentNode->first_node(name.c_str());
//}

Map* Map::loadMapFromTMXFile(const String& filename, World* world)
{
	Map* map = null;

	rapidxml::file<> *file = new rapidxml::file<>(filename.c_str());
	rapidxml::xml_document<> doc;

	cout << "parsing " << filename << "..." << endl;

	doc.parse<0>( (char*)file->data() );

	rapidxml::xml_node<>* nodeMap = doc.first_node("map");

	if (nodeMap == NULL)
	{
		cout << "failure!" << endl;
		throw Exception("Can't read node map from file "+filename);
	}

	int w = parseInt(nodeMap->first_attribute("width")->value());
	int h = parseInt(nodeMap->first_attribute("height")->value());

	rapidxml::xml_node<>* nodeLayer = nodeMap->first_node("layer");

	rapidxml::xml_node<>* n = nodeLayer->first_node("data");

//	generateGrid(w, h, nodeLayer->first_node("data"));

	vector< vector<int> > grid;
	vector<int> row;

	int count=0;



	for (rapidxml::xml_node<> *tile = n->first_node("tile"); tile; tile = tile->next_sibling())
	{
		row.push_back( parseInt(tile->first_attribute("gid")->value()) );
		if (++count == w)
		{
			grid.push_back(row);
			count = 0;
			row.clear();
		}
	}
	if(grid.size() < (unsigned) h)
		throw Exception("Error while generating map "+filename+"!");

	for(unsigned i = 0; i < grid.size(); i ++)
	{
		for(unsigned j = 0; j < grid[i].size(); j++)
			cout << grid[i][j];
		cout << endl;
	}

	cout << w << endl << h << endl;

	vector< vector<int> > grid_t = Util::transpose(grid);

	Image* imgDirt = new Image("resource/tileset-dirt.png");
	Image* imgStone = new Image("resource/tileset-stone.png");

	cout << "map dimensions: " << grid_t.size() << " " << grid_t[0].size() << endl;
	map = new Map(new Image("resource/background.jpg"), w, h, NULL);

	for(unsigned int i = 0; i < grid_t.size() ; i++)
	{
		for(unsigned int j = 0; j < grid_t[0].size() ; j++)
		{
			if(grid_t[i][j] == 1)
			{
				map->grid[i][j] = new Block( imgDirt, i, j);
				world->addBody(map->grid[i][j]->body);
				map->retile(map->grid[i][j]);
			}
			else if(grid_t[i][j] == 2)
			{
				map->grid[i][j] = new Block( imgStone, i, j);
				world->addBody(map->grid[i][j]->body);
				map->retile(map->grid[i][j]);
			}
		}
	}

	return map;
}


void Map::retileNeighbourhood(int x_grid_pos, int y_grid_pos)
{
	if(x_grid_pos > 0) if(grid[x_grid_pos-1][y_grid_pos] != NULL)
		retile(grid[x_grid_pos-1][y_grid_pos], false);

	if(x_grid_pos < (int) grid.size() -1) if(grid[x_grid_pos+1][y_grid_pos] != NULL)
		retile(grid[x_grid_pos+1][y_grid_pos], false);

	if(y_grid_pos > 0) if(grid[x_grid_pos][y_grid_pos-1] != NULL)
		retile(grid[x_grid_pos][y_grid_pos-1], false);

	if(y_grid_pos < (int) grid[0].size() -1) if(grid[x_grid_pos][y_grid_pos+1] != NULL)
		retile(grid[x_grid_pos][y_grid_pos+1], false);
}


/** Draws all the blocks in this map */
void Map::retile(Block* b, bool recursive)
{
	String k;
	bool hasLeft=false, hasRight=false, hasNorth=false, hasSouth=false;

	//WIP tileset algorithm

	if(b->x_grid_pos > 0) if(grid[b->x_grid_pos-1][b->y_grid_pos] != NULL) hasLeft = true;

	if(b->x_grid_pos < (int) grid.size() -1) if(grid[b->x_grid_pos+1][b->y_grid_pos] != NULL) hasRight = true;

	if(b->y_grid_pos > 0) if(grid[b->x_grid_pos][b->y_grid_pos-1] != NULL) hasNorth = true;

	if(b->y_grid_pos < (int) grid[0].size() -1) if(grid[b->x_grid_pos][b->y_grid_pos+1] != NULL) hasSouth = true;

	if(  hasNorth &&  hasSouth &&  hasLeft &&  hasRight) k = "full_cover";
	if( !hasNorth &&  hasSouth &&  hasLeft &&  hasRight) k = "top_free";
	if(  hasNorth && !hasSouth &&  hasLeft &&  hasRight) k = "bottom_free";
	if(  hasNorth &&  hasSouth && !hasLeft &&  hasRight) k = "left_free";
	if(  hasNorth &&  hasSouth &&  hasLeft && !hasRight) k = "right_free";
	if( !hasNorth &&  hasSouth && !hasLeft &&  hasRight) k = "top_left_free";
	if(  hasNorth && !hasSouth && !hasLeft &&  hasRight) k = "bottom_left_free";
	if( !hasNorth &&  hasSouth &&  hasLeft && !hasRight) k = "top_right_free";
	if(  hasNorth && !hasSouth &&  hasLeft && !hasRight) k = "bottom_right_free";
	if(  hasNorth &&  hasSouth && !hasLeft && !hasRight) k = "horizontal_free";
	if( !hasNorth && !hasSouth &&  hasLeft &&  hasRight) k = "vertical_free";
	if( !hasNorth &&  hasSouth && !hasLeft && !hasRight) k = "bottom_covered";
	if(  hasNorth && !hasSouth && !hasLeft && !hasRight) k = "top_covered";
	if( !hasNorth && !hasSouth && !hasLeft &&  hasRight) k = "right_covered";
	if( !hasNorth && !hasSouth &&  hasLeft && !hasRight) k = "left_covered";
	if( !hasNorth && !hasSouth && !hasLeft && !hasRight) k = "full_free";

//	COUT << "hasNorth = " << hasNorth << ", hasSouth = " << hasSouth << ", hasLeft = " << hasLeft << ", hasRight = " << hasRight << ENDL;

	b->animation->setCurrent(k);

	if(recursive)
	{
		if(hasNorth) retile(grid[b->x_grid_pos][b->y_grid_pos-1], false);
		if(hasSouth) retile(grid[b->x_grid_pos][b->y_grid_pos+1], false);
		if(hasLeft)  retile(grid[b->x_grid_pos-1][b->y_grid_pos], false);
		if(hasRight) retile(grid[b->x_grid_pos+1][b->y_grid_pos], false);
	}
}

void Map::addBlock(int x, int y)
{
//	grid[x][y] = new Block()
}
void Map::draw()
{
	background->draw();

	int grid_number_of_lines = grid.capacity();
	int grid_number_of_columns = grid[0].capacity();

	if(visibleArea != NULL && visibleArea->area() != 0 )
	{
		//draws all blocks that can be fully or partially seen, but not the ones that can't be seen
		int start_i_index = visibleArea->x / BLOCK_SIZE;
		int start_j_index = visibleArea->y / BLOCK_SIZE;
		int finish_i_index = ((visibleArea->x + visibleArea->w) / BLOCK_SIZE) + 1;
		int finish_j_index = (visibleArea->y + visibleArea->h) / BLOCK_SIZE;

		//just to be safe
		if( start_i_index < 0 ) start_i_index = 0;
		else if( start_i_index >  grid_number_of_lines -1 ) start_i_index =  grid_number_of_lines -1;
		if( start_j_index < 0 ) start_j_index = 0;
		else if(start_j_index > grid_number_of_columns -1 ) start_j_index = grid_number_of_columns - 1;
		if( finish_i_index < 0 ) finish_i_index = 0;
		else if ( finish_i_index >  grid_number_of_lines -1 ) finish_i_index =  grid_number_of_lines -1;
		if( finish_j_index < 0 ) finish_j_index = 0;
		else if ( finish_j_index > grid_number_of_columns -1 ) finish_j_index = grid_number_of_columns-1;

		//draw the corresponding blocks
		for( int i = start_i_index ; i <= finish_i_index ; i++)
			for( int j = start_j_index ; j <= finish_j_index ; j++)
			{
				if( grid[i][j] != NULL )
				{
					grid[i][j]->draw(visibleArea);
				}
			}
	}

	else //draw every block (usually inefficient)
	{
		for(int i = 0; i < grid_number_of_lines; i++)
			for(int j = 0; j < grid_number_of_columns; j++)
				if( grid[i][j] != NULL )
					grid[i][j]->draw(NULL);

	}
}


