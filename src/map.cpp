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

#include "futil/language.hpp"
#include "futil/string_extra_operators.hpp"
#include "futil/string_actions.hpp"

#include "terrarium_game.hpp"
#include "ingame_state.hpp"

static Physics::Vector GRAVITY = {0.0, 10.0};

Map::Map(InGameState* state, int columns, int lines)
: state(*state), grid(), world(new Physics::World(GRAVITY))
{
	grid.resize(columns, vector<Block*>(lines));  // fill the matrix with null
}

Map::Map(InGameState* state, const string filename)
: state(*state), grid(), world(new Physics::World(GRAVITY))
{
	vector< vector<int> > file_grid;

	(futil::ends_with(filename, ".tmx")? parseGridFromFileTmx : parseGridFromFileTxt)(file_grid, filename);

	cout << "map size (in blocks): " << file_grid.size() << "x" << file_grid[0].size() << endl;

	grid.resize(file_grid.size(), vector<Block*>(file_grid[0].size()));  // fill the matrix with null

	for(unsigned int i = 0; i < file_grid.size() ; i++)
	{
		for(unsigned int j = 0; j < file_grid[0].size() ; j++)
		{
			const int typeId = file_grid[i][j];
			if(typeId != 0 and state->isBlockTypeIdExistent(typeId))
			{
				grid[i][j] = new Block(new Animation(state->tilesets[typeId]), i, j, typeId, state->isBlockTypeIdPassable(typeId));
				world->addBody(grid[i][j]->body);
				retile(grid[i][j]);
			}
			else if(typeId != 0)  // a non-registered non-zero type id is supposed to be wrong, so alert
			{
				cout << "at [" << i << ", " << j << "]: type id (" << typeId << ") not registered! ignoring..." << endl;
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
			{
//				cout << string("deleting block ") + b->gridX + ", "+b->gridY << endl;;
				delete b;
			}
		}
	}

	delete world;
//	delete visibleArea;
}

void Map::saveToFile(const string& filename)
{
	vector< vector<int> > idGrid(grid.size(), vector<int>(grid[0].size()));

	for(unsigned j = 0; j < grid[0].size() ; j++)
	{
		for(unsigned i = 0; i < grid.size() ; i++)
		{
			if(grid[i][j] != null)
				idGrid[i][j] = grid[i][j]->typeID;
		}
	}

	if(not futil::ends_with(filename, ".txt"))
		saveGridToFileTxt(idGrid, filename + ".txt");
	else
		saveGridToFileTxt(idGrid, filename);
}

void Map::retileNeighbourhood(unsigned gridX, unsigned gridY)
{
	if(gridX > 0                and grid[gridX-1][gridY] != null) retile(grid[gridX-1][gridY  ], false);
	if(gridX < grid.size()-1    and grid[gridX+1][gridY] != null) retile(grid[gridX+1][gridY  ], false);
	if(gridY > 0                and grid[gridX][gridY-1] != null) retile(grid[gridX  ][gridY-1], false);
	if(gridY < grid[0].size()-1 and grid[gridX][gridY+1] != null) retile(grid[gridX  ][gridY+1], false);
}

void Map::retile(Block* b, bool recursive)
{
	const bool hasLeft  = (b->gridX > 0                 and grid[b->gridX-1][b->gridY  ] != null),
			   hasRight = (b->gridX < grid.size() -1    and grid[b->gridX+1][b->gridY  ] != null),
			   hasNorth = (b->gridY > 0                 and grid[b->gridX  ][b->gridY-1] != null),
			   hasSouth = (b->gridY < grid[0].size() -1 and grid[b->gridX  ][b->gridY+1] != null);

	if(  hasNorth &&  hasSouth &&  hasLeft &&  hasRight) b->animation->currentIndex = Block::Anim::FULL_COVER;
	if( !hasNorth &&  hasSouth &&  hasLeft &&  hasRight) b->animation->currentIndex = Block::Anim::TOP_FREE;
	if(  hasNorth && !hasSouth &&  hasLeft &&  hasRight) b->animation->currentIndex = Block::Anim::BOTTOM_FREE;
	if(  hasNorth &&  hasSouth && !hasLeft &&  hasRight) b->animation->currentIndex = Block::Anim::LEFT_FREE;
	if(  hasNorth &&  hasSouth &&  hasLeft && !hasRight) b->animation->currentIndex = Block::Anim::RIGHT_FREE;
	if( !hasNorth &&  hasSouth && !hasLeft &&  hasRight) b->animation->currentIndex = Block::Anim::TOP_LEFT_FREE;
	if(  hasNorth && !hasSouth && !hasLeft &&  hasRight) b->animation->currentIndex = Block::Anim::BOTTOM_LEFT_FREE;
	if( !hasNorth &&  hasSouth &&  hasLeft && !hasRight) b->animation->currentIndex = Block::Anim::TOP_RIGHT_FREE;
	if(  hasNorth && !hasSouth &&  hasLeft && !hasRight) b->animation->currentIndex = Block::Anim::BOTTOM_RIGHT_FREE;
	if(  hasNorth &&  hasSouth && !hasLeft && !hasRight) b->animation->currentIndex = Block::Anim::HORIZONTAL_FREE;
	if( !hasNorth && !hasSouth &&  hasLeft &&  hasRight) b->animation->currentIndex = Block::Anim::VERTICAL_FREE;
	if( !hasNorth &&  hasSouth && !hasLeft && !hasRight) b->animation->currentIndex = Block::Anim::BOTTOM_COVERED;
	if(  hasNorth && !hasSouth && !hasLeft && !hasRight) b->animation->currentIndex = Block::Anim::TOP_COVERED;
	if( !hasNorth && !hasSouth && !hasLeft &&  hasRight) b->animation->currentIndex = Block::Anim::RIGHT_COVERED;
	if( !hasNorth && !hasSouth &&  hasLeft && !hasRight) b->animation->currentIndex = Block::Anim::LEFT_COVERED;
	if( !hasNorth && !hasSouth && !hasLeft && !hasRight) b->animation->currentIndex = Block::Anim::FULL_FREE;

//	std::cout << "hasNorth = " << hasNorth << ", hasSouth = " << hasSouth << ", hasLeft = " << hasLeft << ", hasRight = " << hasRight << std::endl;

	if(recursive)
	{
		if(hasLeft)  retile(grid[b->gridX-1][b->gridY], false);
		if(hasRight) retile(grid[b->gridX+1][b->gridY], false);
		if(hasNorth) retile(grid[b->gridX][b->gridY-1], false);
		if(hasSouth) retile(grid[b->gridX][b->gridY+1], false);
	}
}

Rectangle Map::computeDimensions()
{
	Rectangle size = { 0, 0, (float) grid.size()*BLOCK_SIZE, (float)(grid.size() > 0 ? grid[0].size()*BLOCK_SIZE : 0) };
	return size;
}

void Map::addBlock(unsigned gridX, unsigned gridY, unsigned typeId)
{
	grid[gridX][gridY] = new Block(new Animation(state.tilesets[typeId]), gridX, gridY, typeId, state.isBlockTypeIdPassable(typeId));
	world->addBody(grid[gridX][gridY]->body);
	retile(grid[gridX][gridY]);
}

void Map::deleteBlock(unsigned gridX, unsigned gridY)
{
	world->destroyBody(grid[gridX][gridY]->body);
	delete grid[gridX][gridY];
	grid[gridX][gridY] = null;
	retileNeighbourhood(gridX, gridY);
}

/** Draws all the blocks that backgrounds the player */
void Map::draw()
{
	int grid_number_of_lines = grid.capacity();
	int grid_number_of_columns = grid[0].capacity();

	const Rectangle& visibleArea = state.visibleArea;

	//draws all blocks that can be fully or partially seen, but not the ones that can't be seen
	int start_i_index = visibleArea.x / BLOCK_SIZE;
	int start_j_index = visibleArea.y / BLOCK_SIZE;
	int finish_i_index = ((visibleArea.x + visibleArea.w) / BLOCK_SIZE) + 1;
	int finish_j_index = (visibleArea.y + visibleArea.h) / BLOCK_SIZE;

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
			if( grid[i][j] != null and grid[i][j]->typeID != 3)
			{
				grid[i][j]->draw(visibleArea);
			}
		}
}

/** Draws all the blocks that foregrounds the player */
void Map::drawOverlay()
{
	int grid_number_of_lines = grid.capacity();
	int grid_number_of_columns = grid[0].capacity();

	const Rectangle& visibleArea = state.visibleArea;

	//draws all blocks that can be fully or partially seen, but not the ones that can't be seen
	int start_i_index = visibleArea.x / BLOCK_SIZE;
	int start_j_index = visibleArea.y / BLOCK_SIZE;
	int finish_i_index = ((visibleArea.x + visibleArea.w) / BLOCK_SIZE) + 1;
	int finish_j_index = (visibleArea.y + visibleArea.h) / BLOCK_SIZE;

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
			if( grid[i][j] != null and grid[i][j]->typeID == 3)
			{
				grid[i][j]->draw(visibleArea);
			}
		}
}

void Map::updatePrecipitables()
{
	int grid_number_of_lines = grid.capacity();
	int grid_number_of_columns = grid[0].capacity();

	const Rectangle& visibleArea = state.visibleArea;

	//draws all blocks that can be fully or partially seen, but not the ones that can't be seen
	int start_i_index = visibleArea.x / BLOCK_SIZE;
	int start_j_index = visibleArea.y / BLOCK_SIZE;
	int finish_i_index = ((visibleArea.x + visibleArea.w) / BLOCK_SIZE) + 1;
	int finish_j_index = (visibleArea.y + visibleArea.h) / BLOCK_SIZE;

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
		if( grid[i][j] != null and grid[i][j]->typeID != 0)
		{
			const int typeId = grid[i][j]->typeID;
			if(state.isBlockTypeIdExistent(typeId) and state.blockTypeInfo[typeId].precipitability != Block::Type::PRECIPITABILITY_NONE)
			{
				if(j+1 <= finish_j_index and grid[i][j+1] == null)  // free below
				{
					// send this block down
					addBlock(i, j+1, typeId);
					deleteBlock(i, j);
				}

				else if(j+1 <= finish_j_index and i > 0 and grid[i-1][j+1] == null   // If there’s an empty space down and to the left
						and (grid[i-1][j] == null or grid[i][j+1]->typeID == typeId))  // avoids inadequate diagonal passing
				{
					// send this block down and to the left
					addBlock(i-1, j+1, typeId);
					deleteBlock(i, j);
				}

				else if(j+1 <= finish_j_index and i+1 <= finish_i_index and grid[i+1][j+1] == null   // If there’s an empty space down and to the right
						and (grid[i+1][j] == null or grid[i][j+1]->typeID == typeId))  // avoids inadequate diagonal passing
				{
					// send this block down and to the right
					addBlock(i+1, j+1, typeId);
					deleteBlock(i, j);
				}

//				else if(i+1 <= finish_i_index and grid[i+1][j] == null  // free to the right
//						and j > 0 and grid[i][j-1] != null and grid[i][j-1]->typeID == typeId)  // have more of it above
//				{
//					// send block above to the right
//					addBlock(i+1, j, typeId);
//					deleteBlock(i, j-1);
//				}
//
//				else if(i > 0 and grid[i-1][j] == null  // free to the left
//						and j > 0 and grid[i][j-1] != null and grid[i][j-1]->typeID == typeId)  // have more of it above
//				{
//					// send block above to the right
//					addBlock(i-1, j, typeId);
//					deleteBlock(i, j-1);
//				}

				//todo make cases for side sliping liquidous movement
			}
		}
	}
}
