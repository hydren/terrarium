/*
 * physics_chipmunk.hpp
 *
 *  Created on: 29 de nov de 2017
 *      Author: carlosfaruolo
 */

#ifndef PHYSICS_CHIPMUNK_HPP_
#define PHYSICS_CHIPMUNK_HPP_
#include <ciso646>

#include <chipmunk/chipmunk.h>

struct ChipmunkBodyImpl
{
	cpBody* body;
};

struct ChipmunkWorldImpl
{
	cpSpace* world;
};

#endif /* PHYSICS_CHIPMUNK_HPP_ */
