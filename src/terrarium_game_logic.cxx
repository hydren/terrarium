/*
 * terrarium_game_logic.cxx
 *
 *  Created on: 24 de nov de 2017
 *      Author: carlosfaruolo
 */

#include "terrarium_game.hpp"

TerrariumGame::Logic::Logic()
: config(), stageFilename()
{
	config.load("config.properties");
}
