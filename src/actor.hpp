/*
 * actor.hpp
 *
 *  Created on: 23 de fev de 2018
 *      Author: carlosfaruolo
 */

#ifndef ACTOR_HPP_
#define ACTOR_HPP_

#include "entity.hpp"
#include "statuses.hpp"

class Actor extends public Entity
{
	public:
	string label;
	vector<Status> statuses;

	Actor(Animation* anim=null, Body *body=null, string label=string());
};

#endif /* ACTOR_HPP_ */
