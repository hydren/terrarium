/*
 * terrarium_game.cpp
 *
 *  Created on: 12 de dez de 2016
 *      Author: felipe
 */

#include "terrarium_game.hpp"

#include "main_menu_state.hpp"
#include "ingame_state.hpp"
#include "loading_state.hpp"

const int
TerrariumGame::MAIN_MENU_STATE_ID = 0,
TerrariumGame::INGAME_STATE_ID = 1,
TerrariumGame::LOADING_STATE_ID = 2,
TerrariumGame::OPTIONS_STATE_ID = 3;

futil::Properties TerrariumGame::CONFIG;

void TerrariumGame::initializeStatesList()
{
	this->addState(new MainMenuState(this));  // set initial by position
	this->addState(new InGameState(this));
	this->addState(new LoadingState(this));
}

TerrariumGame::TerrariumGame()
{
	maxFps = 60;
}
