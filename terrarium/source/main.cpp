/*
 * main.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#include <cstdlib>
#include "../header/engine.hpp"
#include "../header/game.hpp"
#include "../header/main_menu.hpp"

using std::cout;
using std::endl;

int main(int argc, char** argv)
{
	try
	{
		Engine::initialize();
		Engine::display = new Engine::Display(640, 480, "Nice screen");


		MainMenu menu;
		while(true) switch(menu.show())
		{
			case MainMenu::LOAD_MAP_FROM_FILE:
			{
				Game game;
				game.start();
			}
			break;

			case MainMenu::EXIT:
			default:
			{
				goto out;
			}
		}
		out:

		delete Engine::display;
	}
	catch(Exception& e)
	{
		std::cout << e.message() << std::endl;
	}
	return EXIT_SUCCESS;
}
