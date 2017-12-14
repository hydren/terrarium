/*
 * physics_chipmunk.hpp
 *
 *  Created on: 29 de nov de 2017
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_CHIPMUNK_HPP_
#define PHYSICS_CHIPMUNK_HPP_
#include <ciso646>

#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Implementation using Box2D library

#include <chipmunk/chipmunk.h>

struct ChipmunkBodyImpl
{
	cpBody* cpbody;
};

struct ChipmunkWorldImpl
{
	cpSpace* cpspace;
};

#define BodyImpl ChipmunkBodyImpl
#define WorldImpl ChipmunkWorldImpl
#define USING_PHYSICS_CHIPMUNK

#endif /* PHYSICS_CHIPMUNK_HPP_ */
