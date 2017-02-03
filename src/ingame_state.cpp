/*
 * ingame_state.cpp
 *
 *  Created on: 12 de dez de 2016
 *      Author: carlosfaruolo
 */

#include "ingame_state.hpp"

#include "fgeal/fgeal.hpp"

#include "futil/string/more_operators.hpp"

#include "menu.hpp"
#include "block.hpp"
#include "entity.hpp"
#include "map.hpp"

using Physics::World;
using Physics::Vector;
using Physics::convertToPixels;

using fgeal::Event;
using fgeal::EventQueue;
using fgeal::Font;
using fgeal::Color;

//xxx debug
#include <iostream>
using std::cout; using std::endl;

// xxx hardcoded
float player_body_width, player_body_height;

struct InGameState::implementation
{
	TerrariumGame& game; // helper reference
	bool wasInit;

	implementation(TerrariumGame* terrariumGame)  // @suppress("Class members should be properly initialized")
	: game(*terrariumGame), wasInit(false)
	{}

	// --------------------------------------------------

	vector<Entity*> entities;
	vector<Image*> images;

	Rectangle visibleArea;

	Entity* player;
	Map* game_map;
	Image* tileset_dirt, *playerAnimationSheet;
	AnimationSet* playerAnimation;
	Font* font;
	Menu* inGameMenu;
	World* world;

	bool jumping, inGameMenuShowing;
	bool isKeyUpPressed, isKeyDownPressed, isKeyRightPressed, isKeyLeftPressed;

	float playerJumpImpulse, playerWalkForce;

	void init()
	{
		wasInit = true;
		Rectangle size = {0, 0, (float) fgeal::Display::getInstance().getWidth(), (float) fgeal::Display::getInstance().getHeight()};
		visibleArea = size;

		//loading font
		font = new fgeal::Font("resources/liberation.ttf", 14);

		//loading ingame menu
		Rectangle menuSize = {200, 200, 200, 64};
		inGameMenu = new Menu(menuSize, font, Color::ORANGE);
		inGameMenu->addEntry("Resume");
		inGameMenu->addEntry("Save and exit");
		inGameMenu->addEntry("Exit without saving");

		//loading player graphics

		// Pijamaman specs
		unsigned player_sprite_width = 56;
		unsigned player_sprite_height = 84;
		player_body_width = Physics::convertToMeters(25);
		player_body_height = Physics::convertToMeters(81);

		playerAnimationSheet = new Image("resources/pijamaman-1.png");
		playerAnimation = new AnimationSet(playerAnimationSheet);

		playerAnimation->add("still-left", player_sprite_width, player_sprite_height, 1, 1);
		playerAnimation->ref("still-left").referencePixelY = -2;

		playerAnimation->add("still-right", player_sprite_width, player_sprite_height, 1, 1);
		playerAnimation->ref("still-right").referencePixelY = -2;

		playerAnimation->add("walk-left", player_sprite_width, player_sprite_height, 0.25, 4);
		playerAnimation->ref("walk-left").referencePixelY = -2;

		playerAnimation->add("walk-right", player_sprite_width, player_sprite_height, 0.25, 4);
		playerAnimation->ref("walk-right").referencePixelY = -2;

		playerAnimation->setCurrent("still-right");

		//loading player physics
		player = new Entity(playerAnimation, null, &visibleArea);

		playerJumpImpulse = player_body_width*player_body_height * 0.5;
		playerWalkForce =   player_body_width*player_body_height * 1.2;

		//loading dirt tileset
		tileset_dirt = new Image("resources/tileset-dirt.png");
		game_map = null; // we need to nullify to know afterwards if there was initialization
		world = null; // we need to nullify to know afterwards if there was initialization
	}

	~implementation()
	{
		if(not wasInit)
		{
			cout << "ingame state was not initialized, skip destructor." << endl;
			return;
		}

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
		delete tileset_dirt;
		delete font;
		delete inGameMenu;

		//may be null after calling dispose()
		if(game_map != null) delete game_map;
		if(world != null) delete world;
	}

	void setup()
	{
		//setting flags
		jumping = false;
		inGameMenuShowing = false;
		isKeyUpPressed = false;
		isKeyDownPressed = false;
		isKeyRightPressed = false;
		isKeyLeftPressed = false;

		//create new world
		world = new World(Vector(0.0, 10.0));

		//loading map in world
		game_map = Map::loadMapFromFile(game.stageFilename, world, images);
		game_map->visibleArea = &visibleArea;
		cout << "map size (in pixels): " << game_map->computeDimensions().w << "x" << game_map->computeDimensions().h << endl;

		//create player body for the newly created world and reset sprite animation
		if(player->body != null) delete player->body;
		player->body = new Body(1, 1, player_body_width, player_body_height);
		world->addBody(player->body);
		player->body->setFixedRotation();
		player->animation->setCurrent("still-right");
	}

	void dispose()
	{
		delete world;
		world = null;

		delete game_map;
		game_map = null;
	}

	void handleInput()
	{
		Event event;
		EventQueue& eventQueue = EventQueue::getInstance();
		while(not eventQueue.isEmpty())
		{
			eventQueue.waitNextEvent(&event);

			if(event.getEventType() == fgeal::Event::Type::DISPLAY_CLOSURE)
			{
				game.enterState(TerrariumGame::MENU_STATE_ID);
			}
			else if(event.getEventType() == fgeal::Event::Type::KEY_RELEASE)
			{
				switch(event.getEventKeyCode())
				{
				case fgeal::Keyboard::Key::ARROW_UP:
					isKeyUpPressed = false;
					jumping = false;
					break;
				case fgeal::Keyboard::Key::ARROW_DOWN:
					isKeyDownPressed = false;
					break;
				case fgeal::Keyboard::Key::ARROW_RIGHT:
					player->animation->setCurrent("still-right");
					isKeyRightPressed = false;
					break;
				case fgeal::Keyboard::Key::ARROW_LEFT:
					player->animation->setCurrent("still-left");
					isKeyLeftPressed = false;
					break;
				default:
					break;
				}
			}
			else if(event.getEventType() == fgeal::Event::Type::KEY_PRESS)
			{
				switch(event.getEventKeyCode())
				{
				case fgeal::Keyboard::Key::ARROW_UP:
					isKeyUpPressed = true;
					if(inGameMenuShowing)
						--*inGameMenu;
					break;
				case fgeal::Keyboard::Key::ARROW_DOWN:
					isKeyDownPressed = true;
					if(inGameMenuShowing)
						++*inGameMenu;
					break;
				case fgeal::Keyboard::Key::ARROW_RIGHT:
					isKeyRightPressed = true;
					break;
				case fgeal::Keyboard::Key::ARROW_LEFT:
					isKeyLeftPressed = true;
					break;
				case fgeal::Keyboard::Key::ESCAPE:
					if(inGameMenuShowing)
						inGameMenuShowing=false;
					else
						inGameMenuShowing=true;
					break;
				case fgeal::Keyboard::Key::ENTER:
					if(inGameMenuShowing)
					{
						switch(inGameMenu->selectedIndex)
						{
							case 0:
								inGameMenuShowing=false;
								break;
							case 1:
								Map::saveRawMapToFile(string("resources/maps/saved_map.txt"), game_map);
								game.enterState(TerrariumGame::MENU_STATE_ID);
								break;
							case 2:
								game.enterState(TerrariumGame::MENU_STATE_ID);
								break;
						}
					}
					break;
				default:
					break;
				}
			}
			else if(event.getEventType() == fgeal::Event::Type::MOUSE_BUTTON_PRESS)
			{

				if(event.getEventMouseButton() == fgeal::Mouse::Button::RIGHT)
				{
					unsigned int mx = (visibleArea.x + event.getEventMouseX())/BLOCK_SIZE;
					unsigned int my = (visibleArea.y + event.getEventMouseY())/BLOCK_SIZE;

					if(mx < game_map->grid.capacity() && my < game_map->grid[0].capacity()) // in case you click outside the map
						if (game_map->grid[mx][my] == NULL)
						{
							game_map->grid[mx][my] = new Block(tileset_dirt, mx, my, 1);
							world->addBody(game_map->grid[mx][my]->body);
							game_map->retile(game_map->grid[mx][my]);
						}

				}
				else if (event.getEventMouseButton() == fgeal::Mouse::Button::LEFT)
				{
					unsigned int mx = (visibleArea.x + event.getEventMouseX())/BLOCK_SIZE;
					unsigned int my = (visibleArea.y + event.getEventMouseY())/BLOCK_SIZE;

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

	void update(float delta)
	{
		//visible area quick n' dirt fix
		{
			visibleArea.x = convertToPixels(player->body->getX()) - visibleArea.w/2.0;
			visibleArea.y = convertToPixels(player->body->getY()) - visibleArea.h/2.0;

			Rectangle mapSize = game_map->computeDimensions();

			// adjusts visibleArea if it falls outside map bounds (and only if the player is not outside bounds itself as well)
			if(player->body->getX() + player->body->getWidth() > 0 and player->body->getY() + player->body->getHeight() > 0
			and convertToPixels(player->body->getX()) < mapSize.w and convertToPixels(player->body->getY()) < mapSize.h)
			{
				if(visibleArea.x < 0) visibleArea.x = 0;
				if(visibleArea.y < 0) visibleArea.y = 0;

				if(visibleArea.x + visibleArea.w > mapSize.w)
					visibleArea.x = mapSize.w - visibleArea.w;

				if(visibleArea.y + visibleArea.h > mapSize.h)
					visibleArea.y = mapSize.h - visibleArea.h;
			}
		}

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

		this->handleInput();
		world->step(delta, 6, 2);
	}

	void drawScene()
	{
		fgeal::Display::getInstance().clear();
		/* needs to draw HUD */

		game_map->draw_bg_player();

		player->draw(); //draw player

		game_map->draw_fg_player();

		/* drawing others entities */
		for(vector<Entity*>::iterator it = entities.begin() ; it != entities.end(); ++it){
			if (*it != null) (*it)->draw();
		}

		/* later should be a character class */

		//DEBUG!!!
		drawDebug();

		if(inGameMenuShowing)
			inGameMenu->draw();
	}

	void drawDebug()
	{
		font->drawText("## DEBUG BUILD ##", 245, 0, fgeal::Color::RED);
		font->drawText("POSITION", 0, 14, fgeal::Color::WHITE);
		font->drawText(string("x: ")+player->body->getX()+" y:"+player->body->getY(), 0, 28, fgeal::Color::WHITE);
		font->drawText("SPEED", 0, 42, fgeal::Color::WHITE);
		font->drawText(string("x: ")+player->body->getVelocity().x+" y: "+player->body->getVelocity().y, 0, 56, fgeal::Color::WHITE);
		font->drawText(string("FPS: ")+game.getFpsCount(), 0, 96, fgeal::Color::WHITE);
	}
};

InGameState::InGameState(TerrariumGame* game) : State(*game), self(*new implementation(game)) {}
InGameState::~InGameState() { delete &self; }

void InGameState::initialize() { self.init(); }

void InGameState::onEnter() { self.setup(); }
void InGameState::onLeave() { self.dispose(); }

void InGameState::render() { self.drawScene(); }
void InGameState::update(float delta) { self.update(delta); }
