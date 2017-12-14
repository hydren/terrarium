/*
 * terrarium_game_logic.cpp
 *
 *  Created on: 14 de dez de 2017
 *      Author: carlosfaruolo
 */

#include "terrarium_game_logic.hpp"

#include "ingame_state.hpp"

using std::string;

TerrariumGameLogic::TerrariumGameLogic(TerrariumGame& game)
: game(game), config()
{
	config.load("config.properties");
}

void TerrariumGameLogic::setIngameStateStageFilename(string filename)
{
	static_cast<InGameState*>(game.getState(TerrariumGame::INGAME_STATE_ID))->stageFilename = filename;
}

void TerrariumGameLogic::setIngameCharacterFilename(std::string filename)
{
	static_cast<InGameState*>(game.getState(TerrariumGame::INGAME_STATE_ID))->characterFilename = filename;
}

