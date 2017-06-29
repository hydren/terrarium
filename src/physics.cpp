/*
 * physics.cpp
 *
 *  Created on: 28/10/2013
 *      Author: felipe
 */
#include <ciso646>

#include "physics.hpp"

#include "futil/general/language.hpp"

#include <Box2D/Box2D.h>
#include <vector>
#include <stdexcept>

using std::vector;

namespace Physics
{
	//Implementation using Box2D library

	struct Body::Implementation
	{
		b2Body* body;
		b2BodyDef* bodyDef;
		float* tmpPos;
	};

	struct World::Implementation
	{
		b2World* b2world;
	};

	float convertToMeters(float pixels) { return 0.01f * pixels; }
	float convertToPixels(float meters) { return 100.0f * meters;}

	/// Get the fixture's dimension. If which is false, the width is returned. Otherwise it returns the height.
	float getDimension(b2FixtureDef* fdef, bool which=false)
	{
		if(fdef->shape->m_type == b2Shape::e_chain)
		{
			const b2ChainShape& shape = *static_cast<const b2ChainShape*>(fdef->shape);
			if(which == false)
				return shape.m_vertices[1].x - shape.m_vertices[0].x; // width
			else
				return shape.m_vertices[2].y - shape.m_vertices[1].y; // height
		}
		else if(fdef->shape->m_type == b2Shape::e_polygon)
		{
			const b2PolygonShape& shape = *static_cast<const b2PolygonShape*>(fdef->shape);
			if(which == false)
				return shape.m_vertices[1].x - shape.m_vertices[0].x; // width
			else
				return shape.m_vertices[2].y - shape.m_vertices[1].y; // height
		}
		else throw std::runtime_error("invalid body shape");
	}

	vector<b2BodyDef*> cachedBodyDefs; // this should be static (non-visible outside this source unit) but oddly doing so slightly increases memory usage.

	/// Returns a cached b2BodyDef for Block-type bodies.
	b2BodyDef* getCachedBlockDef(float size, bool isPassable)
	{
		// try a cached def
		for(unsigned i = 0; i < cachedBodyDefs.size(); i++)
		{
			b2FixtureDef& fixture = *static_cast<b2FixtureDef*>(cachedBodyDefs[i]->userData);
			float fixtureWidth = getDimension(&fixture, false), fixtureHeight = getDimension(&fixture, true);

			if(fixtureWidth == size and fixtureHeight == size and (fixture.filter.maskBits == 0x0000)==isPassable)
				return cachedBodyDefs[i];
		}

		return null;
	}

	/// Returns true if the given b2BodyDef is cached (block-type body defs cache).
	bool isBodyDefCached(b2BodyDef* def)
	{
		for(unsigned i = 0; i < cachedBodyDefs.size(); i++)
			if(def == cachedBodyDefs[i])
				return true;

		return false;
	}

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

	Body::Body(double x, double y, double width, double height, int type, bool ignoreCollisions)
	{
		implementation = new Implementation();

		if(type == Type::BLOCK)
		{
			if(width != height)
				throw std::logic_error("Body constructor called with BLOCK passed as type but width and height are not equal. ");

			const float size = width;

			// try a cached def
			implementation->bodyDef = getCachedBlockDef(size, ignoreCollisions);

			// if there is no cached def with the desired spec, create a def (and cache it)
			if(implementation->bodyDef == null)
			{
				b2BodyDef* def = new b2BodyDef;

				b2FixtureDef* fdef = new b2FixtureDef;
				def->userData = fdef;
				def->type = b2_staticBody;

				fdef->density = 0.1f;
				fdef->friction = 0.5f;

				b2Vec2 vs[4];
				vs[0].Set(-size/2.0f,-size/2.0f);
				vs[1].Set( size/2.0f,-size/2.0f);
				vs[2].Set( size/2.0f, size/2.0f);
				vs[3].Set(-size/2.0f, size/2.0f);
				b2ChainShape* chain = new b2ChainShape;
				chain->CreateLoop(vs, 4);
				fdef->shape = chain;

				if(ignoreCollisions) //makes this body unable to collide with any other body
					fdef->filter.maskBits = 0x0000;

				implementation->bodyDef = def;

				cachedBodyDefs.push_back(def); // put this def on cache as well.
			}

			// since bodyDef is shared, store position temporarily to use it afterwards on World::addBody()
			float* position = new float[2];
			position[0] = x+(size/2.0f);
			position[1] = y+(size/2.0f);
			implementation->tmpPos = position;
		}
		else  // non-block type
		{
			b2BodyDef* def = new b2BodyDef;
			def->position.Set(x+(width/2.0f), y+(height/2.0f));
			def->type = b2_dynamicBody;
			implementation->bodyDef = def;

			b2FixtureDef* fdef = new b2FixtureDef;
			b2PolygonShape* polygon = new b2PolygonShape;
			polygon->SetAsBox((width)/2.0f, (height)/2.0f);
			fdef->shape = polygon;
			fdef->density = 0.1f;
			fdef->friction = 0.5f;
			implementation->bodyDef->userData = fdef;

			implementation->tmpPos = null;
		}
	}

	Body::~Body()
	{
		delete implementation;
	}

	bool Body::isDetached()
	{
		return (implementation->body == null);
	}

	double Body::getX() const
	{
		return (implementation->body==null? 0 : implementation->body->GetPosition().x - this->getWidth()/2);
	}

	double Body::getY() const
	{
		return (implementation->body==null? 0 : implementation->body->GetPosition().y - this->getHeight()/2);
	}

	Vector Body::getPosition() const
	{
		return implementation->body==null? Vector(0,0) :  Vector(implementation->body->GetPosition().x - this->getWidth()/2, implementation->body->GetPosition().y - this->getHeight()/2);
	}

	// ******************************* Atatched-only methods (these will crash if called when the body is detatched) **********************************************************************

	double Body::getWidth() const
	{
		return getDimension(static_cast<b2FixtureDef*>(implementation->bodyDef->userData), false);
	}

	double Body::getHeight() const
	{
		return getDimension(static_cast<b2FixtureDef*>(implementation->bodyDef->userData), true);
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

	bool Body::isRotationFixed() const
	{
		return implementation->body->IsFixedRotation();
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
		/* FIXME this destructor does not call Body::~Body, it only destroys b2Body objects directly
		 * This causes some b2BodyDef* and b2FixtureDef* instances to leak.
		 * To solve this, we need to be able to reference the Body instance from the b2Body* within it. */

		b2Body* body = this->implementation->b2world->GetBodyList();

		while(body != null)
		{
			implementation->b2world->DestroyBody(body);
			body = body->GetNext();
		}

		delete implementation->b2world;
		delete implementation;
	}

 	void World::addBody(Body* b)
 	{
 		if(b->implementation->body != null)
 			throw std::runtime_error("error: trying to add a body to a world for a second time!");

 		if(b->implementation->tmpPos != null)
		{
			float* pos = b->implementation->tmpPos;
			b->implementation->bodyDef->position.Set(pos[0], pos[1]);
			delete[] b->implementation->tmpPos;
			b->implementation->tmpPos = null;
		}

 		b->implementation->body = this->implementation->b2world->CreateBody(b->implementation->bodyDef);
 		b->implementation->body->CreateFixture(static_cast<b2FixtureDef*>(b->implementation->bodyDef->userData));
 	}

 	void World::destroyBody(Body* b)
 	{
 		if(not isBodyDefCached(b->implementation->bodyDef))
 		{
 			delete static_cast<b2FixtureDef*>(b->implementation->bodyDef->userData);
			b->implementation->bodyDef->userData = null;

			delete b->implementation->bodyDef;
 		}
 		implementation->b2world->DestroyBody(b->implementation->body);
 	}

 	void World::step(float timeStep, int velocityIterations, int positionIterations)
 	{
 		implementation->b2world->Step(timeStep, velocityIterations, positionIterations);
 	}

}
