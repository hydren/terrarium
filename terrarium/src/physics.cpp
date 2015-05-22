/*
 * physics.cpp
 *
 *  Created on: 28/10/2013
 *      Author: felipe
 */

#include "physics.hpp"

#include <Box2D/Box2D.h>
#include "util.hpp"

namespace Physics
{
	//Implementation using Box2D library

	struct Body::Implementation
	{
		b2Body* body;
	};

	struct World::Implementation
	{
		b2World* b2world;
	};

	/////////////////////////////////////////

	Vector::Vector(float x1, float y1, float z1)
	: x(x1), y(y1), z(z1)
	{}

	Vector::Vector(const Vector& v)
	: x(v.x), y(v.y), z(v.z)
	{}

	/** Creates a new Vector from a b2Vec2 */
	Vector toVector(const b2Vec2& b2v)
	{
		return Vector(b2v.x, b2v.y);
	}

	/** Creates a new b2Vec2 from a Vector */
	b2Vec2 toB2Vec2(const Vector& v)
	{
		return b2Vec2(v.x, v.y);
	}

	/** Creates a detached body instance. To use it, attach to a world instance.
	 * Until the body stays detached, it cannot be moved, rotated, etc, or any other get operation. */
	Body::Body()
	{
		implementation = new Implementation;
	}

	Body::~Body()
	{
		delete implementation;
	}

	/** Returns whether this body is already bound to a world instance. */
	bool Body::isDetached()
	{
		return (implementation->body == null);
	}

	double Body::getX() const
	{
		return (implementation->body==null? 0 : implementation->body->GetPosition().x);
	}

	double Body::getY() const
	{
		return (implementation->body==null? 0 : implementation->body->GetPosition().y);
	}

	Vector Body::getPosition() const
	{
		return implementation->body==null? Vector(0,0) : toVector(implementation->body->GetPosition());
	}

	double Body::getWidth() const
	{
		return 0; //TODO FIXME
	}

	double Body::getHeight() const
	{
		return 0; //TODO FIXME
	}

	Vector Body::getVelocity() const
	{
		return toVector(implementation->body->GetLinearVelocity());
	}

	float Body::getAngle() const
	{
		return implementation->body->GetAngle();
	}

	void Body::move(const Vector& displacement)
	{
		implementation->body->SetTransform(toB2Vec2(displacement), implementation->body->GetAngle());
	}

	void Body::rotate(const double& angle)
	{
		implementation->body->SetTransform(implementation->body->GetPosition(), angle);
	}

	void Body::transform(const Vector& displacement, const double& angle)
	{
		implementation->body->SetTransform(toB2Vec2(displacement), angle);
	}

	void Body::setFixedRotation(bool choice)
	{
		implementation->body->SetFixedRotation(choice);
	}

	void Body::applyImpulse(const Vector& impulse, const Vector& point)
	{
		implementation->body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(point.x, point.y), true);
	}

	void Body::applyForceToCenter(const Vector& force)
	{
		implementation->body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
	}

	World::World(Vector gravity)
	{
		implementation = new Implementation;
		implementation->b2world = new b2World(b2Vec2(gravity.x, gravity.y));
	}

	World::~World()
	{
		//XXX Experimental
		cout << "world destructor..." << endl;
		for ( b2Body* b = implementation->b2world->GetBodyList(); b; b = b->GetNext())
		{
			cout << "destroying b2Body at " << ((long)b) << endl;
			implementation->b2world->DestroyBody(b);
		}
		delete implementation->b2world;
		delete implementation;
	}

	/**
	 * Everything must be passed in meters.
	 * Constructor used for creating non Block bodies.
	 */
 	void World::addBody(Body* b, double x, double y, double width, double height, bool isDynamic)
 	{
 		b2BodyDef bdef;
		bdef.position.Set(x+(width/2.0f), y+(height/2.0f));
		if(isDynamic) bdef.type = b2_dynamicBody;

		b2PolygonShape polygon;
		polygon.SetAsBox((width)/2.0f, (height)/2.0f);
		b2FixtureDef fdef;
		fdef.shape = &polygon;
 		fdef.density = 0.1f;
 		fdef.friction = 0.5f;

 		b->implementation->body = this->implementation->b2world->CreateBody(&bdef);
 		b->implementation->body->CreateFixture(&fdef);
 	}

 	//Constructor used by Block class to create a edge chain. Used on map creation. Everything must be passed in meters.
 	void World::addBody(Body* b, double x, double y, double size, bool ignoreCollisions)
	{
 		b2BodyDef bdef;
		bdef.position.Set(x+(size/2.0f), y+(size/2.0f));

		b2Vec2 vs[4];
		vs[0].Set(-size/2.0f, -size/2.0f);
		vs[1].Set(size/2.0f, -size/2.0f);
		vs[2].Set(size/2.0f, size/2.0f);
		vs[3].Set(-size/2.0f, size/2.0f);
		b2ChainShape chain;
		chain.CreateLoop(vs, 4);
		b2FixtureDef fdef;
		fdef.shape = &chain;
		if(ignoreCollisions) //makes this body unable to collide with any other body
			fdef.filter.maskBits = 0x0000;

		fdef.density = 0.1f;
		fdef.friction = 0.5f;

		b->implementation->body = this->implementation->b2world->CreateBody(&bdef);
		b->implementation->body->CreateFixture(&fdef);
	}

 	void World::destroyBody(Body* b)
 	{
 		implementation->b2world->DestroyBody(b->implementation->body);
 	}

 	void World::step(float timeStep, int velocityIterations, int positionIterations)
 	{
 		implementation->b2world->Step(timeStep, velocityIterations, positionIterations);
 	}

}
