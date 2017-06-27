/*
 * main.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#include "terrarium_game.hpp"

#include "fgeal/fgeal.hpp"
using fgeal::Image;

#include "futil/string/actions.hpp"
#include "futil/string/split.hpp"

#include <cstdlib>
#include <stdexcept>

#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

const string VERSION = "0.1.6";

int main(int argc, char** argv)
{
	string title = "Project Terrarium";
	title += " (v"; title += VERSION; title += ")";

	int screenWidth = 640, screenHeight = 480;
	for(int i = 0; i < argc; i++)
	{
		if(starts_with(string(argv[i]), "-r"))
		{
			if(i+1 < argc)
			{
				string txtResolution(argv[i+1]);
				vector<string> tokens = split(txtResolution, 'x');
				if(tokens.size() == 2)
				{
					int customWidth =  atoi(tokens[0].c_str());
					int customHeight = atoi(tokens[1].c_str());
					if(customWidth != 0 and customHeight != 0)
					{
						screenWidth = customWidth;
						screenHeight = customHeight;
					}

				}
				else cout << "Failed to parse custom resolution" << endl;
			}
			else cout << "Missing argument to -r parameter" << endl;
		}
	}

	try
	{
		TerrariumGame::CONFIG.load("config.properties");
		fgeal::initialize();

		// only need to instantiate, engine will delete it when finalizing
		fgeal::Display::create(screenWidth, screenHeight, title);

		// loading splash
		Image* loading_image = new Image("./resources/loading.png");
		loading_image->draw();
		fgeal::Display::getInstance().refresh();
		delete loading_image;

		// start game
		TerrariumGame* game = new TerrariumGame();
		game->start();
		delete game;
		fgeal::finalize();
	}
	catch(const fgeal::AdapterException& e)
	{
		cout << "engine error " << e.what() << endl;
	}
	catch(std::runtime_error& e)
	{
		cout << "runtime error " << e.what() << endl;
	}
	return EXIT_SUCCESS;
}
