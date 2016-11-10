/*
 * main_menu.cpp
 *
 *  Created on: 01/10/2013
 *      Author: carlosfaruolo
 */

#include "main_menu.hpp"

#include "game.hpp"
#include "menu.hpp"

#include <string>
#include <vector>

using fgeal::Color;
using fgeal::Image;
using fgeal::Font;
using fgeal::EventQueue;
using std::string;
using std::vector;

//static here means not-global to other files
static EventQueue* eventQueue;
static Font* mainFont, *minorFont;
static Image* background;

//functions declarations
static void loadGameWithMap(const string& map_path);

MainMenu::MainMenu()
{
	mainFont = new Font("resources/jack.ttf", 44);
	minorFont = new Font("resources/liberation.ttf", 24);
	background = new Image("resources/title_proto.jpg");
	eventQueue = new EventQueue();
}

MainMenu::~MainMenu()
{
	delete mainFont;
	delete minorFont;
	delete background;
	delete eventQueue;
}

void MainMenu::show()
{
	//TODO Work in progress

	Image loading_image("./resources/loading.png");
	loading_image.draw();
	fgeal::display->refresh();

	Menu mainMenu(createRectangle(64, 108, 300, 150), minorFont, Color::ORANGE);
	mainMenu.addEntry("Generate new map");
	mainMenu.addEntry("Load map from file");
	mainMenu.addEntry("Settings");
	mainMenu.addEntry("Exit");

	fgeal::Font* miniFont = new fgeal::Font("resources/jack.ttf", 16);

	Menu fileMenu(createRectangle(32, 224, 294, 174), miniFont, Color::YELLOW, "Which file?");
	vector<string> dirs = fgeal::getFilenamesWithinDirectory("./resources/maps");
		for(vector<string>::iterator it = dirs.begin(); it != dirs.end() ; ++it)
			fileMenu.addEntry(*it);
	fileMenu.addEntry("< Cancel >");

	bool running = true;
	bool chooseFile = false;

	while(running)
	{
		//EVENTS
		while(! eventQueue->isEmpty() )
		{
			fgeal::Event* ev = eventQueue->waitForEvent();

			if(ev->getEventType() == fgeal::Event::Type::DISPLAY_CLOSURE)
			{
				running=false;
			}
			else if(ev->getEventType() == fgeal::Event::Type::KEY_PRESS)
			{
				switch(ev->getEventKeyCode())
				{
					case fgeal::Event::Key::ARROW_UP:
						if(chooseFile)
							--fileMenu;
						else
							--mainMenu;

						break;

					case fgeal::Event::Key::ARROW_DOWN:
						if(chooseFile)
							++fileMenu;
						else
							++mainMenu;

						break;

					case fgeal::Event::Key::ARROW_RIGHT:
						break;

					case fgeal::Event::Key::ARROW_LEFT:
						break;

					case fgeal::Event::Key::ENTER:

						if(chooseFile == true)
						{
							if(fileMenu.selectedIndex == fileMenu.getNumberOfEntries()-1)
								chooseFile = false;
							else
							{
								loading_image.draw();
								fgeal::display->refresh();

								eventQueue->ignoreEvents();
								loadGameWithMap(fileMenu.getSelectedEntry()->label);
								chooseFile = false;
								eventQueue->listenEvents();
							}

						}
						else switch(mainMenu.selectedIndex) //this isn't elegant...
						{
							case 1:
								chooseFile = true;
								break;

							case 3:
								running = false;
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

		//RENDER

		background->draw();
		mainFont->drawText("Project Terrarium", 84, 25, Color::ORANGE);
		mainMenu.draw();
		if(chooseFile)
		{
			minorFont->drawText("Which file?", 202, 200, Color::BLACK);
			fileMenu.draw();
		}
		fgeal::display->refresh();

	}

}

void loadGameWithMap(const string& map_path)
{
	Game game(map_path);
	game.start();
}
