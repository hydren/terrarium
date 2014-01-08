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

	Image* background;
	Engine::Font* mainFont;
	Engine::Font* minorFont;
	Engine::EventQueue* eventQueue;

	public:

	MainMenu();

	/** Start the main menu */
	void show();
};


#endif /* MAIN_MENU_HPP_ */
