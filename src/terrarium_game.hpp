/*
 * terrarium_game.hpp
 *
 *  Created on: 11 de dez de 2016
 *      Author: felipe
 */

#ifndef TERRARIUM_GAME_HPP_
#define TERRARIUM_GAME_HPP_
#include <ciso646>

#include "futil/general/language.hpp"
#include "fgeal/extra/game.hpp"

class TerrariumGame extends public fgeal::Game
{
	public:
	static const int MENU_STATE_ID, INGAME_STATE_ID;

	std::string stageFilename;

	void initializeStatesList();
};

#endif /* TERRARIUM_GAME_HPP_ */
