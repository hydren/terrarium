/*
 * actor.hpp
 *
 *  Created on: 23 de fev de 2018
 *      Author: carlosfaruolo
 */

#ifndef ACTOR_HPP_
#define ACTOR_HPP_

#include "entity.hpp"

class Actor extends public Entity
{
	public:
	string label;

	// todo parametrize these fields
	unsigned hp, mp;

	Actor(Animation* anim=null, Body *body=null, string label=string());
};

#endif /* ACTOR_HPP_ */
