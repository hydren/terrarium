/*
 * terrarium_game.cpp
 *
 *  Created on: 12 de dez de 2016
 *      Author: felipe
 */

#include "terrarium_game.hpp"

#include "menu_state.hpp"
#include "ingame_state.hpp"

const int
TerrariumGame::MENU_STATE_ID = 0,
TerrariumGame::INGAME_STATE_ID = 1;

void TerrariumGame::initializeStatesList()
{
	this->addState(new MenuState(this));
	this->addState(new InGameState(this));
}
