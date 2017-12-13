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
#include "futil/string_actions.hpp"

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
using fgeal::Display;
using fgeal::Keyboard;
using fgeal::Mouse;

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

MainMenuState::MainMenuState(TerrariumGame* game)
: State(*game), wasInit(false),
  background(null), imgCloud(null), imgSun(null),
  mainFont(null), minorFont(null), miniFont(null), devFont(null),
  mainMenu(null), fileMenu(null),
  cloudies(), onMapFileSelectionDialog(), onMapCreationDialog(),
  caret(), mapCreationFilename(), isMapGenerationRequested(), isMapCreationFilenameAlreadyExisting()
{}

MainMenuState::~MainMenuState()
{
	if(not wasInit) return;

	delete background;
	delete imgCloud;
	delete imgSun;

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

	Display& display = Display::getInstance();
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
	Display& display = Display::getInstance();
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

	onMapFileSelectionDialog = false;
	isMapGenerationRequested = false;
	onMapCreationDialog = false;
	mapCreationFilename.clear();
	isMapCreationFilenameAlreadyExisting = false;
	caret = 0;

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
	Display& display = Display::getInstance();
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
	if(onMapFileSelectionDialog)
		fileMenu->draw();

	if(onMapCreationDialog)
	{
		// draw dialog bg
		Image::drawFilledRectangle(0.25*sw, 0.39*sh, 0.5*sw, 4*minorFont->getHeight(), fileMenu->bgColor);

		minorFont->drawText("Map name:", 0.275*sw, 0.4*sh, Color::WHITE);

		// draw name text field bg
		Image::drawFilledRectangle(0.275*sw, 0.4*sh + 1.1*minorFont->getHeight(), 0.4*sw, minorFont->getHeight(), fileMenu->bgColor);

		// draw cursor
		if(((int) (fgeal::uptime()*2))%2 == 0)
		Image::drawFilledRectangle(0.275*sw + minorFont->getTextWidth(mapCreationFilename), 0.4*sh + 1.1*minorFont->getHeight(), 0.005*sw, minorFont->getHeight(), Color::WHITE);

		if(not mapCreationFilename.empty())
			minorFont->drawText(mapCreationFilename, 0.275*sw, 0.4*sh + minorFont->getHeight(), Color::WHITE);

		if(isMapCreationFilenameAlreadyExisting)
			minorFont->drawText("Filename already exists!", 0.275*sw, 0.4*sh + 2.25*minorFont->getHeight(), Color::RED);

		// Ok button
//		Image::drawFilledEllipse(0.5*sw, 0.4*sh + minorFont->getHeight()*4 , 1.25*minorFont->getTextWidth("Ok"), minorFont->getHeight(), Color::ORANGE);
//		minorFont->drawText("Ok", 0.5*(sw-minorFont->getTextWidth("Ok")), 0.4*sh + minorFont->getHeight()*3.5, Color::WHITE);
	}

	devFont->drawText(string("v")+VERSION+", using fgeal v"+fgeal::VERSION+" on "+fgeal::ADAPTED_LIBRARY_NAME+" v"+fgeal::ADAPTED_LIBRARY_VERSION, 4, Display::getInstance().getHeight() - devFont->getHeight(), Color::CREAM);
}

void MainMenuState::update(float delta)
{
	handleInput();

	for(int i = 0; i < 4; i++)
		cloudies[i].x += cloudies[i].h * 2.0 * delta;
}

void MainMenuState::handleInput()
{
	Event ev;
	EventQueue& eventQueue = EventQueue::getInstance();
	while(not eventQueue.isEmpty() )
	{
		eventQueue.waitNextEvent(&ev);

		if(ev.getEventType() == fgeal::Event::TYPE_DISPLAY_CLOSURE)
			game.running = false;

		else
		{
			if(onMapFileSelectionDialog)
				handleInputOnMapFileSelectionDialog(ev);
			else if(onMapCreationDialog)
				handleInputOnMapCreationDialog(ev);
			else
				handleInputOnMainMenu(ev);
		}
	}
}

void MainMenuState::handleInputOnMainMenu(Event& ev)
{
	if(ev.getEventType() == Event::TYPE_KEY_PRESS)
	{
		if(ev.getEventKeyCode() == Keyboard::KEY_ARROW_UP)
			mainMenu->cursorUp();

		if(ev.getEventKeyCode() == Keyboard::KEY_ARROW_DOWN)
			mainMenu->cursorDown();

		if(ev.getEventKeyCode() == Keyboard::KEY_ENTER)
		switch(mainMenu->getSelectedIndex())  // this isn't exactly elegant...
		{
			case 0:
				onMapCreationDialog = true;
				break;

			case 1:
				onMapFileSelectionDialog = true;
				break;

			case 2:
				game.enterState(TerrariumGame::OPTIONS_STATE_ID);
				break;

			case 3:
				game.running = false;
				break;

			default:
				break;
		}
	}
}

void MainMenuState::handleInputOnMapFileSelectionDialog(Event& ev)
{
	if(ev.getEventType() == Event::TYPE_KEY_PRESS)
	{
		if(ev.getEventKeyCode() == Keyboard::KEY_ESCAPE)
			onMapFileSelectionDialog = false;

		if(ev.getEventKeyCode() == Keyboard::KEY_ARROW_UP)
			fileMenu->cursorUp();

		if(ev.getEventKeyCode() == Keyboard::KEY_ARROW_DOWN)
			fileMenu->cursorDown();

		if(ev.getEventKeyCode() == Keyboard::KEY_ENTER)
		{
			if(fileMenu->getSelectedIndex() == fileMenu->getNumberOfEntries()-1)
				onMapFileSelectionDialog = false;
			else
			{
				static_cast<LoadingState*>(game.getState(TerrariumGame::LOADING_STATE_ID))->reset(this);
				game.enterState(TerrariumGame::LOADING_STATE_ID);
			}
		}
	}
}

void MainMenuState::handleInputOnMapCreationDialog(Event& ev)
{
	if(ev.getEventType() == Event::TYPE_KEY_PRESS)
	{
		if(ev.getEventKeyCode() == Keyboard::KEY_ESCAPE)
			onMapCreationDialog = false;

		if(ev.getEventKeyCode() == Keyboard::KEY_ENTER)
		{
			string filename;

			if(futil::trim(mapCreationFilename).empty())
			{
				do filename = fgeal::filesystem::getCurrentWorkingDirectory() + "/resources/maps/" + createRandomFilename() + ".txt";
				while(fgeal::filesystem::isFilenameArchive(filename) or fgeal::filesystem::isFilenameDirectory(filename));
				mapCreationFilename = filename;
			}
			else
			{
				filename = fgeal::filesystem::getCurrentWorkingDirectory() + "/resources/maps/" + mapCreationFilename;
				if(not futil::ends_with(filename, ".txt"))
					filename += ".txt";

				if(fgeal::filesystem::isFilenameArchive(filename) or fgeal::filesystem::isFilenameDirectory(filename))
					isMapCreationFilenameAlreadyExisting = true;

				else
					mapCreationFilename = filename;
			}

			if(not isMapCreationFilenameAlreadyExisting)
			{
				isMapGenerationRequested = true;
				static_cast<LoadingState*>(game.getState(TerrariumGame::LOADING_STATE_ID))->reset(this);
				game.enterState(TerrariumGame::LOADING_STATE_ID);
			}
		}

		char typed = '\n';
		switch(ev.getEventKeyCode())
		{
			case Keyboard::KEY_A: typed = 'a'; break;
			case Keyboard::KEY_B: typed = 'b'; break;
			case Keyboard::KEY_C: typed = 'c'; break;
			case Keyboard::KEY_D: typed = 'd'; break;
			case Keyboard::KEY_E: typed = 'e'; break;
			case Keyboard::KEY_F: typed = 'f'; break;
			case Keyboard::KEY_G: typed = 'g'; break;
			case Keyboard::KEY_H: typed = 'h'; break;
			case Keyboard::KEY_I: typed = 'i'; break;
			case Keyboard::KEY_J: typed = 'j'; break;
			case Keyboard::KEY_K: typed = 'k'; break;
			case Keyboard::KEY_L: typed = 'l'; break;
			case Keyboard::KEY_M: typed = 'm'; break;
			case Keyboard::KEY_N: typed = 'n'; break;
			case Keyboard::KEY_O: typed = 'o'; break;
			case Keyboard::KEY_P: typed = 'p'; break;
			case Keyboard::KEY_Q: typed = 'q'; break;
			case Keyboard::KEY_R: typed = 'r'; break;
			case Keyboard::KEY_S: typed = 's'; break;
			case Keyboard::KEY_T: typed = 't'; break;
			case Keyboard::KEY_U: typed = 'u'; break;
			case Keyboard::KEY_V: typed = 'v'; break;
			case Keyboard::KEY_W: typed = 'w'; break;
			case Keyboard::KEY_X: typed = 'x'; break;
			case Keyboard::KEY_Y: typed = 'y'; break;
			case Keyboard::KEY_Z: typed = 'z'; break;
			case Keyboard::KEY_NUMPAD_0:
			case Keyboard::KEY_0:       typed = '0'; break;
			case Keyboard::KEY_NUMPAD_1:
			case Keyboard::KEY_1:       typed = '1'; break;
			case Keyboard::KEY_NUMPAD_2:
			case Keyboard::KEY_2:       typed = '2'; break;
			case Keyboard::KEY_NUMPAD_3:
			case Keyboard::KEY_3:       typed = '3'; break;
			case Keyboard::KEY_NUMPAD_4:
			case Keyboard::KEY_4:       typed = '4'; break;
			case Keyboard::KEY_NUMPAD_5:
			case Keyboard::KEY_5:       typed = '5'; break;
			case Keyboard::KEY_NUMPAD_6:
			case Keyboard::KEY_6:       typed = '6'; break;
			case Keyboard::KEY_NUMPAD_7:
			case Keyboard::KEY_7:       typed = '7'; break;
			case Keyboard::KEY_NUMPAD_8:
			case Keyboard::KEY_8:       typed = '8'; break;
			case Keyboard::KEY_NUMPAD_9:
			case Keyboard::KEY_9:       typed = '9'; break;
			case Keyboard::KEY_SPACE:   typed = ' '; break;
			case Keyboard::KEY_PERIOD:  typed = '.'; break;
			case Keyboard::KEY_MINUS:   typed = '-'; break;
			case Keyboard::KEY_BACKSPACE: typed = '<'; break;

			// caret
			case Keyboard::KEY_ARROW_LEFT:
				if(caret > 0) caret--;
				break;

			case Keyboard::KEY_ARROW_RIGHT:
				if(caret < (int) mapCreationFilename.size()) caret++;
				break;
			default:break;
		}

		if(typed == '<' and not mapCreationFilename.empty() and caret > 0)
		{
			isMapCreationFilenameAlreadyExisting = false;
			mapCreationFilename.erase(mapCreationFilename.begin() + caret-1);
			caret--;
		}

		else if(typed != '\n' and typed != '<')
		{
			if(typed >= 'a' and typed <= 'z')
			if(Keyboard::isKeyPressed(Keyboard::KEY_LEFT_SHIFT)
			or Keyboard::isKeyPressed(Keyboard::KEY_RIGHT_SHIFT))
				typed -= 32;

			if(typed == '-')
			if(Keyboard::isKeyPressed(Keyboard::KEY_LEFT_SHIFT)
			or Keyboard::isKeyPressed(Keyboard::KEY_RIGHT_SHIFT))
				typed = '_';

			isMapCreationFilenameAlreadyExisting = false;
			mapCreationFilename.insert(mapCreationFilename.begin() + caret, 1, typed);
			caret++;
		}
	}
}

void MainMenuState::loadDuringLoadingScreen()
{
	TerrariumGame& game = static_cast<TerrariumGame&>(this->game);

	if(isMapGenerationRequested)
	{
		std::cout << "generating " << mapCreationFilename << "..." << std::endl;
		Grid grid = createGrid(256, 128);
		generator5(grid);
		Map::transpose(grid);
		Map::saveGridToFileTxt(grid, mapCreationFilename);
		game.logic.setIngameStateStageFilename(mapCreationFilename);
	}
	else
	{
		game.logic.setIngameStateStageFilename(fileMenu->getSelectedEntry().label);
	}

	game.enterState(TerrariumGame::INGAME_STATE_ID);
}
