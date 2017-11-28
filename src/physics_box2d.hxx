/*
 * physics_box2d.hxx
 *
 *  Created on: 28 de nov de 2017
 *      Author: carlosfaruolo
 */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implementation using Box2D library

#include <Box2D/Box2D.h>

#define library_specific_body_class_fields_definition()\
		b2Body* body;\
		b2BodyDef* bodyDef;\
		float* tmpPos

#define library_specific_world_class_fields_definition()\
		b2World* b2world
