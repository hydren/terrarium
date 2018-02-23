/*
 * ingame_state.hpp
 *
 *  Created on: 12 de dez de 2016
 *      Author: carlosfaruolo
 */

#ifndef INGAME_STATE_HPP_
#define INGAME_STATE_HPP_
#include <ciso646>

#include "terrarium_game.hpp"

#include "futil/language.hpp"
#include "futil/string_extra_operators.hpp"

#include "fgeal/fgeal.hpp"

#include "fgeal/extra/game.hpp"
#include "fgeal/extra/menu.hpp"

#include <cstdlib>
#include <vector>
#include <map>

#include "block.hpp"
#include "entity.hpp"
#include "actor.hpp"
#include "map.hpp"
#include "inventory.hpp"

class InGameState extends public fgeal::Game::State
{
	friend class Map;
	friend class TerrariumGameLogic;

	bool wasInit;

	// ==== assets

	// Default is a black background
	Sprite* backgroundDay, *backgroundNight;

	fgeal::Font* font, *fontInventory;

	fgeal::Color inventoryColor, inventoryFontColor;

	// block icons
	Sprite* iconBlockDirt, *iconBlockStone, *iconPickaxeDev;

	// vector to keep track of images to delete them later
	std::vector<fgeal::Image*> images;

	// vector to record track of tileset animations to delete them later
	std::vector<StackedSingleSheetAnimation*> tilesets;

	// spec. of item types
	std::vector<Item::Type> itemTypeInfo;

	// spec. of block types
	std::vector<Block::Type> blockTypeInfo;

	// ==== entities

	std::vector<Entity*> entities;

	// the player, a separate entity
	Actor* player;

	// ==== logic

	std::string stageFilename, characterFilename;
	float playerHp;

	Map* map;

	fgeal::Menu* inGameMenu;

	fgeal::Rectangle visibleArea;

	bool jumping, inGameMenuShowing;

	float playerJumpImpulse, playerWalkForce;

	Inventory* inventory;
	bool inventoryVisible;
	Item* cursorHeldItem;

	std::map<Entity*, Item*> entityItemMapping;

	double ingameTime, hourDuration;

	Item* inventoryItemHovered;
	double inventoryItemHoverTime;

	int getId() { return TerrariumGame::INGAME_STATE_ID; }

	void handleInput();
	void handleInputOnInGameMenu(fgeal::Event&);

	void spawnItemEntity(Item* type, float posx, float posy);

	bool isItemTypeIdExistant(int id);

	void saveCharacterData();

	// block type id info methods
	bool isBlockTypeIdExistent(int id);
	bool isBlockTypeIdPassable(int id);  // todo improve this check to allow a more refined passability

	public:

	InGameState(TerrariumGame* game);
	~InGameState();

	void initialize();
	void onEnter();
	void onLeave();

	void render();
	void update(float delta);
};

#endif /* INGAME_STATE_HPP_ */
