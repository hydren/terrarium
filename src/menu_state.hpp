/*
 * menu_state.hpp
 *
 *  Created on: 11 de dez de 2016
 *      Author: felipe
 */

#ifndef MENU_STATE_HPP_
#define MENU_STATE_HPP_
#include <ciso646>

#include "terrarium_game.hpp"

#include "futil/general/language.hpp"
#include "fgeal/fgeal.hpp"
#include "fgeal/extra/game.hpp"

#include "menu.hpp"

class MenuState extends public fgeal::Game::State
{
	struct implementation;
	friend class implementation;
	implementation& self;

	public:
	int getId() { return TerrariumGame::MENU_STATE_ID; }

	MenuState(TerrariumGame* game);
	~MenuState();

	void initialize();
	void onEnter();
	void onLeave();

	void render();
	void update(float delta);
};

#endif /* MENU_STATE_HPP_ */
