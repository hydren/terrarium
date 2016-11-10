/*
 * entity.cpp
 *
 *  Created on: 11/05/2013
 *      Author: felipe
 */

#include "entity.hpp"

Entity::Entity(AnimationSet* anim, Body *b, Rect* visibleArea) :
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
		animation->draw(Math::convertToPixels(body->getX()), Math::convertToPixels(body->getY()));

	else
		animation->draw(Math::convertToPixels(body->getX()) - visibleAreaPosition->x, Math::convertToPixels(body->getY()) - visibleAreaPosition->y, body->getAngle());
}
