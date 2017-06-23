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

	/** Default is a black background */
	Sprite* background;

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

	void addBlock(unsigned gridX, unsigned gridY);

	void deleteBlock(unsigned gridX, unsigned gridY);

	/** Draws all the blocks in this map */
	void draw();

	/** Draws all the blocks that backgrounds the player */
	void draw_bg_player();

	/** Draws all the blocks that foregrounds the player */
	void draw_fg_player();

	Rectangle computeDimensions();

	private:
	/// Loads the map grid from a raw txt file, based on the old terrarium "SDL" prototype
	vector< vector<int> > parseGridFromRawTxtFile(const string& file_path);

	/// Loads the map grid from a Tiled TMX file
	vector< vector<int> > parseGridFromTMXFile(const string& file_path);

	/// Saves the map grid to a ram txt file.
	void saveGridToRawTxtFile(vector< vector<int> > grid, const string& filename);
};

#endif /* MAP_HPP_ */
