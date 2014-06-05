/*
 * physics.cpp
 *
 *  Created on: 28/10/2013
 *      Author: felipe
 */

#include "../headers/physics.hpp"

#include <Box2D/Box2D.h>

namespace Physics
{
	//Implementation using Box2D library

	struct Body::Implementation
	{
		b2Body* body;
		b2BodyDef bodyDef;
		b2FixtureDef fixtureDef;
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
	Body::Body(double x, double y, double width, double height)
	{
		this->implementation = new Implementation;

		this->implementation->bodyDef.position.Set(x+(width/2.0f), y+(height/2.0f));

		b2PolygonShape* polygon = new b2PolygonShape;
		polygon->SetAsBox((width)/2.0f, (height)/2.0f);
		this->implementation->fixtureDef.shape = polygon;
	}

	//Constructor used by Block class to create a edge chain. Used on map creation.
	Body::Body(double x, double y, double size, bool ignoreCollisions)
	{
		this->implementation = new Implementation;

		this->implementation->bodyDef.position.Set(x+(size/2.0f), y+(size/2.0f));

		b2Vec2 vs[4];

		vs[0].Set(-size/2.0f, -size/2.0f);
		vs[1].Set(size/2.0f, -size/2.0f);
		vs[2].Set(size/2.0f, size/2.0f);
		vs[3].Set(-size/2.0f, size/2.0f);

		b2ChainShape* chain = new b2ChainShape;
		chain->CreateLoop(vs, 4);
		this->implementation->fixtureDef.shape = chain;

		if(ignoreCollisions) //makes this body unable to collide with any other body
			this->implementation->fixtureDef.filter.maskBits = 0x0000;
	}


	Body::~Body()
	{
		//TODO
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
		this->implementation->bodyDef.position.x = new_x;
	}

	void Body::setY(double new_y)
	{
		this->implementation->bodyDef.position.y = new_y;
	}

	void Body::setDynamic()
	{
		this->implementation->bodyDef.type = b2_dynamicBody;
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

 	void World::addBody(Body* b)
 	{
 		b->implementation->fixtureDef.density = 0.1f; //TODO remove this statement, it is in the wrong place
 		b->implementation->fixtureDef.friction = 0.5f; //TODO remove this statement, it is in the wrong place

 		b->implementation->body = this->implementation->b2world->CreateBody(&(b->implementation->bodyDef));
 		b->implementation->body->CreateFixture(&(b->implementation->fixtureDef));
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
