/*
 * game.hpp
 *
 *  Created on: 11/05/2013
 *      Author: felipe
 */

#ifndef GAME_HPP_
#define GAME_HPP_

#include "fgeal.hpp"

#include <string>

fgeal::Rectangle createRectangle(float x, float y, float w, float h);

struct Game
{
	Game(const std::string& map_path);
	~Game();
	void start();
};


#endif /* GAME_HPP_ */
