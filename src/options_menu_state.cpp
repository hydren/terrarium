/*
 * options_menu_state.cpp
 *
 *  Created on: 5 de out de 2017
 *      Author: carlosfaruolo
 */

#include "options_menu_state.hpp"

using fgeal::Display;
using fgeal::Rectangle;
using fgeal::Color;
using fgeal::Image;
using fgeal::Font;
using fgeal::Menu;

int OptionsMenuState::getId() { return TerrariumGame::OPTIONS_STATE_ID; }

OptionsMenuState::OptionsMenuState(TerrariumGame* game)
: State(*game), wasInit(false),
  imgBackground(null),
  fntTitle(null), fntMenu(null), fntDev(null),
  menuMain(null), menuAspectRatio(null), menuResolution(null),
  onAspectRatioMenu(), onResolutionMenu()
{}

OptionsMenuState::~OptionsMenuState()
{
	if(not wasInit) return;

	delete imgBackground;
	delete fntTitle;
	delete fntMenu;
	delete fntDev;
	delete menuMain;
	delete menuAspectRatio;
	delete menuResolution;
}

void OptionsMenuState::initialize()
{
	fntTitle = new Font("resources/jack.ttf", 44);
	fntMenu = new Font("resources/liberation.ttf", 24);
	fntDev = new Font("resources/liberation.ttf", 14);

	imgBackground = new Image("resources/options.jpg");

	Display& display = Display::getInstance();
	const float sw = display.getWidth(), sh = display.getHeight();
	Rectangle size = {0.1f*sw, 0.225f*sh, 0.5f*sw, 0.25f*sh};
	menuMain = new Menu(size, fntMenu, Color::ORANGE);
	menuMain->bgColor = Color(0, 0, 0, 96);
	menuMain->borderColor = Color::_TRANSPARENT;
	menuMain->addEntry("Resolution");
	menuMain->addEntry("Aspect ratio");
	menuMain->addEntry("Fullscreen");
	menuMain->addEntry("Exit");
}
