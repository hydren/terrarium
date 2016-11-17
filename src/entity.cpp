/*
 * entity.cpp
 *
 *  Created on: 11/05/2013
 *      Author: felipe
 */

#include "entity.hpp"

using Physics::convertToPixels;

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
	float offx = 0, offy = 0;
	if(visibleAreaPosition != NULL)
	{
		offx += -visibleAreaPosition->x;
		offy += -visibleAreaPosition->y;
	}

	offx += (convertToPixels(body->getWidth()) - animation->getCurrentWidth())/2;
	offy +=  convertToPixels(body->getHeight()) - animation->getCurrentHeight();

	animation->draw(convertToPixels(body->getX()) + offx, convertToPixels(body->getY()) + offy);
}
