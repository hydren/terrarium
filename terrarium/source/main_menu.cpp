/*
 * main_menu.cpp
 *
 *  Created on: 01/10/2013
 *      Author: carlosfaruolo
 */

#include "../header/main_menu.hpp"

using Engine::Color;

MainMenu::MainMenu()
: background(new Image("resource/title_proto.jpg")),
  mainFont(new Engine::Font("resource/jack.ttf", 44)),
  minorFont(new Engine::Font("resource/liberation.ttf", 24)),
  eventQueue(new Engine::EventQueue())
{}

MainMenu::MENU_OPTION MainMenu::show()
{
	//TODO Work in progress

	Menu mainMenu(Rect(64, 108, 300, 100), minorFont, Color::BLUE);
	mainMenu.addEntry("Generate new map");
	mainMenu.addEntry("Load map from file");
	mainMenu.addEntry("Settings");
	mainMenu.addEntry("Exit");

	Image fileDialogBackground(Image::FILLED_RECTANGLE, Color::RED, 300, 200);

	Image* temp1 = new Image(Image::FILLED_RECTANGLE, Color::BLACK, 296, 174);
	temp1->blit(fileDialogBackground, 2, 24);
	delete temp1;

	Engine::Font* miniFont = new Engine::Font("resource/jack.ttf", 10);

	Menu fileMenu(Rect(204, 224, 294, 174), miniFont, Color::RED);
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
							else return MainMenu::LOAD_MAP_FROM_FILE;

						}
						else switch(mainMenu.selectedIndex)
						{
							case 1:
//								return MainMenu::LOAD_MAP_FROM_FILE;
								chooseFile = true;

								break;
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
		mainFont->draw_text("Project Terrarium", 84, 25, Color::ORANGE);
		mainMenu.draw();
		if(chooseFile)
		{
			fileDialogBackground.draw(200, 200);
			minorFont->draw_text("Which file?", 202, 200, Color::BLACK);
			fileMenu.draw();
		}
		Engine::display->refresh();

	}

	return MainMenu::EXIT;
}
