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

class Entity
{
	public:
	AnimationSet *animation;
	Body *body;

	public:
	Entity(AnimationSet* anim=NULL, Body *b=NULL);

	void draw();
	void draw(const Rect * visibleAreaPosition);
};


#endif /* ENTITY_HPP_ */
