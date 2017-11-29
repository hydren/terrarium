/*
 * physics_box2d.hpp
 *
 *  Created on: 28 de nov de 2017
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_BOX2D_HPP_
#define PHYSICS_BOX2D_HPP_
#include <ciso646>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implementation using Box2D library

#include <Box2D/Box2D.h>

struct Box2DPhysicsBodyImpl
{
	b2Body* body;
	b2BodyDef* bodyDef;
	float* tmpPos;
};

struct Box2DPhysicsWorldImpl
{
	b2World* b2world;
};

typedef Box2DPhysicsBodyImpl BodyImpl;
typedef Box2DPhysicsWorldImpl WorldImpl;

#endif /* PHYSICS_BOX2D_HPP_ */
