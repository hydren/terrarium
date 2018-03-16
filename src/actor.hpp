/*
 * actor.hpp
 *
 *  Created on: 23 de fev de 2018
 *      Author: carlosfaruolo
 */

#ifndef ACTOR_HPP_
#define ACTOR_HPP_

#include "entity.hpp"
#include "inventory.hpp"

class Actor extends public Entity
{
	public:
	std::string label;

	// id of the actor type
	int typeID;

	// todo parametrize these fields
	unsigned currentHp, maxHp;

	Item* containerItem, *equipHead, *equipTorso, *equipLegs, *equipArms;

	struct Type
	{
		unsigned id;
		std::string name, description, faction;
		unsigned maxHp, contactDamageFactor;
		Animation* animation;

		// todo move behavior class to here
	};

	struct Behavior
	{
		enum Type
		{
			TYPE_DUMMY_STAND,
			TYPE_DUMMY_WALK,
			/* ... */
		};

		Type type;

		// behavior vars
		long lastTimeWalked;
	}
	behavior;

	Actor(Animation* anim=null, Body *body=null, string label=string());

	void behave(float timestep);
};

#endif /* ACTOR_HPP_ */
