/*
 * terrarium_game_logic.hxx
 *
 *  Created on: 24 de nov de 2017
 *      Author: carlosfaruolo
 */

/** Class to wrap together all between-states game logic. */
class Logic
{
	friend class TerrariumGame;

	TerrariumGame& game;

	Logic(TerrariumGame&);

	public:
	Properties config;

	void setIngameStateStageFilename(std::string filename);
	void setIngameCharacterFilename(std::string filename);
};
