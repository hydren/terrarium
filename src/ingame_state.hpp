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
#include "map.hpp"
#include "inventory.hpp"

class InGameState extends public fgeal::Game::State
{
	friend class Map;

	bool wasInit;

	// ==== assets

	// Default is a black background
	Sprite* background;

	fgeal::Font* font;

	// block icons
	Sprite* iconBlockDirt, *iconBlockStone;

	// vector to keep track of images to delete them later
	std::vector<fgeal::Image*> images;

	StackedSingleSheetAnimation* tilesetDirt, *tilesetStone, *tilesetWater, *tilesetGrass;

	// vector to record track of tileset animations to delete them later
	std::vector<StackedSingleSheetAnimation*> tilesets;

	// ==== entities

	std::vector<Entity*> entities;

	// the player, a separate entity
	Entity* player;

	// ==== logic

	Map* map;

	fgeal::Menu* inGameMenu;

	fgeal::Rectangle visibleArea;

	bool jumping, inGameMenuShowing;

	float playerJumpImpulse, playerWalkForce;

	Inventory* inventory;
	bool inventoryVisible;

	std::map<Entity*, Item*> entityItemMapping;

	int getId() { return TerrariumGame::INGAME_STATE_ID; }

	void handleInput();

	void spawnItemEntity(Item* type, float posx, float posy);

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
