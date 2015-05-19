/*
 * map.hpp
 *
 *  Created on: 24/06/2013
 *      Author: carlosfaruolo
 */

#ifndef MAP_HPP_
#define MAP_HPP_

#include "block.hpp"
#include "physics.hpp"

using Physics::World;

struct Map
{
	/** The map grid, with all block slots */
	vector< vector<Block*> > grid;

	/** Default is a black background */
	AnimationSet *background;

	/** The visible area of this map */
	Rect* visibleArea;

	Map(AnimationSet* bg, int columns, int lines, Rect* visibleArea);

	Map(Image* bg, int columns, int lines, Rect* visibleArea);

	~Map();

	static Map* loadMapFromFile(const string& filename, World* world);
	static void saveRawMapToFile(const string& filename, Map* map);

	void retile(Block* b, bool recursive=true);

	void retileNeighbourhood(int x_grid_pos, int y_grid_pos);

	void addBlock(int x, int y);

	/** Draws all the blocks in this map */
	void draw();

	/** Draws all the blocks that backgrounds the player */
	void draw_bg_player();

	/** Draws all the blocks that foregrounds the player */
	void draw_fg_player();

	Rect computeDimensions();
};


#endif /* MAP_HPP_ */
