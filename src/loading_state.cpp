/*
 * loading_state.cpp
 *
 *  Created on: 29 de jun de 2017
 *      Author: carlosfaruolo
 */

#include "loading_state.hpp"

using fgeal::Image;
using fgeal::Display;

LoadingState::LoadingState(TerrariumGame* game)
: State(*game), imgSplash(null), paintedSplash(false), loadable(null)
{}

LoadingState::~LoadingState()
{
	delete imgSplash;
}

void LoadingState::reset(Loadable* loadable)
{
	paintedSplash = false;
	this->loadable = loadable;
}

void LoadingState::initialize()
{
	imgSplash = new Image("./resources/loading.png");
}

void LoadingState::onEnter() {}

void LoadingState::onLeave() {}

void LoadingState::render()
{
	Display& display = Display::getInstance();

	display.clear();
	imgSplash->drawScaled(0, 0, display.getWidth()/imgSplash->getWidth(), display.getHeight()/imgSplash->getHeight());
}

void LoadingState::update(float delta)
{
	if(not paintedSplash)
		paintedSplash = true;
	else if(loadable != null)
		loadable->loadDuringLoadingScreen();
}
