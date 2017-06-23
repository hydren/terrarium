/*
 * entity.cpp
 *
 *  Created on: 11/05/2013
 *      Author: felipe
 */

#include "entity.hpp"

using Physics::convertToPixels;
using Physics::Body;
using fgeal::Rectangle;

Entity::Entity(Animation* anim, Body *b) :
animation(anim),
body(b)
{}

Entity::~Entity()
{
	delete animation;
	delete body;
}

void Entity::draw(const Rectangle& visibleArea)
{
	float offx = 0, offy = 0;
	offx += -visibleArea.x;
	offy += -visibleArea.y;

	offx += (convertToPixels(body->getWidth()) - animation->current().width)/2;
	offy +=  convertToPixels(body->getHeight()) - animation->current().height;

	animation->draw(convertToPixels(body->getX()) + offx, convertToPixels(body->getY()) + offy);
}
