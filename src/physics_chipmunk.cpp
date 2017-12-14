/*
 * physics_chipmunk.cpp
 *
 *  Created on: 13 de dez de 2017
 *      Author: carlosfaruolo
 */

#include "physics.hpp"

#ifdef USING_PHYSICS_CHIPMUNK

#include <vector>
#include <stdexcept>

namespace Physics
{
	// Creates a new b2Vec2 from a Vector
	static cpVect toCpVect(const Vector& v)
	{
		return cpv(v.x, v.y);
	}

	// Creates a new Vector from a b2Vec2
	static Vector toVector(const cpVect& cpv)
	{
		Vector v = {static_cast<float>(cpv.x), static_cast<float>(cpv.y)};
		return v;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Body class

	Body::Body(double x, double y, double width, double height, int type, bool ignoreCollisions)
	{
		if(type == Type::BLOCK)
		{
			if(width != height)
				throw std::logic_error("Body constructor called with BLOCK passed as type but width and height are not equal. ");

			const float size = width;

			cpbody = cpBodyNewStatic();
			cpShape* cpShape = cpBoxShapeNew(cpbody, size, size);
			cpShapeSetFriction(cpShape, 0.5);
			cpBodySetPos(cpbody, cpv(x, y));
		}
		else  // non-block type
		{
			const float mass = width*height*0.1;
			cpbody = cpBodyNew(mass, cpMomentForBox(mass, width, height));
			cpShape* cpShape = cpBoxShapeNew(cpbody, width, height);
			cpShapeSetFriction(cpShape, 0.5);
			cpBodySetPos(cpbody, cpv(x, y));
		}
	}

	void deleteCpShapeCallback(cpBody *body, cpShape* shape, void* data)
	{
		cpShapeFree(shape);
	}

	Body::~Body()
	{
		cpBodyEachShape(cpbody, deleteCpShapeCallback, null);
		cpBodyFree(cpbody);
	}

	bool Body::isDetached()
	{
		return cpBodyIsRogue(cpbody);
	}

	double Body::getX() const
	{
		return getCenterX() - this->getWidth()/2;
	}

	double Body::getY() const
	{
		return getCenterY() - this->getHeight()/2;
	}

	Vector Body::getPosition() const
	{
		return newVector(getX(), getY());
	}

	double Body::getCenterX() const
	{
		return cpBodyGetPos(cpbody).x;
	}

	double Body::getCenterY() const
	{
		return cpBodyGetPos(cpbody).y;
	}

	Vector Body::getCenter() const
	{
		return newVector(getCenterX(), getCenterY());
	}

	void fetchShape(cpBody *body, cpShape* shape, void* data)
	{
		data = shape;
	}

	double Body::getWidth() const
	{
		cpShape* shape = null;
		cpBodyEachShape(cpbody, fetchShape, shape);
		if(shape == null) return 0;
		return cpShapeGetBB(shape).r - cpShapeGetBB(shape).l;
	}

	double Body::getHeight() const
	{
		cpShape* shape = null;
		cpBodyEachShape(cpbody, fetchShape, shape);
		if(shape == null) return 0;
		return cpShapeGetBB(shape).b - cpShapeGetBB(shape).t;
	}

	Vector Body::getVelocity() const
	{
		return toVector(cpBodyGetVel(cpbody));
	}

	float Body::getAngle() const
	{
		return cpBodyGetAngle(cpbody);
	}

	void Body::move(const Vector& displacement)
	{
		cpBodySetPos(cpbody, cpBodyGetPos(cpbody) + toCpVect(displacement));
	}

	void Body::rotate(const double& angle)
	{
		cpBodySetAngle(cpbody, cpBodyGetAngle(cpbody) + angle);
	}

	void Body::transform(const Vector& displacement, const double& angle)
	{
		cpBodySetPos(cpbody, cpBodyGetPos(cpbody) + toCpVect(displacement));
		cpBodySetAngle(cpbody, cpBodyGetAngle(cpbody) + angle);
	}

	bool Body::isRotationFixed() const
	{
		return cpBodyGetAngVelLimit(cpbody) == 0;
	}

	void Body::setFixedRotation(bool choice)
	{
		cpBodySetAngVelLimit(cpbody, 0);
	}

	void Body::applyImpulse(const Vector& impulse, const Vector& point)
	{
		cpBodyApplyImpulse(cpbody, toCpVect(impulse), toCpVect(point));
	}

	void Body::applyForceToCenter(const Vector& force)
	{
		cpBodyApplyForce(cpbody, toCpVect(force), toCpVect(force));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// World class

	World::World(Vector gravity)
	{
		cpspace = cpSpaceNew();
		cpSpaceSetGravity(cpspace, cpv(gravity.x, gravity.y));
	}

	void deleteCpBodyCallback(cpBody *body, void *data)
	{
		cpBodyEachShape(body, deleteCpShapeCallback, data);
		cpBodyFree(body);
	}

	World::~World()
	{
		cpSpaceEachBody(cpspace, deleteCpBodyCallback, null);
		cpSpaceFree(cpspace);
	}

 	void World::addBody(Body* b)
 	{
 		if(not cpBodyIsStatic(b->cpbody))
 			cpSpaceAddBody(cpspace, b->cpbody);
 	}

 	void World::destroyBody(Body* b)
 	{
 		cpSpaceRemoveBody(cpspace, b->cpbody);
 		delete b;
 	}

 	void World::step(float timeStep, int velocityIterations, int positionIterations)
 	{
 		cpSpaceStep(cpspace, timeStep);
 	}
}


#endif  //USING_PHYSICS_CHIPMUNK
