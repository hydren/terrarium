/*
 * menu_state.cpp
 *
 *  Created on: 11 de dez de 2016
 *      Author: felipe
 */

#include "menu_state.hpp"

#include "util.hpp"

#include <vector>

using fgeal::Color;
using fgeal::Image;
using fgeal::Font;
using fgeal::EventQueue;

using std::vector;

MenuState::MenuState(TerrariumGame* game)
:	State(*game),
	eventQueue(null), background(null),
	mainFont(null), minorFont(null), miniFont(null),
	mainMenu(null), fileMenu(null), chooseFile(false)
{}

MenuState::~MenuState()
{
	if(mainFont != null) delete mainFont;
	if(minorFont != null) delete minorFont;
	if(miniFont != null) delete miniFont;
	if(background != null) delete background;
	if(eventQueue != null) delete eventQueue;

	if(mainMenu != null) delete mainMenu;
	if(fileMenu != null) delete fileMenu;
}

void MenuState::initialize()
{
	mainFont = new Font("resources/jack.ttf", 44);
	minorFont = new Font("resources/liberation.ttf", 24);
	miniFont = new Font("resources/jack.ttf", 16);
	background = new Image("resources/title_proto.jpg");
	eventQueue = new EventQueue();

	mainMenu = new Menu(createRectangle(64, 108, 300, 150), minorFont, Color::ORANGE);
	mainMenu->addEntry("Generate new map");
	mainMenu->addEntry("Load map from file");
	mainMenu->addEntry("Settings");
	mainMenu->addEntry("Exit");
}

void MenuState::onEnter()
{
	if(fileMenu != null) delete fileMenu;
	fileMenu = new Menu(createRectangle(32, 224, 294, 174), miniFont, Color::YELLOW, "Which file?");
	vector<string> dirs = fgeal::getFilenamesWithinDirectory("./resources/maps");
		for(vector<string>::iterator it = dirs.begin(); it != dirs.end() ; ++it)
			fileMenu->addEntry(*it);
	fileMenu->addEntry("< Cancel >");

	chooseFile = false;
	eventQueue->listenEvents();
}

void MenuState::onLeave()
{
	eventQueue->ignoreEvents();
}

void MenuState::render()
{
	background->draw();
	mainFont->drawText("Project Terrarium", 84, 25, Color::ORANGE);
	mainMenu->draw();
	if(chooseFile)
	{
		minorFont->drawText("Which file?", 202, 200, Color::BLACK);
		fileMenu->draw();
	}
}

void MenuState::update(float delta)
{
	while(! eventQueue->isEmpty() )
	{
		fgeal::Event ev;
		eventQueue->waitForEvent(&ev);

		if(ev.getEventType() == fgeal::Event::Type::DISPLAY_CLOSURE)
		{
			game.running = false;
		}
		else if(ev.getEventType() == fgeal::Event::Type::KEY_PRESS)
		{
			switch(ev.getEventKeyCode())
			{
				case fgeal::Event::Key::ARROW_UP:
					if(chooseFile)
						--*fileMenu;
					else
						--*mainMenu;

					break;

				case fgeal::Event::Key::ARROW_DOWN:
					if(chooseFile)
						++*fileMenu;
					else
						++*mainMenu;

					break;

				case fgeal::Event::Key::ARROW_RIGHT:
					break;

				case fgeal::Event::Key::ARROW_LEFT:
					break;

				case fgeal::Event::Key::ENTER:

					if(chooseFile == true)
					{
						if(fileMenu->selectedIndex == fileMenu->getNumberOfEntries()-1)
							chooseFile = false;
						else
						{
							//loading_image.draw();
							//fgeal::display->refresh();
							static_cast<TerrariumGame&>(game).stageFilename = fileMenu->getSelectedEntry()->label;
							game.enterState(TerrariumGame::INGAME_STATE_ID);
						}
					}
					else switch(mainMenu->selectedIndex) //this isn't elegant...
					{
						case 1:
							chooseFile = true;
							break;

						case 3:
							game.running = false;
							break;

						default:
							break;
					}
					break;

				default:
					break;
			}
		}
	}
}

