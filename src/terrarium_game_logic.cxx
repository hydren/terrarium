/*
 * terrarium_game_logic.cxx
 *
 *  Created on: 24 de nov de 2017
 *      Author: carlosfaruolo
 */

#include "terrarium_game.hpp"
#include "ingame_state.hpp"

using std::string;

TerrariumGame::Logic::Logic(TerrariumGame& game)
: game(game), config()
{
	config.load("config.properties");
}

void TerrariumGame::Logic::setIngameStateStageFilename(string filename)
{
	static_cast<InGameState*>(game.getState(TerrariumGame::INGAME_STATE_ID))->stageFilename = filename;
}

void TerrariumGame::Logic::setIngameCharacterFilename(std::string filename)
{
	static_cast<InGameState*>(game.getState(TerrariumGame::INGAME_STATE_ID))->characterFilename = filename;
}
