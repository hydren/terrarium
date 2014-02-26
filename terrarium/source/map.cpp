/*
 * map.cpp
 *
 *  Created on: 24/06/2013
 *      Author: carlosfaruolo
 */

#include "../header/map.hpp"
#include "../header/file_parser.hpp"

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


Map* Map::loadMapFromFile(const string& filename, World* world)
{
	Map* map=null;

	vector< vector<int> > file_grid;
	if(String::endsWith(filename, ".tmx"))
		file_grid = FileParser::parseGridFromTMXFile(filename);
	else
		file_grid = FileParser::parseGridFromRawTxtFile(filename);

	cout << "map dimensions: " << file_grid.size() << " " << file_grid[0].size() << endl;

	Image* imgDirt = new Image("resource/tileset-dirt.png");
	Image* imgStone = new Image("resource/tileset-stone.png");
	Image* imgWater = new Image("resource/tileset-water.png");

	map = new Map(new Image("resource/background.jpg"), file_grid.size(), file_grid[0].size(), NULL);

	for(unsigned int i = 0; i < file_grid.size() ; i++)
	{
		for(unsigned int j = 0; j < file_grid[0].size() ; j++)
		{
			if(file_grid[i][j] == 1)
			{
				map->grid[i][j] = new Block( imgDirt, i, j);
				world->addBody(map->grid[i][j]->body);
				map->retile(map->grid[i][j]);
			}
			else if(file_grid[i][j] == 2)
			{
				map->grid[i][j] = new Block( imgStone, i, j);
				world->addBody(map->grid[i][j]->body);
				map->retile(map->grid[i][j]);
			}
			else if(file_grid[i][j] == 3)
			{
				map->grid[i][j] = new Block( imgWater, i, j, true);
				world->addBody(map->grid[i][j]->body);
				map->retile(map->grid[i][j]);
			}
		}
	}
	return map;
}

void Map::saveRawMapToFile(const string& filename, Map* map)
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
	string k;
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


