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

#include <string>
using std::string;

#include "block.hpp"
#include "physics.hpp"

struct InGameState;

struct Map
{
	/** In-game state */
	InGameState& state;

	/** The map grid, with all block slots */
	vector< vector<Block*> > grid;

	/** The physics part of this map. */
	Physics::World* world;

	/** Creates an empty map, with the given number of columns and lines. */
	Map(InGameState* state, int columns, int lines);

	/** Loads a map from a file. An optional background animation can be specified. */
	Map(InGameState* state, const string filename);

	~Map();

	void saveToFile(const string& filename);

	void retile(Block* b, bool recursive=true);

	void retileNeighbourhood(unsigned gridX, unsigned gridY);

	void addBlock(unsigned gridX, unsigned gridY, unsigned typeId);

	void deleteBlock(unsigned gridX, unsigned gridY);

	/** Draws all the blocks that backgrounds the player */
	void draw();

	/** Draws all the blocks that foregrounds the player */
	void drawOverlay();

	Rectangle computeDimensions();

	private:

	/// Loads the map grid from a raw txt file, based on the old terrarium "SDL" prototype
	static void parseGridFromFileTxt(vector< vector<int> >& grid, const string& filename);

	/// Loads the map grid from a Tiled TMX file
	static void parseGridFromFileTmx(vector< vector<int> >& grid, const string& filename);

	/// Saves the map grid to a ram txt file.
	static void saveGridToFileTxt(const vector< vector<int> >& grid, const string& filename);
};

#endif /* MAP_HPP_ */
