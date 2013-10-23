/*
 * main_menu.hpp
 *
 *  Created on: 01/10/2013
 *      Author: carlosfaruolo
 */

#ifndef MAIN_MENU_HPP_
#define MAIN_MENU_HPP_

#include "util.hpp"
#include "engine.hpp"

using Engine::Image;

class MainMenu
{
	private:
	//nothing yet

	static const int NUMBER_OF_OPTIONS;

	Image* bg;
	Engine::Font* font;
	Engine::Font* minorFont;
	Engine::EventQueue* eventQueue;
	int selected;

	public:


	MainMenu();

	enum MENU_OPTION
	{
		NEW_GAME,
		LOAD_MAP_FROM_FILE,
		OPTIONS,
		EXIT,
		NOTHING
	};

	/** Returns the selected option */
	MENU_OPTION show();
};


#endif /* MAIN_MENU_HPP_ */
