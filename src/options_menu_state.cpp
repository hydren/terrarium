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
using std::vector;

int OptionsMenuState::getId() { return TerrariumGame::OPTIONS_STATE_ID; }

OptionsMenuState::OptionsMenuState(TerrariumGame* game)
: State(*game), wasInit(false),
  imgBackground(null),
  fntTitle(null), fntMenu(null), fntDev(null),
  menuMain(null), menuResolution(null),
  isResolutionMenuActive()
{}

OptionsMenuState::~OptionsMenuState()
{
	if(not wasInit) return;

	delete imgBackground;
	delete fntTitle;
	delete fntMenu;
	delete fntDev;
	delete menuMain;
	delete menuResolution;
}

void OptionsMenuState::initialize()
{
	Display& display = game.getDisplay();
	fntTitle = new Font("resources/jack.ttf", display.getHeight()*(44.0/480.0));
	fntMenu = new Font("resources/liberation.ttf", display.getHeight()*(24.0/480.0));
	fntDev = new Font("resources/liberation.ttf", display.getHeight()*(14.0/480.0));

	imgBackground = new Image("resources/options.jpg");

	menuMain = new Menu(Rectangle(), fntMenu, Color::ORANGE);
	menuMain->bgColor = Color(0, 0, 0, 96);
	menuMain->borderColor = Color::_TRANSPARENT;
	menuMain->addEntry("Resolution");
	menuMain->addEntry("Fullscreen");
	menuMain->addEntry("Exit");

	menuResolution = new Menu(Rectangle(), fntDev, Color::ORANGE);
	menuResolution->bgColor = Color(0, 0, 0, 96);
	menuResolution->borderColor = Color::_TRANSPARENT;
	for(unsigned i = 0; i < Display::Mode::getList().size(); i++)
	{
		Display::Mode resolution = Display::Mode::getList()[i];
		menuResolution->addEntry(futil::to_string(resolution.width)+"x"+futil::to_string(resolution.height)
			+ " ("+futil::to_string(resolution.aspectRatio.first)+":"+futil::to_string(resolution.aspectRatio.second)+")"
			+ (resolution.description.empty()? "" : " ("+resolution.description+")"));
	}
}

void OptionsMenuState::onEnter()
{
	isResolutionMenuActive = false;
}

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

	menuResolution->bounds.x = 0.25f*display.getWidth();
	menuResolution->bounds.y = 0.25f*display.getHeight();
	menuResolution->bounds.w = display.getWidth() - 2*menuResolution->bounds.x;
	menuResolution->bounds.h = 0.5f*display.getHeight();

	updateLabels();

	if(isResolutionMenuActive)
		menuResolution->draw();
	else
		menuMain->draw();

	fntTitle->drawText("Options", 0.125*display.getWidth(), 0.0625*display.getHeight(), Color::WHITE);
}

void OptionsMenuState::update(float delta)
{
	if(isResolutionMenuActive) updateOnResolutionMenu();
	else
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
					{
						if(menuMain->getSelectedIndex() == 0)
							isResolutionMenuActive = true;

						if(menuMain->getSelectedIndex() == 1)
							game.getDisplay().setFullscreen(!game.getDisplay().isFullscreen());

						if(menuMain->getSelectedIndex() == menuMain->getEntries().size()-1)
							game.enterState(TerrariumGame::MAIN_MENU_STATE_ID);

						break;
					}
					case Keyboard::KEY_ARROW_UP:
						menuMain->moveCursorUp();
						break;
					case Keyboard::KEY_ARROW_DOWN:
						menuMain->moveCursorDown();
						break;
					default:
						break;
				}
			}
		}
	}
}

void OptionsMenuState::updateLabels()
{
	Display& display = game.getDisplay();
	menuMain->getEntryAt(0).label = string("Resolution: ") + futil::to_string(display.getWidth()) + "x" + futil::to_string(display.getHeight());
	menuMain->getEntryAt(1).label = string("Fullscreen: ") + (display.isFullscreen()? " yes" : " no");
}

void OptionsMenuState::updateOnResolutionMenu()
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
					isResolutionMenuActive = false;
					break;
				case Keyboard::KEY_ENTER:
				{
					Display::Mode resolution = Display::Mode::getList()[menuResolution->getSelectedIndex()];

					game.getDisplay().setSize(resolution.width, resolution.height);

					delete fntTitle;
					fntTitle = new Font("resources/jack.ttf", resolution.height*(44.0/480.0));

					isResolutionMenuActive = false;
					break;
				}
				case Keyboard::KEY_ARROW_UP:
					menuResolution->moveCursorUp();
					break;
				case Keyboard::KEY_ARROW_DOWN:
					menuResolution->moveCursorDown();
					break;
				default:
					break;
			}
		}
	}
}
