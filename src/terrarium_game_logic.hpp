/*
 * terrarium_game_logic.hpp
 *
 *  Created on: 14 de dez de 2017
 *      Author: carlosfaruolo
 */

#ifndef TERRARIUM_GAME_LOGIC_HPP_
#define TERRARIUM_GAME_LOGIC_HPP_
#include <ciso646>

#include "futil/properties.hpp"

//fwd. decl.
class TerrariumGame;

/** Class to wrap together all between-states game logic. */
struct TerrariumGameLogic
{
	friend class TerrariumGame;
	TerrariumGame& game;

	TerrariumGameLogic(TerrariumGame&);

	public:
	futil::Properties config;

	void setIngameStateStageFilename(std::string filename);
	void setIngameCharacterFilename(std::string filename);
};

#endif /* TERRARIUM_GAME_LOGIC_HPP_ */
