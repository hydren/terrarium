/*
 * entity.hpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include "animation.hpp"

class Entity
{
	Animation animation;

	virtual ~Entity();

	public:
	Entity(Animation animation) :
	animation(animation)
	{}

	virtual void draw();
};


#endif /* ENTITY_HPP_ */
