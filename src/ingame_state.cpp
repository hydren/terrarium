/*
 * ingame_state.cpp
 *
 *  Created on: 12 de dez de 2016
 *      Author: carlosfaruolo
 */

#include "ingame_state.hpp"

#include "futil/collection_actions.hpp"
#include "futil/language.hpp"
#include "futil/string_actions.hpp"
#include "futil/random.h"

#include <cmath>
#include <climits>

// xxx debug
#include <iostream>
using std::cout;
using std::endl;
// xxx debug

using Physics::Vector;
using Physics::convertToPixels;
using Physics::convertToMeters;
using Physics::newVector;
using fgeal::Image;
using fgeal::Event;
using fgeal::EventQueue;
using fgeal::Color;
using fgeal::Menu;
using fgeal::Keyboard;
using fgeal::Mouse;
using futil::remove_element;
using futil::Properties;

// xxx hardcoded player body dimensions
const float player_body_width = Physics::convertToMeters(25);
const float player_body_height = Physics::convertToMeters(81);

// lazy as f****
#define isKeyUpPressed    Keyboard::isKeyPressed(fgeal::Keyboard::KEY_ARROW_UP)
#define isKeyDownPressed  Keyboard::isKeyPressed(fgeal::Keyboard::KEY_ARROW_DOWN)
#define isKeyRightPressed Keyboard::isKeyPressed(fgeal::Keyboard::KEY_ARROW_RIGHT)
#define isKeyLeftPressed  Keyboard::isKeyPressed(fgeal::Keyboard::KEY_ARROW_LEFT)

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

	foreach(Actor*, a, vector<Actor*>, actors)
	{
		delete a;
	}

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

	delete backgroundDay;
	delete backgroundNight;

	//may be null after calling dispose()
	if(map != null) delete map;
}

void InGameState::initialize()
{
	Properties& config = static_cast<TerrariumGame&>(game).logic.config;  // @suppress("Field cannot be resolved")

	//loading font
	font = new fgeal::Font(config.get("ingame.font.filename"), atoi(config.get("ingame.font.size").c_str()));
	fontInventory = new fgeal::Font(config.get("ingame.inventory.font.filename"), atoi(config.get("ingame.inventory.font.size").c_str()));

	hourDuration = config.getParsedCStrAllowDefault<double, atof>("ingame.hour_duration", 1);

	//loading ingame menu
	Rectangle menuSize = {0, 0, 200, 64};
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
	actors.push_back(player = new Actor(&anim, null, "player"));

	playerJumpImpulse = player_body_width*player_body_height * 0.5;
	playerWalkForce =   player_body_width*player_body_height * 1.2;

	//loading tilesets
	tilesets.push_back(null);  // null tileset (id 0)
	for(unsigned i = 1; i < 1024; i++)	//xxx hardcoded limit for tilesets IDs
	{
		const string baseKey = "ingame.tileset"+futil::to_string(i), filenameKey = baseKey+".sprite.filename";
		if(config.containsKey(filenameKey))
		{
			cout << "loading tileset as specified by " << baseKey << " (\"" << config.get(filenameKey) << "\")..." << endl;
			tilesets.push_back(Block::createBlockAnimationSet(
				new Image(config.get(filenameKey)),
				config.getParsedCStrAllowDefault<int, atoi>(baseKey+".sprite.frame_count", 1),
				config.getParsedCStrAllowDefault<double, atof>(baseKey+".sprite.frame_duration", -1.0)));
		}
	}

	//loading items
	itemTypeInfo.push_back(Item::Type());  // null item type (id 0)
	for(unsigned i = 1; i < 1024; i++)	//xxx hardcoded limit for item type IDs
	{
		const string baseKey = "item_type"+futil::to_string(i), nameKey = baseKey+".name";
		if(config.containsKey(nameKey))
		{
			cout << "loading item type as specified by " << baseKey << " (\"" << config.get(nameKey) << "\")..." << endl;
			itemTypeInfo.push_back(Item::Type());
			Item::Type& type = itemTypeInfo.back();
			type.id = i;
			type.name = config.get(nameKey);
			type.description = config.get(baseKey+".description");

			type.stackingLimit = config.getParsedCStr<int, atoi>(baseKey+".stacking_limit", 1);
			type.mass = config.getParsedCStr<double, atof>(baseKey+".mass");
			type.isPlaceable = config.get(baseKey+".placeability", "none") == "ground";
			type.isDiggingTool = config.get(baseKey+".usable_action", "none") == "mining";
			type.itemSlotCount = config.getParsedCStr<int, atoi>(baseKey+".capaciousness", 0);
			type.isStartupItem = config.get(baseKey+".is_startup_item", "false") == "true";
			type.placedBlockTypeId = config.getParsedCStr<int, atoi>(baseKey+".placed_block_type_id", 0);

			string iconKey = baseKey + ".icon.filename";
			if(config.containsKey(iconKey))
			{
				type.icon = new Sprite(
					new Image(config.get(iconKey)),
					config.getParsedCStrAllowDefault<int, atoi>(baseKey+".icon.width", DEFAULT_ICON_SIZE),
					config.getParsedCStrAllowDefault<int, atoi>(baseKey+".icon.height", DEFAULT_ICON_SIZE),
					config.getParsedCStrAllowDefault<double, atof>(baseKey+".icon.frame_duration", -1.0),
					config.getParsedCStrAllowDefault<int, atoi>(baseKey+".icon.raw_frame_count", -1),
					config.getParsedCStrAllowDefault<int, atoi>(baseKey+".icon.raw_offset.x", 0),
					config.getParsedCStrAllowDefault<int, atoi>(baseKey+".icon.raw_offset.y", 0),
					true);

				// default scale
				if(type.icon->width  != DEFAULT_ICON_SIZE) type.icon->scale.x = DEFAULT_ICON_SIZE/(float)type.icon->width;
				if(type.icon->height != DEFAULT_ICON_SIZE) type.icon->scale.y = DEFAULT_ICON_SIZE/(float)type.icon->height;

				// optional scale
				type.icon->scale.x = config.getParsedCStr<double, atof>(baseKey+".icon.scale.x", type.icon->scale.x);
				type.icon->scale.y = config.getParsedCStr<double, atof>(baseKey+".icon.scale.y", type.icon->scale.y);
			}
			else type.icon = null;
		}
	}

	// loading block types
	blockTypeInfo.push_back(Block::Type());  // null block type (id 0)
	for(unsigned i = 1; i < 1024; i++)	//xxx hardcoded limit for block type IDs
	{
		const string baseKey = "block_type"+futil::to_string(i), nameKey = baseKey+".name";
		if(config.containsKey(nameKey))
		{
			cout << "loading block type as specified by " << baseKey << " (\"" << config.get(nameKey) << "\")..." << endl;
			blockTypeInfo.push_back(Block::Type());
			Block::Type& type = blockTypeInfo.back();
			type.id = i;
			type.name = config.get(nameKey);
			type.description = config.get(baseKey+".description");
			type.pickaxeMinerable = (config.get(baseKey+".minerable_by", "none") == "pickaxe");
			type.detatchedItemTypeId = config.getParsedCStr<int, atoi>(baseKey+".detatched_item_type_id", 0);

			string valueTxt = config.get(baseKey+".passability", "none");
			if(valueTxt == "full") type.passability = Block::Type::PASSABILITY_FULL;
			else type.passability = Block::Type::PASSABILITY_NONE;

			valueTxt = config.get(baseKey+".precipitability", "none");
			if(valueTxt == "liquidous") type.precipitability = Block::Type::PRECIPITABILITY_LIQUIDOUS;
			else if(valueTxt == "arenaceous") type.precipitability = Block::Type::PRECIPITABILITY_ARENACEOUS;
			else type.precipitability = Block::Type::PRECIPITABILITY_NONE;
		}
	}

	//load bg
	Image* bgImgDay = new Image(config.get("ingame.bg_day.filename"));
	backgroundDay = new Sprite(bgImgDay, bgImgDay->getWidth(), bgImgDay->getHeight(), -1, -1, 0, 0, true);

	Image* bgImgNight = new Image(config.get("ingame.bg_night.filename"));
	backgroundNight = new Sprite(bgImgNight, bgImgNight->getWidth(), bgImgNight->getHeight(), -1, -1, 0, 0, true);

	inventory = null;
	inventoryColor = Color::parseCStr(config.get("ingame.inventory.color").c_str(), true);
	inventoryColor.a = 128;
	inventoryFontColor = Color::parseCStr(config.get("ingame.inventory.font.color").c_str(), true);
	inventoryFontColor.a = 255;

	map = null; // we need to nullify to know afterwards if there was initialization

	wasInit = true;  // mark the this state was initialized successfully
}

void InGameState::onEnter()
{
	if(stageFilename.empty())
	{
		// todo throw a proper exception here
		cout << "no stage filename specified!" << endl;
		game.enterState(TerrariumGame::MAIN_MENU_STATE_ID);
		return;
	}

	fgeal::Display& display = fgeal::Display::getInstance();

	visibleArea.x = 0;
	visibleArea.y = 0;
	visibleArea.w = display.getWidth();
	visibleArea.h = display.getHeight();

	inGameMenu->bounds.x = 0.5f*(visibleArea.w - inGameMenu->bounds.w);
	inGameMenu->bounds.y = 0.5f*(visibleArea.h - inGameMenu->bounds.h);

	backgroundDay->scale.x = display.getWidth()  / backgroundDay->getCurrentFrame().w;
	backgroundDay->scale.y = display.getHeight() / backgroundDay->getCurrentFrame().h;

	backgroundNight->scale.x = display.getWidth()  / backgroundNight->getCurrentFrame().w;
	backgroundNight->scale.y = display.getHeight() / backgroundNight->getCurrentFrame().h;

	//setting flags
	jumping = false;
	inGameMenuShowing = false;

	//loading map in world
	map = new Map(this, stageFilename);
	cout << "map size (in pixels): " << map->computeDimensions().w << "x" << map->computeDimensions().h << endl;

	//create player body for the newly created world and reset sprite animation
	if(player->body != null) delete player->body;
	player->body = new Body(1, 1, player_body_width, player_body_height);
	map->world->addBody(player->body);
	player->body->setFixedRotation();
	player->animation->currentIndex = ANIM_PLAYER_STAND_RIGHT;

	const Rectangle inventoryBounds = {
		0.25f * display.getWidth(), 1.25f * BLOCK_SIZE,
		0.5f  * display.getWidth(), 0.25f * display.getHeight()
	};

	Inventory::GLOBAL_INVENTORY_ITEM_TYPE.name = "inventory";
	Inventory::GLOBAL_INVENTORY_ITEM_TYPE.description = "the player's inventory";
	Inventory::GLOBAL_INVENTORY_ITEM_TYPE.stackingLimit = 1;
	Inventory::GLOBAL_INVENTORY_ITEM_TYPE.mass = 0.5;
	Inventory::GLOBAL_INVENTORY_ITEM_TYPE.icon = null;
	Inventory::GLOBAL_INVENTORY_ITEM_TYPE.isPlaceable = false;
	Inventory::GLOBAL_INVENTORY_ITEM_TYPE.isDiggingTool = false;
	Inventory::GLOBAL_INVENTORY_ITEM_TYPE.itemSlotCount = 32;

	player->containerItem = new Item(Inventory::GLOBAL_INVENTORY_ITEM_TYPE);

	inventory = new Inventory(inventoryBounds, fontInventory, player->containerItem);
	inventory->color = inventoryColor;
	inventory->colorFont = inventoryFontColor;
	inventoryVisible = false;
	cursorHeldItem = null;

	if(not fgeal::filesystem::isFilenameArchive(characterFilename))
		throw std::logic_error("character filename is not a valid file");

	Properties charProp;
	charProp.load(characterFilename);

	if(charProp.containsKey("new") and charProp.get("new") == "true")
	{
		// add startup items to freshly created character
		for(unsigned i = 0; i < itemTypeInfo.size(); i++)
		{
			if(itemTypeInfo[i].isStartupItem)
				inventory->add(new Item(itemTypeInfo[i]));
		}
	}
	else for(unsigned i = 0; i < inventory->container->type.itemSlotCount; i++)
	{
		const string baseKey = string("item") + futil::to_string(i),
					 idKey = baseKey + "_id";

		if(charProp.containsKey(idKey))
		{
			unsigned typeId = charProp.getParsedCStr<int, atoi>(idKey);
			if(typeId != 0)
			{
				Item* item = new Item(itemTypeInfo[typeId]);
				item->amount = charProp.getParsedCStr<int, atoi>(baseKey+"_amount", 1);
				inventory->add(item);
			}
		}
	}


	player->label = charProp.get("name", "unamed");

	ingameTime = 7*hourDuration*60;
	inventoryItemHovered = null;
	inventoryItemHoverTime = 0;
}

void InGameState::onLeave()
{
	delete map;
	map = null;

	delete inventory;
	inventory = null;
}

void InGameState::render()
{
	fgeal::Display& display = fgeal::Display::getInstance();
	display.clear();

	/* needs to draw HUD */

	const float period = 24*hourDuration*60, timeOfDay = fmod(ingameTime, (double) period);

	if(timeOfDay > 0.25*period and timeOfDay < 0.75*period)
		backgroundDay->draw();
	else
		backgroundNight->draw();

	map->draw();

	/* drawing others entities */
	foreach(Entity*, entity, vector<Entity*>, entities)
	{
		entity->draw(visibleArea);
	}

	map->drawOverlay();

	const float maxDarkening = 0.75;
	Color darkFilterColor(0, 0, 0, 0);

	// sunset
	if(timeOfDay > 0.7*period)
	{
		const float proportion = (timeOfDay/period - 0.7)*(1/0.3);
		darkFilterColor.a = UCHAR_MAX * maxDarkening * proportion;
	}

	// dawn
	else if(timeOfDay < 0.3*period)
	{
		const float proportion = (0.3 - timeOfDay/period)*(1/0.3);
		darkFilterColor.a = UCHAR_MAX * maxDarkening * proportion;
	}

	Image::drawFilledRectangle(0, 0, display.getWidth(), display.getHeight(), darkFilterColor);

	/* later should be a character class */

	//DEBUG!!!
	{
		font->drawText("## DEBUG BUILD ##", 245, 0, Color::RED);
		font->drawText("POSITION", 0, 14, Color::WHITE);
		font->drawText(string("x: ")+player->body->getX()+" y:"+player->body->getY(), 0, 28, Color::WHITE);
		font->drawText("SPEED", 0, 42, Color::WHITE);
		font->drawText(string("x: ")+player->body->getVelocity().x+" y: "+player->body->getVelocity().y, 0, 56, Color::WHITE);
		font->drawText(string("Time: ")+((int)(24*timeOfDay/period))+":00", 0, 72, Color::WHITE);
		font->drawText(string("FPS: ")+game.getFpsCount(), 0, 96, Color::WHITE);
	}

	if     (player->currentHp >= 1.00 * player->maxHp) font->drawText("Unharmed", 0.8*display.getWidth(), 1.1*font->getHeight(), Color::CYAN);
	else if(player->currentHp >  0.75 * player->maxHp) font->drawText("Good", 0.8*display.getWidth(), 1.1*font->getHeight(), Color::GREEN);
	else if(player->currentHp >  0.50 * player->maxHp) font->drawText("Injured", 0.8*display.getWidth(), 1.1*font->getHeight(), Color::YELLOW);
	else if(player->currentHp >  0.25 * player->maxHp) font->drawText("Badly Injured", 0.8*display.getWidth(), 1.1*font->getHeight(), Color::ORANGE);
	else if(player->currentHp >  0.00 * player->maxHp) font->drawText("Near death", 0.8*display.getWidth(), 1.1*font->getHeight(), Color::RED);
	else 										       font->drawText("Dead", 0.8*display.getWidth(), 1.1*font->getHeight(), Color::MAROON);

	if(inventoryVisible)
		inventory->draw();

	if(cursorHeldItem != null)
		cursorHeldItem->draw(Mouse::getPositionX(), Mouse::getPositionY(), font, Color::BLACK);

	if(inventoryItemHovered != null and (fgeal::uptime() - inventoryItemHoverTime) > 1.0)
		font->drawText(itemTypeInfo[inventoryItemHovered->type.id].name, Mouse::getPositionX(), Mouse::getPositionY() - font->getHeight(), Color::BLACK);

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
	map->updatePrecipitables();
	ingameTime += delta;

	/*
	// todo spawn dummy enemies
	if(actors.size() < 2 and futil::random_between(0, 100) == 0)
	{
		actors.push_back(new Actor());
	}
	*/

	// trashing out stuff
	vector<Entity*> trash;
	foreach(Entity*, entity, vector<Entity*>, entities)
	{
		Item* entityItem = entityItemMapping[entity];
		const bool isNotItemOrCanAdd = (entityItem == null or inventory->canAdd(entityItem));

		const Physics::Vector distanceVector = player->body->getCenter() - entity->body->getCenter();
		const double distanceLength = distanceVector.length();

		if(distanceLength < 0.1 and isNotItemOrCanAdd)
		{
			trash.push_back(entity);

			if(entityItem != null)
			{
				cout << itemTypeInfo[entityItem->id].name << " eaten" << endl;

				inventory->add(entityItem);
				entityItemMapping.erase(entity);
			}
		}

		else if(distanceLength < 0.8 and isNotItemOrCanAdd)
		{
			const double magnetude = 0.05*(1-1/(1+distanceLength));
			entity->body->applyForceToCenter(distanceVector.unit().scale(magnetude));
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

		if(inGameMenuShowing)
		{
			handleInputOnInGameMenu(event);
			continue;
		}

		if(event.getEventType() == fgeal::Event::TYPE_KEY_RELEASE)
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
				case fgeal::Keyboard::KEY_ESCAPE:
					inGameMenuShowing = true;
					break;

				case fgeal::Keyboard::KEY_I:
					inventoryVisible = !inventoryVisible;
					if(not inventoryVisible)
					{
						inventoryItemHovered = null;
						inventoryItemHoverTime = fgeal::uptime();
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
//				unsigned int mx = (visibleArea.x + event.getEventMouseX())/BLOCK_SIZE;
//				unsigned int my = (visibleArea.y + event.getEventMouseY())/BLOCK_SIZE;
//
//				if(mx < map->grid.capacity() && my < map->grid[0].capacity()) // in case you click outside the map
//					if (map->grid[mx][my] == NULL)
//						map->addBlock(mx, my);
			}
			else if (event.getEventMouseButton() == fgeal::Mouse::BUTTON_LEFT)
			{
				unsigned int mx = (visibleArea.x + event.getEventMouseX())/BLOCK_SIZE;
				unsigned int my = (visibleArea.y + event.getEventMouseY())/BLOCK_SIZE;

				if(mx < 0) mx = 0; // safety
				if(my < 0) my = 0; // safety

				if(inventoryVisible and inventory->isPointWithin(event.getEventMouseX(), event.getEventMouseY()))
				{
					Item* itemOnSlot = inventory->getItemInSlotPointedBy(event.getEventMouseX(), event.getEventMouseY());
					remove_element(inventory->container->items, itemOnSlot);

					if(cursorHeldItem != null)
					{
						inventory->add(cursorHeldItem);
					}

					cursorHeldItem = itemOnSlot;
				}

				// if you're not clicking outside the map
				else if(mx < map->grid.capacity() and my < map->grid[0].capacity())
				{
					Block* block = map->grid[mx][my];
					if(cursorHeldItem != null)
					{
						if(cursorHeldItem->type.isPlaceable and block == null)  // if placing a placeable item
						{
							bool didPlaceIt = false;

							if(isBlockTypeIdExistent(cursorHeldItem->type.placedBlockTypeId))
							{
								map->addBlock(mx, my, cursorHeldItem->type.placedBlockTypeId);
								didPlaceIt = true;
							}

							if(didPlaceIt)
							{
								if(cursorHeldItem->amount > 1)
									cursorHeldItem->amount--;
								else
								{
									delete cursorHeldItem;
									cursorHeldItem = null;
								}
							}
						}
						else if(cursorHeldItem->type.isDiggingTool and block != null)  // if using a digging tool
						{
							Item* item = null;

							if(isBlockTypeIdExistent(block->typeID) and blockTypeInfo[block->typeID].pickaxeMinerable)
							{
								int detatchedItemTypeId = blockTypeInfo[block->typeID].detatchedItemTypeId;
								if(isBlockTypeIdExistent(detatchedItemTypeId))
									item = new Item(itemTypeInfo[detatchedItemTypeId]);

								map->deleteBlock(mx, my);
							}

							if(item != null)
								this->spawnItemEntity(item, convertToMeters(mx*BLOCK_SIZE), convertToMeters(my*BLOCK_SIZE));
						}
					}
				}
			}
		}

		else if(event.getEventType() == fgeal::Event::TYPE_MOUSE_MOTION)
		{
			if(inventoryVisible and inventory->isPointWithin(event.getEventMouseX(), event.getEventMouseY()))
				inventoryItemHovered = inventory->getItemInSlotPointedBy(event.getEventMouseX(), event.getEventMouseY());
			else
			{
				inventoryItemHovered = null;
				inventoryItemHoverTime = fgeal::uptime();
			}
		}
	}
}

void InGameState::handleInputOnInGameMenu(Event& event)
{
	if(event.getEventType() == fgeal::Event::TYPE_KEY_PRESS)
	{
		switch(event.getEventKeyCode())
		{
			case fgeal::Keyboard::KEY_ARROW_UP:
				inGameMenu->cursorUp();
				break;

			case fgeal::Keyboard::KEY_ARROW_DOWN:
				inGameMenu->cursorDown();
				break;

			case fgeal::Keyboard::KEY_ESCAPE:
				inGameMenuShowing = false;
				break;

			case fgeal::Keyboard::KEY_ENTER:
			{
				TerrariumGame& game = static_cast<TerrariumGame&>(this->game);
				switch(inGameMenu->getSelectedIndex())
				{
					case 0:
						inGameMenuShowing=false;
						break;
					case 1:
						map->saveToFile(stageFilename);
						saveCharacterData();
						game.enterState(TerrariumGame::MAIN_MENU_STATE_ID);
						break;
					case 2:
						game.enterState(TerrariumGame::MAIN_MENU_STATE_ID);
						break;
				}
			}
			break;

			default:break;
		}
	}
}

void InGameState::spawnItemEntity(Item* item, float posx, float posy)
{
	const float physicalBlockSize = Physics::convertToMeters(BLOCK_SIZE);
	Body* detatchedBlockBody = new Body(posx, posy, 0.5*physicalBlockSize, 0.5*physicalBlockSize, Body::Type::DROP);
	Sprite* detatchedBlockSprite = new Sprite(item->type.icon->image, item->type.icon->width, item->type.icon->height, item->type.icon->duration,
												item->type.icon->numberOfFrames, item->type.icon->rawOffsetX, item->type.icon->rawOffsetY, false);  // MUST PASS FALSE!!!
	detatchedBlockSprite->scale = item->type.icon->scale;
	Entity* detatchedBlock = new Entity(new Animation(detatchedBlockSprite), detatchedBlockBody);
	map->world->addBody(detatchedBlockBody);
	detatchedBlockBody->setFixedRotation(false);
	detatchedBlockBody->applyForceToCenter(newVector(0.0f, -playerJumpImpulse*0.5));
	entities.push_back(detatchedBlock);
	entityItemMapping[detatchedBlock] = item;
	cout << "pooped out " << item->type.name << "!" << endl;
}

bool InGameState::isBlockTypeIdExistent(int id)
{
	return id > 0 and id < (int) blockTypeInfo.size();
}

bool InGameState::isBlockTypeIdPassable(int id)
{
	return blockTypeInfo[id].passability == Block::Type::PASSABILITY_FULL;
}

bool InGameState::isItemTypeIdExistant(int id)
{
	return id > 0 and id < (int) itemTypeInfo.size();
}

void InGameState::saveCharacterData()
{
	Properties charProp;
	charProp.put("name", player->label);
	for(unsigned i = 0; i < inventory->container->items.size(); i++)
	{
		Item* item = inventory->container->items[i];
		if(item != null)
		{
			const string baseKey = string("item") + futil::to_string(i);
			charProp.put(baseKey + "_id", futil::to_string(item->type.id));
			charProp.put(baseKey + "_amount", futil::to_string(item->amount));
		}
	}

	charProp.store(characterFilename);
}
