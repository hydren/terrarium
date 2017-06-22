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

#include "futil/general/language.hpp"
#include "futil/string/more_operators.hpp"

#include "fgeal/fgeal.hpp"
using fgeal::Rectangle;
using fgeal::Image;

#include "fgeal/extra/game.hpp"
#include "fgeal/extra/menu.hpp"

#include <cstdlib>
#include <vector>
using std::vector;

#include "block.hpp"
#include "entity.hpp"
#include "map.hpp"

using fgeal::Font;
using fgeal::Menu;

class InGameState extends public fgeal::Game::State
{
	public:
	bool wasInit;

	vector<Entity*> entities;
	vector<Image*> images;

	Image* tilesetDirt, *tilesetStone, *tilesetWater, *tilesetGrass;

	Rectangle visibleArea;

	Image* bg;
	Map* game_map;

	Entity* player;
	StackedSingleSheetAnimation* playerAnimation;

	Font* font;
	Menu* inGameMenu;

	bool jumping, inGameMenuShowing;
	bool isKeyUpPressed, isKeyDownPressed, isKeyRightPressed, isKeyLeftPressed;

	float playerJumpImpulse, playerWalkForce;

	int getId() { return TerrariumGame::INGAME_STATE_ID; }

	void handleInput();

	InGameState(TerrariumGame* game);
	~InGameState();

	void initialize();
	void onEnter();
	void onLeave();

	void render();
	void update(float delta);
};

#endif /* INGAME_STATE_HPP_ */
