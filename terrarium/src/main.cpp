/*
 * main.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#include <cstdlib>
#include <stdexcept>

#include "fgeal.hpp"
#include "file_parser.hpp"
#include "main_menu.hpp"

using std::cout;
using std::endl;

string VERSION = "0.1.1-alpha-fgeal0.1";

int main(int argc, char** argv)
{
	try
	{
		GameEngine::initialize();
		GameEngine::display = new GameEngine::Display(640, 480, "Project Terrarium (v"+VERSION+")");

//		cout << "Lets test the directory listing method" << endl;
//		list<String> dirs = GameEngine::getFilenamesWithinDirectory("./resources/maps");
//		for(list<String>::iterator it = dirs.begin(); it != dirs.end() ; ++it)
//			cout << *it << endl;

		Image loading_image("./resources/loading.png");
		loading_image.draw();
		GameEngine::display->refresh();

		MainMenu menu;
		menu.show();

		delete GameEngine::display;
		GameEngine::finalize();
	}
	catch(std::runtime_error& e)
	{
		cout << e.what() << endl;
	}
	return EXIT_SUCCESS;
}
