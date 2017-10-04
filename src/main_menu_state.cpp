/*
 * main_menu_state.cpp
 *
 *  Created on: 20 de jun de 2017
 *      Author: carlosfaruolo
 */

#include "main_menu_state.hpp"

#include "map.hpp"

#include "libgeramun/generators.hpp"

#include "fgeal/filesystem.hpp"

#include "futil/random.h"

#include <algorithm>
#include <iostream>

#include <cstdio>

using std::vector;
using std::string;

using fgeal::Event;
using fgeal::Font;
using fgeal::Menu;
using fgeal::Image;
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

	background = new Image("resources/bg-day-clear.jpg");
	imgCloud = new Image("resources/cloud.png");
	imgSun = new Image("resources/sun.png");

	fgeal::Display& display = fgeal::Display::getInstance();
	const float sw = display.getWidth(), sh = display.getHeight();
	Rectangle size = {0.1f*sw, 0.225f*sh, 0.5f*sw, 0.25f*sh};
	mainMenu = new Menu(size, minorFont, Color::ORANGE);
	mainMenu->bgColor = Color(0, 0, 0, 96);
	mainMenu->borderColor = Color::_TRANSPARENT;
	mainMenu->addEntry("Generate new map");
	mainMenu->addEntry("Load map from file");
	mainMenu->addEntry("Settings");
	mainMenu->addEntry("Exit");
}

void MainMenuState::onEnter()
{
	fgeal::Display& display = fgeal::Display::getInstance();
	const float sw = display.getWidth(), sh = display.getHeight();
	vector<string> dirs = fgeal::filesystem::getFilenamesWithinDirectory("./resources/maps");

	const float fileMenuHeight = std::min(sh, (dirs.size()+3) * miniFont->getHeight());

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
	mapGenerationRequested = false;

	cloudies.clear();
	cloudies.resize(16);
	for(unsigned i = 0; i < cloudies.size(); i++)
	{
		cloudies[i].x = futil::random_between_decimal(0.1, 0.9)*sw;
		cloudies[i].y = futil::random_between_decimal(0.1, 0.9)*sh;
		cloudies[i].w = futil::random_between_decimal(0.1, 0.2)*sw/imgCloud->getWidth();
		cloudies[i].h = futil::random_between(-1, 2);
	}
}

void MainMenuState::onLeave() {}

void MainMenuState::render()
{
	fgeal::Display& display = fgeal::Display::getInstance();
	const float sw = display.getWidth(), sh = display.getHeight();

	display.clear();
	background->drawScaled(0, 0, sw/background->getWidth(), sh/background->getHeight());
	imgSun->drawScaled(0.75*sw, 0.25*sh, 0.125*sw/imgSun->getWidth(), 0.125*sw/imgSun->getHeight());
	for(unsigned i = 0; i < cloudies.size(); i++)
		imgCloud->drawScaled(cloudies[i].x, cloudies[i].y,
							 cloudies[i].w, cloudies[i].w,
							 (cloudies[i].h < 0? Image::FLIP_HORIZONTAL : cloudies[i].h > 0? Image::FLIP_VERTICAL : Image::FLIP_NONE));

	mainFont->drawText("Project Terrarium", sw*0.125, sh*0.05, Color::ORANGE);
	mainMenu->draw();
	if(chooseFile)
		fileMenu->draw();

	devFont->drawText(string("Using fgeal v")+fgeal::VERSION+" on "+fgeal::ADAPTED_LIBRARY_NAME+" v"+fgeal::ADAPTED_LIBRARY_VERSION, 4, fgeal::Display::getInstance().getHeight() - devFont->getHeight(), Color::CREAM);
}

void MainMenuState::update(float delta)
{
	Event ev;
	EventQueue& eventQueue = EventQueue::getInstance();
	while(not eventQueue.isEmpty() )
	{
		eventQueue.waitNextEvent(&ev);

		if(ev.getEventType() == fgeal::Event::TYPE_DISPLAY_CLOSURE)
		{
			game.running = false;
		}
		else if(ev.getEventType() == fgeal::Event::TYPE_KEY_PRESS)
		{
			switch(ev.getEventKeyCode())
			{
				case fgeal::Keyboard::KEY_ARROW_UP:
					if(chooseFile)
						fileMenu->cursorUp();
					else
						mainMenu->cursorUp();

					break;

				case fgeal::Keyboard::KEY_ARROW_DOWN:
					if(chooseFile)
						fileMenu->cursorDown();
					else
						mainMenu->cursorDown();

					break;

				case fgeal::Keyboard::KEY_ARROW_RIGHT:
					break;

				case fgeal::Keyboard::KEY_ARROW_LEFT:
					break;

				case fgeal::Keyboard::KEY_ENTER:

					if(chooseFile == true)
					{
						if(fileMenu->getSelectedIndex() == fileMenu->getNumberOfEntries()-1)
							chooseFile = false;
						else
						{
							static_cast<LoadingState*>(game.getState(TerrariumGame::LOADING_STATE_ID))->reset(this);
							game.enterState(TerrariumGame::LOADING_STATE_ID);
						}
					}
					else switch(mainMenu->getSelectedIndex()) //this isn't elegant...
					{
						case 0:
							mapGenerationRequested = true;
							static_cast<LoadingState*>(game.getState(TerrariumGame::LOADING_STATE_ID))->reset(this);
							game.enterState(TerrariumGame::LOADING_STATE_ID);

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

	for(int i = 0; i < 4; i++)
		cloudies[i].x += cloudies[i].h * 2.0 * delta;
}

static string createRandomFilename()
{
	static const char alphanum[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";

	string name;
	for (int i = 0; i < 32; ++i)
		name += (alphanum[rand() % (sizeof(alphanum) - 1)]);

	return name;
}

void MainMenuState::loadDuringLoadingScreen()
{
	TerrariumGame& game = static_cast<TerrariumGame&>(this->game);

	if(mapGenerationRequested)
	{
		string randomFilename;
		do randomFilename = fgeal::filesystem::getCurrentWorkingDirectory() + "/resources/maps/" + createRandomFilename() + ".txt";
		while(fgeal::filesystem::isFilenameArchive(randomFilename) or fgeal::filesystem::isFilenameDirectory(randomFilename));

		std::cout << "generating " << randomFilename << "..." << std::endl;
		Grid grid = createGrid(256, 128);
		generator5(grid);
		Map::transpose(grid);
		Map::saveGridToFileTxt(grid, randomFilename);
		game.stageFilename = randomFilename;
	}
	else
	{
		game.stageFilename = fileMenu->getSelectedEntry().label;
	}

	game.enterState(TerrariumGame::INGAME_STATE_ID);
}
