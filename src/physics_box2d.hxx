/*
 * physics_box2d.hxx
 *
 *  Created on: 28 de nov de 2017
 *      Author: carlosfaruolo
 */

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
