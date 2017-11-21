/*
 * options_menu_state.cpp
 *
 *  Created on: 5 de out de 2017
 *      Author: carlosfaruolo
 */

#include "options_menu_state.hpp"
#include "futil/string_actions.hpp"

using fgeal::Display;
using fgeal::Event;
using fgeal::EventQueue;
using fgeal::Keyboard;
using fgeal::Rectangle;
using fgeal::Color;
using fgeal::Image;
using fgeal::Font;
using fgeal::Menu;
using std::string;

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
	Display& display = game.getDisplay();
	fntTitle = new Font("resources/jack.ttf", display.getHeight()*(44.0/480.0));
	fntMenu = new Font("resources/liberation.ttf", display.getHeight()*(24.0/480.0));
	fntDev = new Font("resources/liberation.ttf", display.getHeight()*(14.0/480.0));

	imgBackground = new Image("resources/options.jpg");

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

void OptionsMenuState::onEnter()
{}

void OptionsMenuState::onLeave()
{}

void OptionsMenuState::render()
{
	Display& display = game.getDisplay();
	display.clear();
	imgBackground->drawScaled(0, 0, display.getWidth()/(float) imgBackground->getWidth(), display.getHeight()/(float) imgBackground->getHeight());

	// update menu bounds
	menuMain->bounds.x = 0.25f*display.getWidth();
	menuMain->bounds.y = 0.25f*display.getHeight();
	menuMain->bounds.w = display.getWidth() - 2*menuMain->bounds.x;
	menuMain->bounds.h = 0.5f*display.getHeight();

	updateLabels();
	menuMain->draw();

	fntTitle->drawText("Options", 0.125*display.getWidth(), 0.0625*display.getHeight(), Color::WHITE);
}

void OptionsMenuState::update(float delta)
{
	Event event;
	EventQueue& eventQueue = EventQueue::getInstance();
	while(eventQueue.hasEvents())
	{
		eventQueue.getNextEvent(&event);
		if(event.getEventType() == Event::TYPE_DISPLAY_CLOSURE)
		{
			game.running = false;
		}
		else if(event.getEventType() == Event::TYPE_KEY_PRESS)
		{
			switch(event.getEventKeyCode())
			{
				case Keyboard::KEY_ESCAPE:
					game.enterState(TerrariumGame::MAIN_MENU_STATE_ID);
					break;
				case Keyboard::KEY_ENTER:
					this->onMenuSelect();
					break;
				case Keyboard::KEY_ARROW_UP:
					menuMain->cursorUp();
					break;
				case Keyboard::KEY_ARROW_DOWN:
					menuMain->cursorDown();
					break;
				default:
					break;
			}
		}
	}
}

void OptionsMenuState::onMenuSelect()
{
	if(menuMain->getSelectedIndex() == 2)
		game.getDisplay().setFullscreen(!game.getDisplay().isFullscreen());

	if(menuMain->getSelectedIndex() == menuMain->getEntryCount()-1)
		game.enterState(TerrariumGame::MAIN_MENU_STATE_ID);
}

void OptionsMenuState::updateLabels()
{
	Display& display = game.getDisplay();
	menuMain->at(0).label = string("Resolution: ") + futil::to_string(display.getWidth()) + "x" + futil::to_string(display.getHeight());
	menuMain->at(1).label = string("Aspect ratio: ") + ("4:3");
	menuMain->at(2).label = string("Fullscreen: ") + (display.isFullscreen()? " yes" : " no");
}
