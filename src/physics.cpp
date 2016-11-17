/*
 * physics.cpp
 *
 *  Created on: 28/10/2013
 *      Author: felipe
 */

#include "physics.hpp"

#include <iostream>
using std::cout; using std::endl;

#include <Box2D/Box2D.h>
#include "fgeal.hpp"

namespace Physics
{
	//Implementation using Box2D library

	float convertToMeters(float pixels)
	{
		return 0.01f * pixels;
	}

	float convertToPixels(float meters)
	{
		return 100.0f * meters;
	}

	struct Body::Implementation
	{
		b2Body* body;
		b2BodyDef* bodyDef;
		float width, height;
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

	/**
	 * Everything must be passed in meters.
	 * Constructor used for creating non Block bodies.
	 */
	Body::Body(double x, double y, double width, double height, bool isDynamic)
	{
		implementation = new Implementation;

		b2BodyDef* def = new b2BodyDef;
		def->position.Set(x+(width/2.0f), y+(height/2.0f));
		if(isDynamic) def->type = b2_dynamicBody;
		implementation->bodyDef = def;

		b2FixtureDef* fdef = new b2FixtureDef;
		b2PolygonShape* polygon = new b2PolygonShape;
		polygon->SetAsBox((width)/2.0f, (height)/2.0f);
		fdef->shape = polygon;
		implementation->bodyDef->userData = fdef;

		implementation->width = width;
		implementation->height = height;
	}

	//Constructor used by Block class to create a edge chain. Used on map creation.
	Body::Body(double x, double y, double size, bool ignoreCollisions)
	{
		implementation = new Implementation;

		b2BodyDef* def = new b2BodyDef;
		def->position.Set(x+(size/2.0f), y+(size/2.0f));
		implementation->bodyDef = def;

		b2Vec2 vs[4];
		vs[0].Set(-size/2.0f, -size/2.0f);
		vs[1].Set(size/2.0f, -size/2.0f);
		vs[2].Set(size/2.0f, size/2.0f);
		vs[3].Set(-size/2.0f, size/2.0f);
		b2ChainShape* chain = new b2ChainShape;
		chain->CreateLoop(vs, 4);
		b2FixtureDef* fdef = new b2FixtureDef;
		fdef->shape = chain;
		if(ignoreCollisions) //makes this body unable to collide with any other body
			fdef->filter.maskBits = 0x0000;
		implementation->bodyDef->userData = fdef;

		implementation->width = implementation->height = size;
	}


	Body::~Body()
	{
		// XXX Experimental
//		delete implementation->body;
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
		return implementation->body==null? Vector(0,0) :  Vector(implementation->body->GetPosition().x - implementation->width/2, implementation->body->GetPosition().y - implementation->height/2);
	}

	double Body::getWidth() const
	{
		return implementation->width;
	}

	double Body::getHeight() const
	{
		return implementation->height;
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

 	void World::addBody(Body* b)
 	{
 		((b2FixtureDef*) b->implementation->bodyDef->userData)->density = 0.1f; //TODO remove this statement, it is in the wrong place
 		((b2FixtureDef*) b->implementation->bodyDef->userData)->friction = 0.5f; //TODO remove this statement, it is in the wrong place

 		b->implementation->body = this->implementation->b2world->CreateBody(b->implementation->bodyDef);
 		b->implementation->body->CreateFixture((b2FixtureDef*) b->implementation->bodyDef->userData);
 		delete ((b2FixtureDef*) b->implementation->bodyDef->userData); b->implementation->bodyDef->userData = null;
 		delete b->implementation->bodyDef; b->implementation->bodyDef = null;
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
