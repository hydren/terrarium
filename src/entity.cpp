/*
 * entity.cpp
 *
 *  Created on: 11/05/2013
 *      Author: felipe
 */

#include "entity.hpp"

Entity::Entity(AnimationSet* anim, Body *b, Rectangle* visibleArea) :
animation(anim),
body(b),
visibleAreaPosition(visibleArea)
{}

Entity::~Entity()
{
	delete animation;
	delete body;
//	delete visibleAreaPosition;
}


void Entity::draw()
{
	if(visibleAreaPosition == NULL)
		animation->draw(Physics::convertToPixels(body->getX()), Physics::convertToPixels(body->getY()));

	else
		animation->draw(Physics::convertToPixels(body->getX()) - visibleAreaPosition->x, Physics::convertToPixels(body->getY()) - visibleAreaPosition->y, body->getAngle());
}
