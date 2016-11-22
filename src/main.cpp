/*
 * main.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#include <cstdlib>
#include <stdexcept>

#include "fgeal/fgeal.hpp"
#include "file_parser.hpp"
#include "main_menu.hpp"

#include <iostream>
using std::cout;
using std::endl;

string VERSION = "0.1.2";

int main(int argc, char** argv)
{
	try
	{
		fgeal::initialize();
		{
			fgeal::Display display(640, 480, "Project Terrarium (v"+VERSION+") (fgeal v"+ fgeal::VERSION + "/" + fgeal::BACKEND_NAME +" backend)");

	//		cout << "Lets test the directory listing method" << endl;
	//		list<String> dirs = fgeal::getFilenamesWithinDirectory("./resources/maps");
	//		for(list<String>::iterator it = dirs.begin(); it != dirs.end() ; ++it)
	//			cout << *it << endl;

			Image loading_image("./resources/loading.png");
			loading_image.draw();
			fgeal::display->refresh();

			MainMenu menu;
			menu.show();
		}
		fgeal::finalize();
	}
	catch(std::runtime_error& e)
	{
		cout << e.what() << endl;
	}
	return EXIT_SUCCESS;
}
