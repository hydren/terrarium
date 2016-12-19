/*
 * menu_state.cpp
 *
 *  Created on: 11 de dez de 2016
 *      Author: felipe
 */

#include "menu_state.hpp"

#include <vector>

using fgeal::Color;
using fgeal::Image;
using fgeal::Font;
using fgeal::EventQueue;

using std::vector;

struct MenuState::implementation
{
	TerrariumGame& game; // helper reference
	bool wasInit;

	implementation(TerrariumGame* terrariumGame)  // @suppress("Class members should be properly initialized")
	: game(*terrariumGame), wasInit(false),
	  fileMenu(null)
	{}

	// --------------------------------------------------

	fgeal::EventQueue* eventQueue;
	fgeal::Image* background;
	fgeal::Font* mainFont, *minorFont, *miniFont;

	Menu* mainMenu, *fileMenu;

	bool chooseFile;

	void init()
	{
		mainFont = new Font("resources/jack.ttf", 44);
		minorFont = new Font("resources/liberation.ttf", 24);
		miniFont = new Font("resources/jack.ttf", 16);
		background = new Image("resources/title_proto.jpg");
		eventQueue = new EventQueue();

		Rectangle size = {64, 108, 300, 150};
		mainMenu = new Menu(size, minorFont, Color::ORANGE);
		mainMenu->addEntry("Generate new map");
		mainMenu->addEntry("Load map from file");
		mainMenu->addEntry("Settings");
		mainMenu->addEntry("Exit");
	}

	~implementation()
	{
		if(not wasInit) return;

		delete mainFont;
		delete minorFont;
		delete miniFont;
		delete background;
		delete eventQueue;

		delete mainMenu;
		delete fileMenu;
	}

	void setup()
	{
		if(fileMenu != null) delete fileMenu;
		Rectangle size = {32, 224, 294, 174};
		fileMenu = new Menu(size, miniFont, Color::YELLOW, "Which file?");
		vector<string> dirs = fgeal::getFilenamesWithinDirectory("./resources/maps");
			for(vector<string>::iterator it = dirs.begin(); it != dirs.end() ; ++it)
				fileMenu->addEntry(*it);
		fileMenu->addEntry("< Cancel >");

		chooseFile = false;
		eventQueue->listenEvents();
	}

	void render()
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

	void update()
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
								game.stageFilename = fileMenu->getSelectedEntry()->label;
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
};

MenuState::MenuState(TerrariumGame* game)
:	State(*game), self(*new implementation(game))
{}

MenuState::~MenuState() { delete &self; }

void MenuState::initialize() { self.init(); }

void MenuState::onEnter() { self.setup(); }

void MenuState::onLeave()
{
	self.eventQueue->ignoreEvents();
}

void MenuState::render() { self.render(); }

void MenuState::update(float delta) { self.update(); }

