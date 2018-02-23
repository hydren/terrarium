/*
 * actor.cpp
 *
 *  Created on: 23 de fev de 2018
 *      Author: carlosfaruolo
 */

#include "actor.hpp"

Actor::Actor(Animation* anim, Body *body, string label)
: Entity(anim, body),
  label(label),
  statuses()
{}
