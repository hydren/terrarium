/*
 * main_menu.cpp
 *
 *  Created on: 01/10/2013
 *      Author: carlosfaruolo
 */

#include "../header/main_menu.hpp"

MainMenu::MainMenu()
: background(new Image("resource/title_proto.jpg")),
  mainFont(new Engine::Font("resource/jack.ttf", 44)),
  minorFont(new Engine::Font("resource/liberation.ttf", 24)),
  eventQueue(new Engine::EventQueue())
{}

MainMenu::MENU_OPTION MainMenu::show()
{
	//TODO Work in progress

	Menu menu(Rect(64, 108, 300, 100), minorFont, Engine::Color::BLUE);
	menu.addEntry("Generate new map");
	menu.addEntry("Load map from file");
	menu.addEntry("Settings");
	menu.addEntry("Exit");

	bool running = true;

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
						--menu;
						break;

					case Engine::Event::Key::ARROW_DOWN:
						++menu;
						break;

					case Engine::Event::Key::ARROW_RIGHT:
						break;

					case Engine::Event::Key::ARROW_LEFT:
						break;

					case Engine::Event::Key::ENTER:

						switch(menu.selectedIndex)
						{
							case 1:
								return MainMenu::LOAD_MAP_FROM_FILE;
							case 3:
								return MainMenu::EXIT;
							default: break;
						}
						break;

				default:
					break;
				}
			}
		}

		//RENDER

		background->draw();
		mainFont->draw_text("Project Terrarium", 84, 25, Engine::Color::ORANGE);
		menu.draw();
		Engine::display->refresh();

	}

	return MainMenu::EXIT;
}
