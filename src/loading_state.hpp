/*
 * loading_state.hpp
 *
 *  Created on: 29 de jun de 2017
 *      Author: carlosfaruolo
 */

#ifndef LOADING_STATE_HPP_
#define LOADING_STATE_HPP_
#include <ciso646>

#include "terrarium_game.hpp"

#include "fgeal/fgeal.hpp"
#include "fgeal/extra/game.hpp"

class LoadingState extends public fgeal::Game::State
{
	public:
	struct Loadable
	{
		virtual void loadDuringLoadingScreen() abstract;
	};

	private:
	fgeal::Image* imgSplash;

	bool paintedSplash;
	Loadable* loadable;

	public:
	void reset(Loadable* loadable);

	int getId() { return TerrariumGame::LOADING_STATE_ID; }

	LoadingState(TerrariumGame* game);
	~LoadingState();

	void initialize();
	void onEnter();
	void onLeave();

	void render();
	void update(float delta);
};

#endif /* LOADING_STATE_HPP_ */
