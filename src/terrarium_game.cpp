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
#include "options_menu_state.hpp"

void TerrariumGame::initializeStatesList()
{
	this->addState(new MainMenuState(this));  // set initial by position
	this->addState(new InGameState(this));
	this->addState(new LoadingState(this));
	this->addState(new OptionsMenuState(this));
}

TerrariumGame::TerrariumGame()
: Game(), logic(*this)
{
	maxFps = 60;
}
