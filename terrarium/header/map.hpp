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

	static Map* loadRawMapFromFile(const String& filename, World* world);
	static Map* loadRawMapFromFile(const char* filename, World* world);
	static void saveRawMapToFile(const String& filename, Map* map);

	static Map* loadMapFromTMXFile(const String& filename, World* world);


	void retile(Block* b, bool recursive=true);

	void retileNeighbourhood(int x_grid_pos, int y_grid_pos);

	void addBlock(int x, int y);

	/** Draws all the blocks in this map */
	void draw();
};


#endif /* MAP_HPP_ */
