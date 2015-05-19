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

struct Entity
{
	AnimationSet *animation;
	Body *body;
	Rect* visibleAreaPosition;

	Entity(AnimationSet* anim=NULL, Body *b=NULL, Rect* visibleArea=null);
	~Entity();

	void draw();
	void draw(const Rect * visibleAreaPosition);
};


#endif /* ENTITY_HPP_ */
