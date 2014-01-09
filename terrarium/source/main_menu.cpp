/*
 * main_menu.cpp
 *
 *  Created on: 01/10/2013
 *      Author: carlosfaruolo
 */

#include "../header/main_menu.hpp"
#include "../header/game.hpp"
#include "../header/menu.hpp"

using Engine::Color;

//functions declarations
void loadGameWithMap(const String& map_path);

MainMenu::MainMenu()
: background(new Image("resource/title_proto.jpg")),
  mainFont(new Engine::Font("resource/jack.ttf", 44)),
  minorFont(new Engine::Font("resource/liberation.ttf", 24)),
  eventQueue(new Engine::EventQueue())
{}

void MainMenu::show()
{
	//TODO Work in progress

	Image loading_image("./resource/loading.png");
	loading_image.draw();
	Engine::display->refresh();

	Menu mainMenu(Rect(64, 108, 300, 150), minorFont, Color::ORANGE, true);
	mainMenu.addEntry("Generate new map");
	mainMenu.addEntry("Load map from file");
	mainMenu.addEntry("Settings");
	mainMenu.addEntry("Exit");

	Engine::Font* miniFont = new Engine::Font("resource/jack.ttf", 16);

	Menu fileMenu(Rect(32, 224, 294, 174), miniFont, Color::YELLOW, true, "Which file?");
	list<String> dirs = Engine::getFilenamesWithinDirectory("./resource/maps");
		for(list<String>::iterator it = dirs.begin(); it != dirs.end() ; ++it)
			fileMenu.addEntry(*it);
	fileMenu.addEntry("< Cancel >");

	bool running = true;
	bool chooseFile = false;

	while(running)
	{
		//EVENTS
		while(! eventQueue->isEmpty() )
		{
			Engine::Event* ev = eventQueue->waitForEvent();

			if(ev->getEventType() == Engine::Event::Type::DISPLAY_CLOSURE)
			{
				running=false;
			}
			else if(ev->getEventType() == Engine::Event::Type::KEY_RELEASE)
			{
				switch(ev->getEventKeyCode())
				{
					case Engine::Event::Key::ARROW_UP:
						if(chooseFile)
							--fileMenu;
						else
							--mainMenu;

						break;

					case Engine::Event::Key::ARROW_DOWN:
						if(chooseFile)
							++fileMenu;
						else
							++mainMenu;

						break;

					case Engine::Event::Key::ARROW_RIGHT:
						break;

					case Engine::Event::Key::ARROW_LEFT:
						break;

					case Engine::Event::Key::ENTER:

						if(chooseFile == true)
						{
							if(fileMenu.selectedIndex == fileMenu.getNumberOfEntries()-1)
								chooseFile = false;
							else
							{
								loading_image.draw();
								Engine::display->refresh();

								loadGameWithMap(fileMenu.getSelectedEntry()->label);
								chooseFile = false;
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
		Engine::display->refresh();

	}

}

void loadGameWithMap(const String& map_path)
{
	Game game(map_path);
	game.start();
}
