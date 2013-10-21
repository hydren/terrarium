/*
 * map.hpp
 *
 *  Created on: 24/06/2013
 *      Author: carlosfaruolo
 */

#ifndef MAP_HPP_
#define MAP_HPP_

#include "block.hpp"

struct Map
{
	/** The map grid, with all block slots */
	std::vector< std::vector<Block*> > grid;

	/** Default is a black background */
	AnimationSet *background;

	/** The visible area of this map */
	Rect* visibleArea;

	Map(AnimationSet* bg, int columns, int lines, Rect* visibleArea);

	Map(Image* bg, int columns, int lines, Rect* visibleArea);

	static Map* loadRawMapFromFile(String filename, b2World* world);
	static Map* loadRawMapFromFile(char* filename, b2World* world);

	static void saveRawMapToFile(String filename, Map* map);

	void retile(Block* b, bool recursive=true);

	void retileNeighbourhood(int x_grid_pos, int y_grid_pos);

	void addBlock(int x, int y);

	/** Draws all the blocks in this map */
	void draw();
};


#endif /* MAP_HPP_ */
