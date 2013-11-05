/*
 * entity.cpp
 *
 *  Created on: 11/05/2013
 *      Author: felipe
 */

#include "../header/entity.hpp"

Entity::Entity(AnimationSet* anim, Body *b, Rect* visibleArea) :
animation(anim),
body(b),
visibleAreaPosition(visibleArea)
{}


void Entity::draw()
{
	if(visibleAreaPosition == NULL)
		animation->draw(convertToPixels(body->getX()), convertToPixels(body->getY()));

	else
		animation->draw(convertToPixels(body->getX()) - visibleAreaPosition->x, convertToPixels(body->getY()) - visibleAreaPosition->y, body->getAngle());
}
