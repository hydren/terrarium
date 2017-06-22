/*
 * map.hpp
 *
 *  Created on: 24/06/2013
 *      Author: carlosfaruolo
 */

#ifndef MAP_HPP_
#define MAP_HPP_

#include <vector>
using std::vector;

#include "block.hpp"
#include "physics.hpp"

struct InGameState;

struct Map
{
	/** The map grid, with all block slots */
	vector< vector<Block*> > grid;

	/** Default is a black background */
	Sprite* background;

	/** An optional visible area for this map. If null, all map is drawn (slow). */
	Rectangle* visibleArea;

	/** The physics part of this map. */
	Physics::World* world;

	/** Creates an empty map, with the given number of columns and lines. */
	Map(int columns, int lines);

	/** Loads a map from a file. An optional background animation can be specified. */
	Map(InGameState* game, const string filename);

	~Map();

	void saveToFile(const string& filename);

	void retile(Block* b, bool recursive=true);

	void retileNeighbourhood(int x_grid_pos, int y_grid_pos);

	void addBlock(int x, int y);

	/** Draws all the blocks in this map */
	void draw();

	/** Draws all the blocks that backgrounds the player */
	void draw_bg_player();

	/** Draws all the blocks that foregrounds the player */
	void draw_fg_player();

	Rectangle computeDimensions();
};


#endif /* MAP_HPP_ */
