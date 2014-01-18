/*
 * game.hpp
 *
 *  Created on: 11/05/2013
 *      Author: felipe
 */

#ifndef GAME_HPP_
#define GAME_HPP_

#include "entity.hpp"
#include "engine.hpp"
#include "menu.hpp"
#include "block.hpp"
#include "map.hpp"

struct Game
{
	Game(const string& map_path);
	void start();
};


#endif /* GAME_HPP_ */
