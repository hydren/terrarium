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

struct MainMenu
{
	MainMenu();

	~MainMenu();

	/** Start the main menu */
	void show();
};


#endif /* MAIN_MENU_HPP_ */
