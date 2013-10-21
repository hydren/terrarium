/*
 * game.hpp
 *
 *  Created on: 11/05/2013
 *      Author: felipe
 */

#ifndef GAME_HPP_
#define GAME_HPP_

#include "body.hpp"
#include "image.hpp"
#include "entity.hpp"
#include "engine.hpp"
#include "block.hpp"
#include "map.hpp"


struct Game
{
	vector<Entity*> entities;
	vector<Image*> images;
	vector<Block*> blocks;

	Entity* player;

	Rect visibleArea;

	Map* map;

	Image* green_box;

	Engine::Event::Queue* eventQueue;
	Engine::Font* font;

	b2World* world;

	bool running, jumping;
	bool isKeyUpPressed, isKeyDownPressed, isKeyRightPressed, isKeyLeftPressed;

	void handleInput();
	void drawScene();
	void drawDebug();
	void physics();
	void start();

	Game();


};


#endif /* GAME_HPP_ */
