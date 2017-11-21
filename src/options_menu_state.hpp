/*
 * options_menu_state.hpp
 *
 *  Created on: 5 de out de 2017
 *      Author: carlosfaruolo
 */

#ifndef OPTIONS_MENU_STATE_HPP_
#define OPTIONS_MENU_STATE_HPP_
#include <ciso646>

#include "terrarium_game.hpp"

#include "futil/language.hpp"

#include "fgeal/fgeal.hpp"
#include "fgeal/extra/game.hpp"
#include "fgeal/extra/menu.hpp"

class OptionsMenuState extends public fgeal::Game::State
{
	public:
	int getId();

	bool wasInit;

	fgeal::Image* imgBackground;
	fgeal::Font* fntTitle, *fntMenu, *fntDev;

	fgeal::Menu* menuMain, *menuAspectRatio, *menuResolution;

	bool isAspectRatioMenuActive, isResolutionMenuActive;

	OptionsMenuState(TerrariumGame* game);
	~OptionsMenuState();

	void initialize();
	void onEnter();
	void onLeave();

	void render();
	void update(float delta);

	private:
	void updateOnAspectRatioMenu();
	void updateOnResolutionMenu();

	void updateLabels();
};

#endif /* OPTIONS_MENU_STATE_HPP_ */
