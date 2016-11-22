/*
 * game.cpp
 *
 *  Created on: 11/05/2013
 *      Author: felipe
 */

#include "game.hpp"

#include <iostream>
using std::cout; using std::endl;

#include "futil/string/more_operators.hpp"

#include "util.hpp"
#include "block.hpp"
#include "entity.hpp"
#include "map.hpp"
#include "menu.hpp"

using Physics::World;
using Physics::Vector;

using fgeal::EventQueue;
using fgeal::Font;
using fgeal::Color;

struct GameStuff
{
	vector<Entity*> entities;
	vector<Image*> images;

	Rectangle visibleArea;

	Entity* player;
	Map* game_map;
	Image* tileset_dirt;
	Font* font;
	Menu* inGameMenu;
	EventQueue* eventQueue;
	World* world;

	bool running, jumping, inGameMenuShowing;
	bool isKeyUpPressed, isKeyDownPressed, isKeyRightPressed, isKeyLeftPressed;

	float playerJumpImpulse, playerWalkForce;

	GameStuff(const string& map_path)
	{
		visibleArea = createRectangle(0, 0, fgeal::display->getWidth(), fgeal::display->getHeight());

		//loading font
		font = new fgeal::Font("resources/liberation.ttf", 14);

		//loading ingame menu
		inGameMenu = new Menu(createRectangle(200, 200, 200, 64), font, Color::ORANGE);
		inGameMenu->addEntry("Resume");
		inGameMenu->addEntry("Save and exit");
		inGameMenu->addEntry("Exit without saving");

		//loading event queue
		eventQueue = new fgeal::EventQueue();

		//setting flags
		running = true;
		jumping = false;
		inGameMenuShowing = false;
		isKeyUpPressed = false;
		isKeyDownPressed = false;
		isKeyRightPressed = false;
		isKeyLeftPressed = false;

		//loading world
		Vector gravity(0.0f, 10.0f);
		world = new World(gravity);

		//loading map
		game_map = Map::loadMapFromFile(map_path, world, images);
		game_map->visibleArea = &visibleArea;


		//loading player graphics

		// Pijamaman specs
		unsigned player_sprite_width = 56, player_sprite_height = 84;
		float player_body_width = Physics::convertToMeters(25), player_body_height = Physics::convertToMeters(81);

		Image* player_img = new Image("resources/pijamaman-1.png");
		AnimationSet* anim = new AnimationSet(player_img);

		anim->add("still-left", player_sprite_width, player_sprite_height, 1, 1);
		anim->ref("still-left").referencePixelY = -2;

		anim->add("still-right", player_sprite_width, player_sprite_height, 1, 1);
		anim->ref("still-right").referencePixelY = -2;

		anim->add("walk-left", player_sprite_width, player_sprite_height, 0.25, 4);
		anim->ref("walk-left").referencePixelY = -2;

		anim->add("walk-right", player_sprite_width, player_sprite_height, 0.25, 4);
		anim->ref("walk-right").referencePixelY = -2;

		anim->setCurrent("still-right");

		//loading player physics
		Body* b = new Body(1, 1, player_body_width, player_body_height);
		player = new Entity(anim, b, &visibleArea);
		world->addBody(player->body);
		player->body->setFixedRotation();

		playerJumpImpulse = player_body_width*player_body_height * 0.5;
		playerWalkForce =   player_body_width*player_body_height * 1.2;

		//loading dirt tileset
		tileset_dirt = new Image("resources/tileset-dirt.png");
		cout << "map size (in pixels): " << game_map->computeDimensions().w << "x" << game_map->computeDimensions().h << endl;
	}

	~GameStuff()
	{
		cout << "game stuff destructor..." << endl;
		foreach(Entity*, e, vector<Entity*>, entities)
		{
			delete e;
		}

		foreach(Image*, img, vector<Image*>, images)
		{
			delete img;
		}

		delete player;
		delete game_map;
		delete tileset_dirt;
		delete font;
		delete inGameMenu;
		delete eventQueue;
		delete world;
	}

	void start()
	{
		while(running)
		{
			//visible area quick n' dirt fix
			{
				visibleArea.x = Physics::convertToPixels(player->body->getX()) - visibleArea.w/2.0;
				visibleArea.y = Physics::convertToPixels(player->body->getY()) - visibleArea.h/2.0;
				if(visibleArea.x + visibleArea.w > game_map->computeDimensions().w)
					visibleArea.x = game_map->computeDimensions().w - visibleArea.w;
				if(visibleArea.x < 0)
					visibleArea.x = 0;
				if(visibleArea.y + visibleArea.h > game_map->computeDimensions().h)
					visibleArea.y = game_map->computeDimensions().h - visibleArea.h;
				if(visibleArea.y < 0)
					visibleArea.y = 0;
			}

			handleInput();
			drawScene();
		}
	}

	void handleInput()
	{

		if(not inGameMenuShowing and isKeyUpPressed and !jumping) {
			Vector v = player->body->getVelocity();
			if(v.y >= -2.0f) {
				player->body->applyImpulse(Vector(0.0f, -playerJumpImpulse), Vector(player->body->getX(), player->body->getY()));
				jumping = true;
			}
		}
		if(not inGameMenuShowing and isKeyDownPressed) {
			Vector v = player->body->getVelocity();
			if(v.y <= 2.0f)
				player->body->applyForceToCenter(Vector(0.0f, 0.2f));
		}
		if(not inGameMenuShowing and isKeyRightPressed) {
			player->animation->setCurrent("walk-right");
			Vector v = player->body->getVelocity();
			if(v.x <= 2.0f)
				player->body->applyForceToCenter(Vector(playerWalkForce, 0.0f));
		}
		if(not inGameMenuShowing and isKeyLeftPressed) {
			player->animation->setCurrent("walk-left");
			Vector v = player->body->getVelocity();
			if(v.x >= -2.0f)
				player->body->applyForceToCenter(Vector(-playerWalkForce, 0.0f));
		}

		while(! eventQueue->isEmpty() )
		{
			fgeal::Event* ev = eventQueue->waitForEvent();

			if(ev->getEventType() == fgeal::Event::Type::DISPLAY_CLOSURE)
			{
				running=false;
			}
			else if(ev->getEventType() == fgeal::Event::Type::KEY_RELEASE)
			{
				switch(ev->getEventKeyCode())
				{
				case fgeal::Event::Key::ARROW_UP:
					isKeyUpPressed = false;
					jumping = false;
					break;
				case fgeal::Event::Key::ARROW_DOWN:
					isKeyDownPressed = false;
					break;
				case fgeal::Event::Key::ARROW_RIGHT:
					player->animation->setCurrent("still-right");
					isKeyRightPressed = false;
					break;
				case fgeal::Event::Key::ARROW_LEFT:
					player->animation->setCurrent("still-left");
					isKeyLeftPressed = false;
					break;
				default:
					break;
				}
			}
			else if(ev->getEventType() == fgeal::Event::Type::KEY_PRESS)
			{
				switch(ev->getEventKeyCode())
				{
				case fgeal::Event::Key::ARROW_UP:
					isKeyUpPressed = true;
					if(inGameMenuShowing)
						--*inGameMenu;
					break;
				case fgeal::Event::Key::ARROW_DOWN:
					isKeyDownPressed = true;
					if(inGameMenuShowing)
						++*inGameMenu;
					break;
				case fgeal::Event::Key::ARROW_RIGHT:
					isKeyRightPressed = true;
					break;
				case fgeal::Event::Key::ARROW_LEFT:
					isKeyLeftPressed = true;
					break;
				case fgeal::Event::Key::ESCAPE:
					if(inGameMenuShowing)
						inGameMenuShowing=false;
					else
						inGameMenuShowing=true;
					break;
				case fgeal::Event::Key::ENTER:
					if(inGameMenuShowing)
					{
						switch(inGameMenu->selectedIndex)
						{
							case 0:
								inGameMenuShowing=false;
								break;
							case 1:
								Map::saveRawMapToFile(string("resources/maps/saved_map.txt"), game_map);
								running=false;
								break;
							case 2:
								running=false;
								break;
						}
					}
					break;
				default:
					break;
				}
			}
			else if(ev->getEventType() == fgeal::Event::Type::MOUSE_BUTTON_PRESS)
			{

				if(ev->getEventMouseButton() == fgeal::Event::MouseButton::RIGHT)
				{
					unsigned int mx = (visibleArea.x + ev->getEventMouseX())/BLOCK_SIZE;
					unsigned int my = (visibleArea.y + ev->getEventMouseY())/BLOCK_SIZE;

					if(mx < game_map->grid.capacity() && my < game_map->grid[0].capacity()) // in case you click outside the map
						if (game_map->grid[mx][my] == NULL)
						{
							game_map->grid[mx][my] = new Block(tileset_dirt, mx, my, 1);
							world->addBody(game_map->grid[mx][my]->body);
							game_map->retile(game_map->grid[mx][my]);
						}

				}
				else if (ev->getEventMouseButton() == fgeal::Event::MouseButton::LEFT)
				{
					unsigned int mx = (visibleArea.x + ev->getEventMouseX())/BLOCK_SIZE;
					unsigned int my = (visibleArea.y + ev->getEventMouseY())/BLOCK_SIZE;

					if(mx < 0) mx = 0; // safety
					if(my < 0) my = 0; // safety
					if(mx < game_map->grid.capacity() && my < game_map->grid[0].capacity()) // in case you click outside the map
						if (game_map->grid[mx][my] != NULL)
						{
							world->destroyBody(game_map->grid[mx][my]->body);
							delete game_map->grid[mx][my];
							game_map->grid[mx][my] = NULL;
							game_map->retileNeighbourhood(mx, my);
						}
				}
			}
		}

	}

	void drawScene()
	{
		fgeal::display->clear();
		world->step((1.0f / 60.0f), 6, 2);
		/* needs to draw HUD */

		game_map->draw_bg_player();

		player->draw(); //draw player

		game_map->draw_fg_player();

		/* drawing others entities */
		for(vector<Entity*>::iterator it = entities.begin() ; it != entities.end(); ++it){
			if (*it != NULL) (*it)->draw();
		}

		/* later should be a character class */

		//DEBUG!!!
		drawDebug();

		if(inGameMenuShowing)
			inGameMenu->draw();


		fgeal::display->refresh();
	}


	void drawDebug()
	{
		font->drawText("## DEBUG BUILD ##", 245, 0, fgeal::Color::RED);

		font->drawText("POSITION", 0, 14, fgeal::Color::WHITE);

		font->drawText(string("x: ")+player->body->getX()+" y:"+player->body->getY(), 0, 28, fgeal::Color::WHITE);

		font->drawText("SPEED", 0, 42, fgeal::Color::WHITE);

		font->drawText(string("x: ")+player->body->getVelocity().x+" y: "+player->body->getVelocity().y, 0, 56, fgeal::Color::WHITE);

	}
};

//===========================================================================================
GameStuff* stuff;
Game::Game(const string& map_path)
{
	stuff = new GameStuff(map_path);
}

Game::~Game()
{
	delete stuff;
}

void Game::start()
{
	stuff->start();
}


