/*
 * main_menu_state.cpp
 *
 *  Created on: 20 de jun de 2017
 *      Author: carlosfaruolo
 */

#include "main_menu_state.hpp"

#include <algorithm>

using std::vector;
using std::string;

using fgeal::Event;
using fgeal::EventQueue;
using fgeal::Rectangle;
using fgeal::Color;

MainMenuState::MainMenuState(TerrariumGame* game)  // @suppress("Class members should be properly initialized")
:	State(*game), wasInit(false), fileMenu(null)
{}

MainMenuState::~MainMenuState()
{
	if(not wasInit) return;

	delete mainFont;
	delete minorFont;
	delete miniFont;
	delete devFont;
	delete background;

	delete mainMenu;
	delete fileMenu;
}

void MainMenuState::initialize()
{
	mainFont = new Font("resources/jack.ttf", 44);
	minorFont = new Font("resources/liberation.ttf", 24);
	miniFont = new Font("resources/jack.ttf", 16);
	devFont = new Font("resources/liberation.ttf", 14);
	background = new Image("resources/title_proto.jpg");

	fgeal::Display& display = fgeal::Display::getInstance();
	const float sw = display.getWidth(), sh = display.getHeight();
	Rectangle size = {0.1f*sw, 0.225f*sh, 0.5f*sw, 0.25f*sh};
	mainMenu = new Menu(size, minorFont, Color::ORANGE);
	mainMenu->bgColor = Color(0, 0, 0, 96);
	mainMenu->borderColor = Color::TRANSPARENT;
	mainMenu->addEntry("Generate new map");
	mainMenu->addEntry("Load map from file");
	mainMenu->addEntry("Settings");
	mainMenu->addEntry("Exit");
}

void MainMenuState::onEnter()
{
	fgeal::Display& display = fgeal::Display::getInstance();
	const float sw = display.getWidth(), sh = display.getHeight();
	vector<string> dirs = fgeal::getFilenamesWithinDirectory("./resources/maps");

	const float fileMenuHeight = std::min(sh, (dirs.size()+3) * miniFont->getFontHeight());

	Rectangle size = {0.25f*sw, 0.5f*(sh-fileMenuHeight), 0.5f*sw, fileMenuHeight};

	if(fileMenu != null) delete fileMenu;
	fileMenu = new Menu(size, miniFont, Color::YELLOW, "Which file?");
	fileMenu->focusedEntryFontColor = Color::AZURE;
	fileMenu->titleColor = Color::WHITE;
	fileMenu->bgColor = Color(0, 0, 0, 128);
	for(vector<string>::iterator it = dirs.begin(); it != dirs.end() ; ++it)
		fileMenu->addEntry(*it);
	fileMenu->addEntry("< Cancel >");

	chooseFile = false;
}

void MainMenuState::onLeave() {}

void MainMenuState::render()
{
	fgeal::Display& display = fgeal::Display::getInstance();
	const float sw = display.getWidth(), sh = display.getHeight();

	display.clear();
	background->drawScaled(0, 0, sw/background->getWidth(), sh/background->getHeight());
	mainFont->drawText("Project Terrarium", sw*0.125, sh*0.05, Color::ORANGE);
	mainMenu->draw();
	if(chooseFile)
		fileMenu->draw();

	devFont->drawText(string("Using fgeal v")+fgeal::VERSION+" on "+fgeal::ADAPTED_LIBRARY_NAME+" v"+fgeal::ADAPTED_LIBRARY_VERSION, 4, fgeal::Display::getInstance().getHeight() - devFont->getFontHeight(), Color::CREAM);
}

void MainMenuState::update(float delta)
{
	Event ev;
	EventQueue& eventQueue = EventQueue::getInstance();
	while(not eventQueue.isEmpty() )
	{
		eventQueue.waitNextEvent(&ev);

		if(ev.getEventType() == fgeal::Event::Type::DISPLAY_CLOSURE)
		{
			game.running = false;
		}
		else if(ev.getEventType() == fgeal::Event::Type::KEY_PRESS)
		{
			switch(ev.getEventKeyCode())
			{
				case fgeal::Keyboard::Key::ARROW_UP:
					if(chooseFile)
						fileMenu->cursorUp();
					else
						mainMenu->cursorUp();

					break;

				case fgeal::Keyboard::Key::ARROW_DOWN:
					if(chooseFile)
						fileMenu->cursorDown();
					else
						mainMenu->cursorDown();

					break;

				case fgeal::Keyboard::Key::ARROW_RIGHT:
					break;

				case fgeal::Keyboard::Key::ARROW_LEFT:
					break;

				case fgeal::Keyboard::Key::ENTER:

					if(chooseFile == true)
					{
						if(fileMenu->getSelectedIndex() == fileMenu->getNumberOfEntries()-1)
							chooseFile = false;
						else
						{
							//loading_image.draw();
							//fgeal::display->refresh();
							TerrariumGame& game = *static_cast<TerrariumGame*>(&this->game);
							game.stageFilename = fileMenu->getSelectedEntry().label;
							game.enterState(TerrariumGame::INGAME_STATE_ID);
						}
					}
					else switch(mainMenu->getSelectedIndex()) //this isn't elegant...
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
