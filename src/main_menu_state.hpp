/*
 * main_menu_state.hpp
 *
 *  Created on: 20 de jun de 2017
 *      Author: carlosfaruolo
 */

#ifndef MAIN_MENU_STATE_HPP_
#define MAIN_MENU_STATE_HPP_
#include <ciso646>

#include "terrarium_game.hpp"

#include "futil/general/language.hpp"

#include "fgeal/fgeal.hpp"
#include "fgeal/extra/game.hpp"
#include "fgeal/extra/menu.hpp"

using fgeal::Image;
using fgeal::Font;
using fgeal::Menu;

class MainMenuState extends public fgeal::Game::State
{
	public:
	int getId() { return TerrariumGame::MAIN_MENU_STATE_ID; }

	bool wasInit;

	fgeal::Image* background;
	fgeal::Font* mainFont, *minorFont, *miniFont, *devFont;

	Menu* mainMenu, *fileMenu;

	bool chooseFile;

	MainMenuState(TerrariumGame* game);
	~MainMenuState();

	void initialize();
	void onEnter();
	void onLeave();

	void render();
	void update(float delta);
};

#endif /* MAIN_MENU_STATE_HPP_ */
