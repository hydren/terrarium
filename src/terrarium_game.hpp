/*
 * terrarium_game.hpp
 *
 *  Created on: 11 de dez de 2016
 *      Author: felipe
 */

#ifndef TERRARIUM_GAME_HPP_
#define TERRARIUM_GAME_HPP_
#include <ciso646>

#include "terrarium_game_logic.hpp"

#include "futil/language.hpp"
#include "fgeal/extra/game.hpp"

extern const std::string VERSION;

class TerrariumGame extends public fgeal::Game
{
	public:
	enum StateID
	{
		MAIN_MENU_STATE_ID,
		INGAME_STATE_ID,
		LOADING_STATE_ID,
		OPTIONS_STATE_ID
	};

	TerrariumGame();
	void initializeStatesList();

	TerrariumGameLogic logic;
};

#endif /* TERRARIUM_GAME_HPP_ */
