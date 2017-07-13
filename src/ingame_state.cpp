/*
 * ingame_state.cpp
 *
 *  Created on: 12 de dez de 2016
 *      Author: carlosfaruolo
 */

#include "ingame_state.hpp"

using Physics::Vector;
using Physics::convertToPixels;
using Physics::newVector;

using fgeal::Event;
using fgeal::EventQueue;
using fgeal::Color;
using fgeal::Menu;

#include <cmath>

#include "futil/collection_actions.hpp"
#include "futil/language.hpp"

using futil::remove_element;

// xxx debug
#include <iostream>
using std::cout; using std::endl;

// xxx hardcoded player body dimensions
const float player_body_width = Physics::convertToMeters(25);
const float player_body_height = Physics::convertToMeters(81);

// natty macros are natty
#define isKeyUpPressed fgeal::Keyboard::isKeyPressed(fgeal::Keyboard::KEY_ARROW_UP)
#define isKeyDownPressed fgeal::Keyboard::isKeyPressed(fgeal::Keyboard::KEY_ARROW_DOWN)
#define isKeyRightPressed fgeal::Keyboard::isKeyPressed(fgeal::Keyboard::KEY_ARROW_RIGHT)
#define isKeyLeftPressed fgeal::Keyboard::isKeyPressed(fgeal::Keyboard::KEY_ARROW_LEFT)

enum AnimEnum
{
	ANIM_PLAYER_STAND_LEFT,
	ANIM_PLAYER_STAND_RIGHT,
	ANIM_PLAYER_WALK_LEFT,
	ANIM_PLAYER_WALK_RIGHT,
	ANIM_PLAYER_JUMP_LEFT,
	ANIM_PLAYER_JUMP_RIGHT,
};

InGameState::InGameState(TerrariumGame* game)  // @suppress("Class members should be properly initialized")
: State(*game), wasInit(false)
{}

InGameState::~InGameState()
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

	foreach(StackedSingleSheetAnimation*, tileset, vector<StackedSingleSheetAnimation*>, tilesets)
	{
		delete tileset;
	}

	delete font;
	delete inGameMenu;

	delete background;

	//may be null after calling dispose()
	if(map != null) delete map;
}

void InGameState::initialize()
{
	fgeal::Display& display = fgeal::Display::getInstance();
	Properties& config = TerrariumGame::CONFIG;

	visibleArea.x = 0;
	visibleArea.y = 0;
	visibleArea.w = display.getWidth();
	visibleArea.h = display.getHeight();

	//loading font
	font = new fgeal::Font(config.get("ingame.font.filename"), atoi(config.get("ingame.font.size").c_str()));

	//loading ingame menu
	Rectangle menuSize = {visibleArea.w*0.5f-100, visibleArea.h*0.5f-32, 200, 64};
	inGameMenu = new Menu(menuSize, font, Color::ORANGE);
	inGameMenu->addEntry("Resume");
	inGameMenu->addEntry("Save and exit");
	inGameMenu->addEntry("Exit without saving");

	//loading player graphics
	Image* playerAnimationSheet = new Image(config.get("player.sprite.filename"));

	StackedSingleSheetAnimation& anim = *new StackedSingleSheetAnimation(playerAnimationSheet);

	const unsigned player_sprite_width = atoi(config.get("player.sprite.width").c_str());
	const unsigned player_sprite_height = atoi(config.get("player.sprite.height").c_str());

	const unsigned animStandLeftFrameCount = atoi(config.get("player.sprite.anim.stand_left.frame_count").c_str());
	const float animStandLeftFrameDuration = atof(config.get("player.sprite.anim.stand_left.frame_duration").c_str());
	anim.addSprite(player_sprite_width, player_sprite_height, animStandLeftFrameCount, animStandLeftFrameDuration);

	const unsigned animStandRightFrameCount = atoi(config.get("player.sprite.anim.stand_right.frame_count").c_str());
	const float animStandRightFrameDuration = atof(config.get("player.sprite.anim.stand_right.frame_duration").c_str());
	anim.addSprite(player_sprite_width, player_sprite_height, animStandRightFrameCount, animStandRightFrameDuration);

	const unsigned animWalkLeftFrameCount = atoi(config.get("player.sprite.anim.walk_left.frame_count").c_str());
	const float animWalkLeftFrameDuration = atof(config.get("player.sprite.anim.walk_left.frame_duration").c_str());
	anim.addSprite(player_sprite_width, player_sprite_height, animWalkLeftFrameCount, animWalkLeftFrameDuration);

	const unsigned animWalkRightFrameCount = atoi(config.get("player.sprite.anim.walk_right.frame_count").c_str());
	const float animWalkRightFrameDuration = atof(config.get("player.sprite.anim.walk_right.frame_duration").c_str());

	anim.addSprite(player_sprite_width, player_sprite_height, animWalkRightFrameCount, animWalkRightFrameDuration);

	const unsigned player_reference_pixel_x = atoi(config.get("player.sprite.reference_pixel.x").c_str());
	const unsigned player_reference_pixel_y = atoi(config.get("player.sprite.reference_pixel.y").c_str());
	for(unsigned i = 0; i < anim.sprites.size(); i++)
	{
		anim[i].referencePixelX = player_reference_pixel_x;
		anim[i].referencePixelY = player_reference_pixel_y;
	}

	anim.currentIndex = ANIM_PLAYER_STAND_RIGHT;

	//loading player
	entities.push_back(player = new Entity(&anim, null));

	playerJumpImpulse = player_body_width*player_body_height * 0.5;
	playerWalkForce =   player_body_width*player_body_height * 1.2;

	//loading tilesets
	tilesets.push_back(tilesetDirt =  Block::createBlockAnimationSet(new Image("resources/tileset-dirt.png")));
	tilesets.push_back(tilesetStone = Block::createBlockAnimationSet(new Image("resources/tileset-stone.png")));
	tilesets.push_back(tilesetWater = Block::createBlockAnimationSet(new Image("resources/tileset-water.png"), 3, 1.0));
	tilesets.push_back(tilesetGrass = Block::createBlockAnimationSet(new Image("resources/tileset-grass.png")));

	//loading some icons
	iconBlockDirt = new Sprite(tilesetDirt->sheet, BLOCK_SIZE, BLOCK_SIZE);
	iconBlockDirt->scale.x = 0.5;
	iconBlockDirt->scale.y = 0.5;

	iconBlockStone = new Sprite(tilesetStone->sheet, BLOCK_SIZE, BLOCK_SIZE);
	iconBlockStone->scale.x = 0.5;
	iconBlockStone->scale.y = 0.5;

	//load bg
	Image* bgImg = new Image(config.get("ingame.bg.filename"));
	background = new Sprite(bgImg, bgImg->getWidth(), bgImg->getHeight(), -1, -1, 0, 0, true);
	background->scale.x = fgeal::Display::getInstance().getWidth()  / background->getCurrentFrame().w;
	background->scale.y = fgeal::Display::getInstance().getHeight() / background->getCurrentFrame().h;

	map = null; // we need to nullify to know afterwards if there was initialization

	wasInit = true;  // mark the this state was initialized successfully
}

void InGameState::onEnter()
{
	//setting flags
	jumping = false;
	inGameMenuShowing = false;

	//loading map in world
	TerrariumGame& game = *static_cast<TerrariumGame*>(&this->game);
	map = new Map(this, game.stageFilename);
	cout << "map size (in pixels): " << map->computeDimensions().w << "x" << map->computeDimensions().h << endl;

	//create player body for the newly created world and reset sprite animation
	if(player->body != null) delete player->body;
	player->body = new Body(1, 1, player_body_width, player_body_height);
	map->world->addBody(player->body);
	player->body->setFixedRotation();
	player->animation->currentIndex = ANIM_PLAYER_STAND_RIGHT;
}

void InGameState::onLeave()
{
	delete map;
	map = null;
}

void InGameState::render()
{
	fgeal::Display::getInstance().clear();

	/* needs to draw HUD */

	background->draw();
	map->draw();

	/* drawing others entities */
	foreach(Entity*, entity, vector<Entity*>, entities)
	{
		entity->draw(visibleArea);
	}

	map->drawOverlay();

	/* later should be a character class */

	//DEBUG!!!
	{
		font->drawText("## DEBUG BUILD ##", 245, 0, fgeal::Color::RED);
		font->drawText("POSITION", 0, 14, fgeal::Color::WHITE);
		font->drawText(string("x: ")+player->body->getX()+" y:"+player->body->getY(), 0, 28, fgeal::Color::WHITE);
		font->drawText("SPEED", 0, 42, fgeal::Color::WHITE);
		font->drawText(string("x: ")+player->body->getVelocity().x+" y: "+player->body->getVelocity().y, 0, 56, fgeal::Color::WHITE);
		font->drawText(string("FPS: ")+game.getFpsCount(), 0, 96, fgeal::Color::WHITE);
	}

	if(inGameMenuShowing)
		inGameMenu->draw();
}

void InGameState::update(float delta)
{
	//visible area quick n' dirt fix
	{
		visibleArea.x = convertToPixels(player->body->getX()) - visibleArea.w/2.0;
		visibleArea.y = convertToPixels(player->body->getY()) - visibleArea.h/2.0;

		Rectangle mapSize = map->computeDimensions();

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
			player->body->applyImpulse(newVector(0.0f, -playerJumpImpulse), newVector(player->body->getX(), player->body->getY()));
			jumping = true;
		}
	}
	if(not inGameMenuShowing and isKeyDownPressed) {
		Vector v = player->body->getVelocity();
		if(v.y <= 2.0f)
			player->body->applyForceToCenter(newVector(0.0f, 0.2f));
	}
	if(not inGameMenuShowing and isKeyRightPressed) {
		player->animation->currentIndex = ANIM_PLAYER_WALK_RIGHT;
		Vector v = player->body->getVelocity();
		if(v.x <= 2.0f)
			player->body->applyForceToCenter(newVector(playerWalkForce, 0.0f));
	}
	if(not inGameMenuShowing and isKeyLeftPressed) {
		player->animation->currentIndex = ANIM_PLAYER_WALK_LEFT;
		Vector v = player->body->getVelocity();
		if(v.x >= -2.0f)
			player->body->applyForceToCenter(newVector(-playerWalkForce, 0.0f));
	}

	this->handleInput();
	map->world->step(delta, 6, 2);
	vector<Entity*> trash;
	foreach(Entity*, entity, vector<Entity*>, entities)
	{
		if(entity != player)
		{
			const Physics::Vector distanceVector = player->body->getCenter() - entity->body->getCenter();
			const double distanceLength = distanceVector.length();
			if(distanceLength < 0.1)
			{
				trash.push_back(entity);
				cout << "prop crush!" << endl;
			}

			else if(distanceLength < 0.8)
			{
				const double magnetude = 0.05*(1-1/(1+distanceLength));
				entity->body->applyForceToCenter(distanceVector.unit().scale(magnetude));
			}
		}
	}
	foreach(Entity*, entity, vector<Entity*>, trash)
	{
		remove_element(entities, entity);
		delete entity;
	}
}

void InGameState::handleInput()
{
	Event event;
	EventQueue& eventQueue = EventQueue::getInstance();
	while(not eventQueue.isEmpty())
	{
		eventQueue.waitNextEvent(&event);

		if(event.getEventType() == fgeal::Event::TYPE_DISPLAY_CLOSURE)
		{
			game.enterState(TerrariumGame::MAIN_MENU_STATE_ID);
		}
		else if(event.getEventType() == fgeal::Event::TYPE_KEY_RELEASE)
		{
			switch(event.getEventKeyCode())
			{
				case fgeal::Keyboard::KEY_ARROW_UP:
					jumping = false;
					break;
				case fgeal::Keyboard::KEY_ARROW_RIGHT:
					player->animation->currentIndex = ANIM_PLAYER_STAND_RIGHT;
					break;
				case fgeal::Keyboard::KEY_ARROW_LEFT:
					player->animation->currentIndex = ANIM_PLAYER_STAND_LEFT;
					break;
				default:
					break;
			}
		}
		else if(event.getEventType() == fgeal::Event::TYPE_KEY_PRESS)
		{
			switch(event.getEventKeyCode())
			{
				case fgeal::Keyboard::KEY_ARROW_UP:
					if(inGameMenuShowing)
						--*inGameMenu;
					break;
				case fgeal::Keyboard::KEY_ARROW_DOWN:
					if(inGameMenuShowing)
						++*inGameMenu;
					break;
				case fgeal::Keyboard::KEY_ESCAPE:
					if(inGameMenuShowing)
						inGameMenuShowing=false;
					else
						inGameMenuShowing=true;
					break;
				case fgeal::Keyboard::KEY_ENTER:
					if(inGameMenuShowing)
					{
						switch(inGameMenu->getSelectedIndex())
						{
							case 0:
								inGameMenuShowing=false;
								break;
							case 1:
								// todo create a dialog to choose file name
								map->saveToFile("resources/maps/saved_map.txt");
								game.enterState(TerrariumGame::MAIN_MENU_STATE_ID);
								break;
							case 2:
								game.enterState(TerrariumGame::MAIN_MENU_STATE_ID);
								break;
						}
					}
					break;
				default:
					break;
			}
		}
		else if(event.getEventType() == fgeal::Event::TYPE_MOUSE_BUTTON_PRESS)
		{
			if(event.getEventMouseButton() == fgeal::Mouse::BUTTON_RIGHT)
			{
				unsigned int mx = (visibleArea.x + event.getEventMouseX())/BLOCK_SIZE;
				unsigned int my = (visibleArea.y + event.getEventMouseY())/BLOCK_SIZE;

				if(mx < map->grid.capacity() && my < map->grid[0].capacity()) // in case you click outside the map
					if (map->grid[mx][my] == NULL)
						map->addBlock(mx, my);
			}
			else if (event.getEventMouseButton() == fgeal::Mouse::BUTTON_LEFT)
			{
				unsigned int mx = (visibleArea.x + event.getEventMouseX())/BLOCK_SIZE;
				unsigned int my = (visibleArea.y + event.getEventMouseY())/BLOCK_SIZE;

				if(mx < 0) mx = 0; // safety
				if(my < 0) my = 0; // safety
				if(mx < map->grid.capacity() && my < map->grid[0].capacity()) // in case you click outside the map
					if (map->grid[mx][my] != NULL)
						map->deleteBlock(mx, my);
			}
			else if (event.getEventMouseButton() == fgeal::Mouse::BUTTON_MIDDLE)
			{
				const float posx = Physics::convertToMeters(visibleArea.x + event.getEventMouseX()),
							posy = Physics::convertToMeters(visibleArea.y + event.getEventMouseY()),
							physicalBlockSize = Physics::convertToMeters(BLOCK_SIZE);

				Body* detatchedBlockBody = new Body(posx, posy, 0.5*physicalBlockSize, 0.5*physicalBlockSize, Body::Type::DROP);
				Entity* detatchedBlock = new Entity(new Animation(new Sprite(*iconBlockDirt)), detatchedBlockBody);
				map->world->addBody(detatchedBlockBody);
				detatchedBlockBody->setFixedRotation(false);
				detatchedBlockBody->applyForceToCenter(newVector(0.0f, -playerJumpImpulse*0.5));
				entities.push_back(detatchedBlock);
				cout << "pooped block!" << endl;
			}
		}
	}
}
