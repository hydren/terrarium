/*
 * main_menu.cpp
 *
 *  Created on: 01/10/2013
 *      Author: carlosfaruolo
 */

#include "main_menu.hpp"

#include "game.hpp"
#include "menu.hpp"

using GameEngine::Color;
using GameEngine::Image;
using GameEngine::Font;
using GameEngine::EventQueue;

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
	GameEngine::display->refresh();

	Menu mainMenu(Rect(64, 108, 300, 150), minorFont, Color::ORANGE);
	mainMenu.addEntry("Generate new map");
	mainMenu.addEntry("Load map from file");
	mainMenu.addEntry("Settings");
	mainMenu.addEntry("Exit");

	GameEngine::Font* miniFont = new GameEngine::Font("resources/jack.ttf", 16);

	Menu fileMenu(Rect(32, 224, 294, 174), miniFont, Color::YELLOW, "Which file?");
	list<string> dirs = GameEngine::getFilenamesWithinDirectory("./resources/maps");
		for(list<string>::iterator it = dirs.begin(); it != dirs.end() ; ++it)
			fileMenu.addEntry(*it);
	fileMenu.addEntry("< Cancel >");

	bool running = true;
	bool chooseFile = false;

	while(running)
	{
		//EVENTS
		while(! eventQueue->isEmpty() )
		{
			GameEngine::Event* ev = eventQueue->waitForEvent();

			if(ev->getEventType() == GameEngine::Event::Type::DISPLAY_CLOSURE)
			{
				running=false;
			}
			else if(ev->getEventType() == GameEngine::Event::Type::KEY_PRESS)
			{
				switch(ev->getEventKeyCode())
				{
					case GameEngine::Event::Key::ARROW_UP:
						if(chooseFile)
							--fileMenu;
						else
							--mainMenu;

						break;

					case GameEngine::Event::Key::ARROW_DOWN:
						if(chooseFile)
							++fileMenu;
						else
							++mainMenu;

						break;

					case GameEngine::Event::Key::ARROW_RIGHT:
						break;

					case GameEngine::Event::Key::ARROW_LEFT:
						break;

					case GameEngine::Event::Key::ENTER:

						if(chooseFile == true)
						{
							if(fileMenu.selectedIndex == fileMenu.getNumberOfEntries()-1)
								chooseFile = false;
							else
							{
								loading_image.draw();
								GameEngine::display->refresh();

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
		mainFont->draw_text("Project Terrarium", 84, 25, Color::ORANGE);
		mainMenu.draw();
		if(chooseFile)
		{
			minorFont->draw_text("Which file?", 202, 200, Color::BLACK);
			fileMenu.draw();
		}
		GameEngine::display->refresh();

	}

}

void loadGameWithMap(const string& map_path)
{
	Game game(map_path);
	game.start();
}
