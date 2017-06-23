/*
 * entity.hpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include "animation.hpp"
#include "physics.hpp"

using Physics::Body;
using fgeal::Rectangle;

struct Entity
{
	Animation *animation;
	Body *body;

	Entity(Animation* anim=null, Body *body=null);
	~Entity();

	void draw(const Rectangle& visibleAreaPosition);
};

#endif /* ENTITY_HPP_ */
