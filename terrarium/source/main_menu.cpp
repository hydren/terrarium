/*
 * main_menu.cpp
 *
 *  Created on: 01/10/2013
 *      Author: carlosfaruolo
 */

#include "../header/main_menu.hpp"

//static initialization
const int MainMenu::NUMBER_OF_OPTIONS = 2;

MainMenu::MainMenu()
: bg(new Image("resource/title_proto.jpg")),
  font(new Engine::Font("resource/jack.ttf", 44)),
  minorFont(new Engine::Font("resource/liberation.ttf", 24)),
  eventQueue(new Engine::EventQueue()),
  selected(0)
{}

MainMenu::MENU_OPTION MainMenu::show()
{
	//TODO

//	Menu m(Rect(100, 100, 300, 100), font, Engine::Color::CYAN);
//	m.addEntry("Test 1");
//	m.addEntry("Test 2");

	bool running = true;
	selected = 0;

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
						if(selected > 0)
							selected--;

					break;
					case Engine::Event::Key::ARROW_DOWN:
						if(selected < NUMBER_OF_OPTIONS-1)
							selected++;

					break;
					case Engine::Event::Key::ARROW_RIGHT:

					break;
					case Engine::Event::Key::ARROW_LEFT:

					break;

					case Engine::Event::Key::ENTER:
						switch(selected)
						{
							case 0:
								return MainMenu::LOAD_MAP_FROM_FILE;
							case 1:
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

		bg->draw();
		font->draw_text("Project Terrarium", 84, 25, Engine::Color::ORANGE);
		minorFont->draw_text("Load map from file", 96, 192, Engine::Color::BLUE);
		minorFont->draw_text("Exit", 96, 224, Engine::Color::BLUE);
		minorFont->draw_text(">>", 64, 192 + selected*32, Engine::Color::RED);
//		m.draw();
		Engine::display->refresh();

	}

	return MainMenu::EXIT;
}
