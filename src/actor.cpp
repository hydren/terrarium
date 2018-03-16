/*
 * actor.cpp
 *
 *  Created on: 23 de fev de 2018
 *      Author: carlosfaruolo
 */

#include "actor.hpp"

#include <cstdlib>

using fgeal::Vector2D;

Actor::Actor(Animation* anim, Body *body, string label)
: Entity(anim, body),
  label(label),
  currentHp(100)
{}

void Actor::behave(float dt)
{
	if(typeID != 0)
	{
		if(behavior.lastTimeWalked == 0)  // init clock
			behavior.lastTimeWalked = fgeal::uptime();

		else if(body->getVelocity().magnitude() < 1.0)
//		else if(fgeal::uptime() - behavior.lastTimeWalked > 1.0f)
		{
			static bool first = false;
			behavior.lastTimeWalked = fgeal::uptime();
			Vector2D impulse = {not first or rand()%2==0? 0.01f: -0.01f, -0.01f}, actorPosition = { (float) body->getCenterX(), (float) body->getCenterY() };
			body->applyImpulse(impulse, actorPosition);
			first = true;
		}
	}
}
