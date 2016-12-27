/*
 * main.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#include <cstdlib>
#include <stdexcept>

#include "fgeal/fgeal.hpp"
using fgeal::Image;

#include "terrarium_game.hpp"

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

std::string VERSION = "0.1.5-dev";

int main(int argc, char** argv)
{
	try
	{
		fgeal::initialize();

		cout << "Lets test the directory listing method" << endl;
		vector<string> dirs = fgeal::getFilenamesWithinDirectory("./resources/maps");
		for(unsigned i = 0; i < dirs.size(); i++)
			cout << dirs[i] << endl;

		//only need to instantiate, engine will delete it when finalizing
		fgeal::Display::create(640, 480, "Project Terrarium (v"+VERSION+") (fgeal v"+ fgeal::VERSION + "/" + fgeal::BACKEND_NAME +" backend)");

		Image* loading_image = new Image("./resources/loading.png");
		loading_image->draw();
		fgeal::Display::getInstance().refresh();
		delete loading_image;

		TerrariumGame* game = new TerrariumGame();
		game->start();
		delete game;
		fgeal::finalize();
	}
	catch(std::runtime_error& e)
	{
		cout << e.what() << endl;
	}
	return EXIT_SUCCESS;
}
