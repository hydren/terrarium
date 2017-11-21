/*
 * options_menu_state.cpp
 *
 *  Created on: 5 de out de 2017
 *      Author: carlosfaruolo
 */

#include "options_menu_state.hpp"
#include "futil/string_actions.hpp"
#include "resolutions.hpp"

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
  isAspectRatioMenuActive(), isResolutionMenuActive()
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

	menuMain = new Menu(Rectangle(), fntMenu, Color::ORANGE);
	menuMain->bgColor = Color(0, 0, 0, 96);
	menuMain->borderColor = Color::_TRANSPARENT;
	menuMain->addEntry("Resolution");
	menuMain->addEntry("Aspect ratio: all");
	menuMain->addEntry("Fullscreen");
	menuMain->addEntry("Exit");

	menuAspectRatio = new Menu(Rectangle(), fntMenu, Color::OLIVE);
	menuAspectRatio->bgColor = Color(0, 0, 0, 96);
	menuAspectRatio->borderColor = Color::_TRANSPARENT;
	menuAspectRatio->addEntry("all");
	for(unsigned i = 0; i < Resolution::getAspects().size(); i++)
	{
		const std::pair<unsigned, unsigned> aspectRatio = Resolution::getAspects()[i];
		menuAspectRatio->addEntry(futil::to_string(aspectRatio.first)+":"+futil::to_string(aspectRatio.second));
	}

	menuResolution = new Menu(Rectangle(), fntMenu, Color::PURPLE);
	menuResolution->bgColor = Color(0, 0, 0, 96);
	menuResolution->borderColor = Color::_TRANSPARENT;
	for(unsigned i = 0; i < Resolution::get().size(); i++)
	{
		Resolution resolution = Resolution::get()[i];
		menuResolution->addEntry(futil::to_string(resolution.width)+"x"+futil::to_string(resolution.height));
	}
}

void OptionsMenuState::onEnter()
{
	isAspectRatioMenuActive = isResolutionMenuActive = false;
}

void OptionsMenuState::onLeave()
{}

void OptionsMenuState::render()
{
	Display& display = game.getDisplay();
	display.clear();
	imgBackground->drawScaled(0, 0, display.getWidth()/(float) imgBackground->getWidth(), display.getHeight()/(float) imgBackground->getHeight());

	// update menu bounds
	menuAspectRatio->bounds.x = 0.25f*display.getWidth();
	menuAspectRatio->bounds.y = 0.25f*display.getHeight();
	menuAspectRatio->bounds.w = display.getWidth() - 2*menuAspectRatio->bounds.x;
	menuAspectRatio->bounds.h = 0.5f*display.getHeight();

	menuResolution->bounds.x = 0.25f*display.getWidth();
	menuResolution->bounds.y = 0.25f*display.getHeight();
	menuResolution->bounds.w = display.getWidth() - 2*menuResolution->bounds.x;
	menuResolution->bounds.h = 0.5f*display.getHeight();

	menuMain->bounds.x = 0.25f*display.getWidth();
	menuMain->bounds.y = 0.25f*display.getHeight();
	menuMain->bounds.w = display.getWidth() - 2*menuMain->bounds.x;
	menuMain->bounds.h = 0.5f*display.getHeight();

	updateLabels();

	if(isAspectRatioMenuActive)
		menuAspectRatio->draw();
	else if(isResolutionMenuActive)
		menuResolution->draw();
	else
		menuMain->draw();

	fntTitle->drawText("Options", 0.125*display.getWidth(), 0.0625*display.getHeight(), Color::WHITE);
}

void OptionsMenuState::update(float delta)
{
	if(isAspectRatioMenuActive) updateOnAspectRatioMenu();
	else if(isResolutionMenuActive) updateOnResolutionMenu();
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
							isAspectRatioMenuActive = true;

						if(menuMain->getSelectedIndex() == 2)
							game.getDisplay().setFullscreen(!game.getDisplay().isFullscreen());

						if(menuMain->getSelectedIndex() == menuMain->getEntryCount()-1)
							game.enterState(TerrariumGame::MAIN_MENU_STATE_ID);

						break;
					}
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
}

void OptionsMenuState::updateLabels()
{
	Display& display = game.getDisplay();
	menuMain->at(0).label = string("Resolution: ") + futil::to_string(display.getWidth()) + "x" + futil::to_string(display.getHeight());
	menuMain->at(2).label = string("Fullscreen: ") + (display.isFullscreen()? " yes" : " no");
}

void OptionsMenuState::updateOnAspectRatioMenu()
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
					isAspectRatioMenuActive = false;
					break;
				case Keyboard::KEY_ENTER:
				{
					menuResolution->setSelectedIndex(0);

					for(unsigned i = 0; i < menuResolution->getEntryCount(); i++)
						menuResolution->removeEntry(i);

					if(menuAspectRatio->getSelectedIndex() == 0)
					{
						for(unsigned i = 0; i < Resolution::get().size(); i++)
						{
							Resolution resolution = Resolution::get()[i];
							menuResolution->addEntry(futil::to_string(resolution.width)+"x"+futil::to_string(resolution.height));
						}
						menuMain->at(1).label = string("Aspect ratio: all");
					}
					else
					{
						const std::pair<unsigned, unsigned> aspectRatio = Resolution::getAspects()[menuAspectRatio->getSelectedIndex()-1];
						for(unsigned i = 0; i < Resolution::get(aspectRatio).size(); i++)
						{
							Resolution resolution = Resolution::get(aspectRatio)[i];
							menuResolution->addEntry(futil::to_string(resolution.width)+"x"+futil::to_string(resolution.height));
						}
						menuMain->at(1).label = string("Aspect ratio: ") + futil::to_string(aspectRatio.first) + ":" + futil::to_string(aspectRatio.second);
					}

					isAspectRatioMenuActive = false;
					break;
				}
				case Keyboard::KEY_ARROW_UP:
					menuAspectRatio->cursorUp();
					break;
				case Keyboard::KEY_ARROW_DOWN:
					menuAspectRatio->cursorDown();
					break;
				default:
					break;
			}
		}
	}
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
					Resolution resolution = (menuAspectRatio->getSelectedIndex() == 0)? Resolution::get()[menuResolution->getSelectedIndex()]
					: Resolution::get(Resolution::getAspects()[menuAspectRatio->getSelectedIndex()-1])[menuResolution->getSelectedIndex()];

					game.getDisplay().setSize(resolution.width, resolution.height);

					delete fntTitle;
					fntTitle = new Font("resources/jack.ttf", resolution.height*(44.0/480.0));

					isResolutionMenuActive = false;
					break;
				}
				case Keyboard::KEY_ARROW_UP:
					menuResolution->cursorUp();
					break;
				case Keyboard::KEY_ARROW_DOWN:
					menuResolution->cursorDown();
					break;
				default:
					break;
			}
		}
	}
}
