/*
 * main.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#include <cstdlib>
#include "../header/engine.hpp"
#include "../header/main_menu.hpp"
#include "../header/game.hpp"

using std::cout;
using std::endl;

int main(int argc, char** argv)
{
	try
	{
		Engine::initialize();
		Engine::display = new Engine::Display(640, 480, "Project Terrarium");

		cout << "Lets test the directory listing method" << endl;
		list<String> dirs = Engine::getFilenamesWithinDirectory(".");
		for(list<String>::iterator it = dirs.begin(); it != dirs.end() ; ++it)
			cout << *it << endl;


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
		Engine::finalize();
	}
	catch(Exception& e)
	{
		cout << e.message() << endl;
	}
	return EXIT_SUCCESS;
}
