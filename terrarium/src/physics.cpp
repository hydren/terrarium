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
		b2BodyDef* bodyDef;
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

	Vector toVector(const b2Vec2 &b2v)
	{
		return Vector(b2v.x, b2v.y);
	}

	/**
	 * Everything must be passed in meters.
	 * Constructor used for creating non Block bodies.
	 */
	Body::Body(double x, double y, double width, double height, bool isDynamic)
	{
		this->implementation = new Implementation;

		b2BodyDef* def = new b2BodyDef;
		def->position.Set(x+(width/2.0f), y+(height/2.0f));
		if(isDynamic) def->type = b2_dynamicBody;
		this->implementation->bodyDef = def;

		b2FixtureDef* fdef = new b2FixtureDef;
		b2PolygonShape* polygon = new b2PolygonShape;
		polygon->SetAsBox((width)/2.0f, (height)/2.0f);
		fdef->shape = polygon;
		this->implementation->bodyDef->userData = fdef;
	}

	//Constructor used by Block class to create a edge chain. Used on map creation.
	Body::Body(double x, double y, double size, bool ignoreCollisions)
	{
		this->implementation = new Implementation;

		b2BodyDef* def = new b2BodyDef;
		def->position.Set(x+(size/2.0f), y+(size/2.0f));
		this->implementation->bodyDef = def;

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
		this->implementation->bodyDef->userData = fdef;
	}


	Body::~Body()
	{
		// XXX Experimental
//		delete implementation->body;
		delete implementation;
	}

	double Body::getX() const
	{
		return this->implementation->body->GetPosition().x;
	}

	double Body::getY() const
	{
		return this->implementation->body->GetPosition().y;
	}

	double Body::getWidth() const
	{
		return 0; //TODO
	}

	double Body::getHeight() const
	{
		return 0; //TODO
	}

	Vector Body::getVelocity() const
	{
		return toVector(this->implementation->body->GetLinearVelocity());
	}

	float Body::getAngle() const
	{
		return this->implementation->body->GetAngle();
	}

	void Body::setX(double new_x)
	{
		//FIXME It should be a SetTransform on the body
		this->implementation->bodyDef->position.x = new_x;
	}

	void Body::setY(double new_y)
	{
		//FIXME It should be a SetTransform on the body
		this->implementation->bodyDef->position.y = new_y;
	}

	void Body::setFixedRotation(bool choice)
	{
		this->implementation->body->SetFixedRotation(choice);
	}

	void Body::applyImpulse(Vector impulse, Vector point)
	{
		this->implementation->body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(point.x, point.y), true);
	}

	void Body::applyForceToCenter(Vector force)
	{
		this->implementation->body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
	}

	World::World(Vector gravity)
	{
		this->implementation = new Implementation;
		this->implementation->b2world = new b2World(b2Vec2(gravity.x, gravity.y));
	}

	World::~World()
	{
		//XXX Experimental
		delete implementation->b2world;
		delete implementation;
	}

 	void World::addBody(Body* b)
 	{
 		((b2FixtureDef*) b->implementation->bodyDef->userData)->density = 0.1f; //TODO remove this statement, it is in the wrong place
 		((b2FixtureDef*) b->implementation->bodyDef->userData)->friction = 0.5f; //TODO remove this statement, it is in the wrong place

 		b->implementation->body = this->implementation->b2world->CreateBody(b->implementation->bodyDef);
 		b->implementation->body->CreateFixture((b2FixtureDef*) b->implementation->bodyDef->userData);
 		delete ((b2FixtureDef*) b->implementation->bodyDef->userData);
 		delete b->implementation->bodyDef;
 	}

 	void World::destroyBody(Body* b)
 	{
 		this->implementation->b2world->DestroyBody(b->implementation->body);
 	}

 	void World::step(float timeStep, int velocityIterations, int positionIterations)
 	{
 		this->implementation->b2world->Step(timeStep, velocityIterations, positionIterations);
 	}

}
