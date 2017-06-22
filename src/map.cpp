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
#include "ingame_state.hpp"

static Physics::Vector GRAVITY(0.0, 10.0);

Map::Map(InGameState* state, int columns, int lines)
: state(*state), grid(), background(null), visibleArea(null), world(new Physics::World(GRAVITY))
{
	grid.resize(columns, vector<Block*>(lines));  // fill the matrix with null
}

Map::Map(InGameState* state, const string filename)
: state(*state), grid(), background(new Sprite(state->bg, state->bg->getWidth(), state->bg->getHeight())), visibleArea(null), world(new Physics::World(GRAVITY))
{
	vector< vector<int> > file_grid;
	if(ends_with(filename, ".tmx"))
		file_grid = FileParser::parseGridFromTMXFile(filename);
	else
		file_grid = FileParser::parseGridFromRawTxtFile(filename);

	cout << "map size (in blocks): " << file_grid.size() << "x" << file_grid[0].size() << endl;

	grid.resize(file_grid.size(), vector<Block*>(file_grid[0].size()));  // fill the matrix with null

	for(unsigned int i = 0; i < file_grid.size() ; i++)
	{
		for(unsigned int j = 0; j < file_grid[0].size() ; j++)
		{
			if(file_grid[i][j] == 1)
			{
				grid[i][j] = new Block(Block::createBlockAnimationSet(state->tilesetDirt), i, j, 1);
				world->addBody(grid[i][j]->body);
				retile(grid[i][j]);
			}
			else if(file_grid[i][j] == 2)
			{
				grid[i][j] = new Block(Block::createBlockAnimationSet(state->tilesetStone), i, j, 2);
				world->addBody(grid[i][j]->body);
				retile(grid[i][j]);
			}
			else if(file_grid[i][j] == 3)
			{
				grid[i][j] = new Block(Block::createBlockAnimationSet(state->tilesetWater, 3, 1.0), i, j, 3, true);
				world->addBody(grid[i][j]->body);
				retile(grid[i][j]);
			}
			else if(file_grid[i][j] == 4)
			{
				grid[i][j] = new Block(Block::createBlockAnimationSet(state->tilesetGrass), i, j, 1);
				world->addBody(grid[i][j]->body);
				retile(grid[i][j]);
			}
		}
	}
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

	delete world;
	delete background;
//	delete visibleArea;
}

void Map::saveToFile(const string& filename)
{
	std::ofstream stream(filename.c_str());

	if(stream.is_open())
	{
		for(unsigned int j = 0; j < grid[0].size() ; j++)
		{
			for(unsigned int i = 0; i < grid.size() ; i++)
			{
				stream << (grid[i][j] != null);
				if(i != grid.size() - 1)
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

void Map::addBlock(int mx, int my)
{
	grid[mx][my] = new Block(Block::createBlockAnimationSet(state.tilesetDirt), mx, my, 1);
	world->addBody(grid[mx][my]->body);
	retile(grid[mx][my]);
}

void Map::deleteBlock(int mx, int my)
{
	world->destroyBody(grid[mx][my]->body);
	delete grid[mx][my];
	grid[mx][my] = NULL;
	retileNeighbourhood(mx, my);
}

/** Draws all the blocks that backgrounds the player */
void Map::draw_bg_player()
{
	background->scale.x = fgeal::Display::getInstance().getWidth()  / background->getCurrentFrame().w;
	background->scale.y = fgeal::Display::getInstance().getHeight() / background->getCurrentFrame().h;
	background->draw(0, 0);

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


