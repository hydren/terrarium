/*
 * ingame_state.hpp
 *
 *  Created on: 12 de dez de 2016
 *      Author: carlosfaruolo
 */

#ifndef INGAME_STATE_HPP_
#define INGAME_STATE_HPP_
#include <ciso646>

#include "terrarium_game.hpp"

#include "futil/general/language.hpp"

#include "fgeal/extra/game.hpp"

class InGameState extends public fgeal::Game::State
{
	struct implementation;
	friend class implementation;
	implementation& self;

	public:
	int getId() { return TerrariumGame::INGAME_STATE_ID; }

	InGameState(TerrariumGame* game);
	~InGameState();

	void initialize();
	void onEnter();
	void onLeave();

	void render();
	void update(float delta);
};

#endif /* INGAME_STATE_HPP_ */
