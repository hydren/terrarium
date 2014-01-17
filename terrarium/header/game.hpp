/*
 * game.hpp
 *
 *  Created on: 11/05/2013
 *      Author: felipe
 */

#ifndef GAME_HPP_
#define GAME_HPP_

#include "entity.hpp"
#include "engine.hpp"
#include "menu.hpp"
#include "block.hpp"
#include "map.hpp"

struct Game
{
	Game(const string& map_path);
	void start();

	private:
	vector<Entity*> entities;
	vector<Image*> images;
	vector<Block*> blocks;


	Entity* player;

	Rect visibleArea;

	Map* map;

	Image* green_box;

	Engine::Font* font;
	Menu* inGameMenu;

	Engine::EventQueue* eventQueue;

	Physics::World* world;

	bool running, jumping, inGameMenuShowing;
	bool isKeyUpPressed, isKeyDownPressed, isKeyRightPressed, isKeyLeftPressed;

	void handleInput();
	void drawScene();
	void drawDebug();
	void drawInGameMenu();
	void physics();

	Game();


};


#endif /* GAME_HPP_ */
