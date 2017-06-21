/*
 * map.cpp
 *
 *  Created on: 24/06/2013
 *      Author: carlosfaruolo
 */

#include "map.hpp"

#include <iostream>
using std::cout; using std::endl;

#include <fstream>

#include "futil/general/language.hpp"
#include "futil/string/more_operators.hpp"
#include "futil/string/actions.hpp"

#include "file_parser.hpp"

#include "terrarium_game.hpp"

Map::Map(Animation* bg, int columns, int lines, Rectangle* visibleArea) :
grid(), background(bg), visibleArea(visibleArea)
{
	grid.resize(columns, vector<Block*>(lines)); //fill the matrix with null
}

Map::Map(Image* bg, int columns, int lines, Rectangle* visibleArea) :
grid(), background(null), visibleArea(visibleArea)
{
	grid.resize(columns, vector<Block*>(lines));//fill the matrix with null
	SingleSheetAnimation* staticAnim = new SingleSheetAnimation(bg); //lets create an animation with a single image, given as parameter
	staticAnim->addSprite();  //setting the static animation with the whole image
	background = staticAnim;
}

Map::~Map()
{
	cout << "map destructor..." << endl;
	foreach(vector<Block*>, v, vector< vector<Block*> >, grid)
	{
		foreach(Block*, b, vector<Block*>, v)
		{
			if(b != null)
				cout << string("deleting block ") + b->x_grid_pos + ", "+b->y_grid_pos << endl;;
			delete b;
		}
	}

	delete background;
//	delete visibleArea;
}


Map* Map::loadMapFromFile(const string& filename, World* world, vector<Image*>& createdImages)
{
	Map* map=null;

	vector< vector<int> > file_grid;
	if(ends_with(filename, ".tmx"))
		file_grid = FileParser::parseGridFromTMXFile(filename);
	else
		file_grid = FileParser::parseGridFromRawTxtFile(filename);

	cout << "map size (in blocks): " << file_grid.size() << "x" << file_grid[0].size() << endl;

	Properties& config = TerrariumGame::CONFIG;

	Image* imgDirt = new Image("resources/tileset-dirt.png"); createdImages.push_back(imgDirt);
	Image* imgStone = new Image("resources/tileset-stone.png"); createdImages.push_back(imgStone);
	Image* imgWater = new Image("resources/tileset-water.png"); createdImages.push_back(imgWater);
	Image* imgGrass = new Image("resources/tileset-grass.png"); createdImages.push_back(imgGrass);
	Image* bg = new Image(config.get("ingame.bg.filename")); createdImages.push_back(bg);

	map = new Map(bg, file_grid.size(), file_grid[0].size(), NULL);

	for(unsigned int i = 0; i < file_grid.size() ; i++)
	{
		for(unsigned int j = 0; j < file_grid[0].size() ; j++)
		{
			if(file_grid[i][j] == 1)
			{
				map->grid[i][j] = new Block(Block::createBlockAnimationSet(imgDirt), i, j, 1);
				world->addBody(map->grid[i][j]->body);
				map->retile(map->grid[i][j]);
			}
			else if(file_grid[i][j] == 2)
			{
				map->grid[i][j] = new Block(Block::createBlockAnimationSet(imgStone), i, j, 2);
				world->addBody(map->grid[i][j]->body);
				map->retile(map->grid[i][j]);
			}
			else if(file_grid[i][j] == 3)
			{
				map->grid[i][j] = new Block(Block::createBlockAnimationSet(imgWater, 3, 1.0), i, j, 3, true);
				world->addBody(map->grid[i][j]->body);
				map->retile(map->grid[i][j]);
			}
			else if(file_grid[i][j] == 4)
			{
				map->grid[i][j] = new Block(Block::createBlockAnimationSet(imgGrass), i, j, 1);
				world->addBody(map->grid[i][j]->body);
				map->retile(map->grid[i][j]);
			}
		}
	}
	return map;
}

void Map::saveRawMapToFile(const string& filename, Map* map)
{
	std::ofstream stream(filename.c_str());

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
	unsigned k;
	bool hasLeft=false, hasRight=false, hasNorth=false, hasSouth=false;

	//WIP tileset algorithm

	if(b->x_grid_pos > 0) if(grid[b->x_grid_pos-1][b->y_grid_pos] != NULL) hasLeft = true;

	if(b->x_grid_pos < (int) grid.size() -1) if(grid[b->x_grid_pos+1][b->y_grid_pos] != NULL) hasRight = true;

	if(b->y_grid_pos > 0) if(grid[b->x_grid_pos][b->y_grid_pos-1] != NULL) hasNorth = true;

	if(b->y_grid_pos < (int) grid[0].size() -1) if(grid[b->x_grid_pos][b->y_grid_pos+1] != NULL) hasSouth = true;

	if(  hasNorth &&  hasSouth &&  hasLeft &&  hasRight) k = Block::Anim::FULL_COVER;
	if( !hasNorth &&  hasSouth &&  hasLeft &&  hasRight) k = Block::Anim::TOP_FREE;
	if(  hasNorth && !hasSouth &&  hasLeft &&  hasRight) k = Block::Anim::BOTTOM_FREE;
	if(  hasNorth &&  hasSouth && !hasLeft &&  hasRight) k = Block::Anim::LEFT_FREE;
	if(  hasNorth &&  hasSouth &&  hasLeft && !hasRight) k = Block::Anim::RIGHT_FREE;
	if( !hasNorth &&  hasSouth && !hasLeft &&  hasRight) k = Block::Anim::TOP_LEFT_FREE;
	if(  hasNorth && !hasSouth && !hasLeft &&  hasRight) k = Block::Anim::BOTTOM_LEFT_FREE;
	if( !hasNorth &&  hasSouth &&  hasLeft && !hasRight) k = Block::Anim::TOP_RIGHT_FREE;
	if(  hasNorth && !hasSouth &&  hasLeft && !hasRight) k = Block::Anim::BOTTOM_RIGHT_FREE;
	if(  hasNorth &&  hasSouth && !hasLeft && !hasRight) k = Block::Anim::HORIZONTAL_FREE;
	if( !hasNorth && !hasSouth &&  hasLeft &&  hasRight) k = Block::Anim::VERTICAL_FREE;
	if( !hasNorth &&  hasSouth && !hasLeft && !hasRight) k = Block::Anim::BOTTOM_COVERED;
	if(  hasNorth && !hasSouth && !hasLeft && !hasRight) k = Block::Anim::TOP_COVERED;
	if( !hasNorth && !hasSouth && !hasLeft &&  hasRight) k = Block::Anim::RIGHT_COVERED;
	if( !hasNorth && !hasSouth &&  hasLeft && !hasRight) k = Block::Anim::LEFT_COVERED;
	if( !hasNorth && !hasSouth && !hasLeft && !hasRight) k = Block::Anim::FULL_FREE;

//	COUT << "hasNorth = " << hasNorth << ", hasSouth = " << hasSouth << ", hasLeft = " << hasLeft << ", hasRight = " << hasRight << ENDL;

	b->animation->currentIndex = k;

	if(recursive)
	{
		if(hasNorth) retile(grid[b->x_grid_pos][b->y_grid_pos-1], false);
		if(hasSouth) retile(grid[b->x_grid_pos][b->y_grid_pos+1], false);
		if(hasLeft)  retile(grid[b->x_grid_pos-1][b->y_grid_pos], false);
		if(hasRight) retile(grid[b->x_grid_pos+1][b->y_grid_pos], false);
	}
}

Rectangle Map::computeDimensions()
{
	Rectangle size = { 0, 0, (float) grid.size()*BLOCK_SIZE, (float)(grid.size() > 0 ? grid[0].size()*BLOCK_SIZE : 0) };
	return size;
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

	if(visibleArea != NULL && visibleArea->w*visibleArea->y != 0 )
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

/** Draws all the blocks that backgrounds the player */
void Map::draw_bg_player()
{
	background->current().scale.x = fgeal::Display::getInstance().getWidth() / (float) background->current().width;
	background->current().scale.y = fgeal::Display::getInstance().getHeight() / (float) background->current().height;
	background->draw();

	int grid_number_of_lines = grid.capacity();
	int grid_number_of_columns = grid[0].capacity();

	if(visibleArea != NULL && visibleArea->w*visibleArea->h != 0 )
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
				if( grid[i][j] != NULL and grid[i][j]->typeID != 3)
				{
					grid[i][j]->draw(visibleArea);
				}
			}
	}

	else // case no usable visible area
	{
		//draw every block (usually inefficient)
		for(int i = 0; i < grid_number_of_lines; i++)
			for(int j = 0; j < grid_number_of_columns; j++)
				if( grid[i][j] != NULL and grid[i][j]->typeID != 3 )
					grid[i][j]->draw(NULL);

	}
}

/** Draws all the blocks that foregrounds the player */
void Map::draw_fg_player()
{
	int grid_number_of_lines = grid.capacity();
	int grid_number_of_columns = grid[0].capacity();

	if(visibleArea != NULL && visibleArea->w*visibleArea->h != 0 )
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
				if( grid[i][j] != NULL and grid[i][j]->typeID == 3)
				{
					grid[i][j]->draw(visibleArea);
				}
			}
	}

	else // case no usable visible area
	{
		//draw every block (usually inefficient)
		for(int i = 0; i < grid_number_of_lines; i++)
			for(int j = 0; j < grid_number_of_columns; j++)
				if( grid[i][j] != NULL and grid[i][j]->typeID == 3 )
					grid[i][j]->draw(NULL);

	}
}


