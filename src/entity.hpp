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
	AnimationSet *animation;
	Body *body;
	Rectangle* visibleAreaPosition;

	Entity(AnimationSet* anim=NULL, Body *b=NULL, Rectangle* visibleArea=null);
	~Entity();

	void draw();
	void draw(const Rectangle * visibleAreaPosition);
};


#endif /* ENTITY_HPP_ */
