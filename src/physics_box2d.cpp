/*
 * physics_box2d.cpp
 *
 *  Created on: 28 de nov de 2017
 *      Author: carlosfaruolo
 */

#include "physics.hpp"

#ifdef USING_PHYSICS_BOX2D

#include "futil/language.hpp"

#include <vector>
#include <stdexcept>

using std::vector;

namespace Physics
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Static file scope functions

	// Get the fixture's dimension. If which is false, the width is returned. Otherwise it returns the height.
	static float getDimension(b2FixtureDef* fdef, bool which=false)
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

	// Returns a cached b2BodyDef for Block-type bodies.
	static b2BodyDef* getCachedBlockDef(float size, bool isPassable)
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

	// Returns true if the given b2BodyDef is cached (block-type body defs cache).
	static bool isBodyDefCached(b2BodyDef* def)
	{
		for(unsigned i = 0; i < cachedBodyDefs.size(); i++)
			if(def == cachedBodyDefs[i])
				return true;

		return false;
	}

	// Creates a new Vector from a b2Vec2
	static Vector toVector(const b2Vec2& b2v)
	{
		Vector v = {b2v.x, b2v.y};
		return v;
	}

	// Creates a new b2Vec2 from a Vector
	static b2Vec2 toB2Vec2(const Vector& v)
	{
		return b2Vec2(v.x, v.y);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Body class

	Body::Body(double x, double y, double width, double height, int type, bool ignoreCollisions)
	{
		// signal that this block wasn't attatched to any world yet
		body = null;

		if(type == Type::BLOCK)
		{
			if(width != height)
				throw std::logic_error("Body constructor called with BLOCK passed as type but width and height are not equal. ");

			const float size = width;

			// try a cached def
			bodyDef = getCachedBlockDef(size, ignoreCollisions);

			// if there is no cached def with the desired spec, create a def (and cache it)
			if(bodyDef == null)
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

				fdef->filter.categoryBits = type;
				if(ignoreCollisions) //makes this body unable to collide with any other body
					fdef->filter.maskBits = 0x0000;

				bodyDef = def;

				cachedBodyDefs.push_back(def); // put this def on cache as well.
			}

			// since bodyDef is shared, store position temporarily to use it afterwards on World::addBody()
			float* position = new float[2];
			position[0] = x+(size/2.0f);
			position[1] = y+(size/2.0f);
			tmpPos = position;
		}
		else  // non-block type
		{
			b2BodyDef* def = new b2BodyDef;
			def->position.Set(x+(width/2.0f), y+(height/2.0f));
			def->type = b2_dynamicBody;
			bodyDef = def;

			if(type == Type::BULLET)
				def->bullet = true;

			b2FixtureDef* fdef = new b2FixtureDef;
			b2PolygonShape* polygon = new b2PolygonShape;
			polygon->SetAsBox((width)/2.0f, (height)/2.0f);
			fdef->shape = polygon;
			fdef->density = 0.1f;
			fdef->friction = 0.5f;
			fdef->filter.categoryBits = type;

			switch(type)
			{
				case Type::ACTOR:  fdef->filter.maskBits = 1 | Type::BLOCK | Type::BULLET; break;
				case Type::DROP:   fdef->filter.maskBits = 1 | Type::BLOCK; break;
				case Type::BULLET: fdef->filter.maskBits = 1 | Type::BLOCK | Type::ACTOR; break;
			}

			bodyDef->userData = fdef;
			tmpPos = null;
		}
	}

	Body::~Body()
	{}

	bool Body::isDetached()
	{
		return (body == null);
	}

	double Body::getX() const
	{
		return body->GetPosition().x - this->getWidth()/2;
	}

	double Body::getY() const
	{
		return body->GetPosition().y - this->getHeight()/2;
	}

	Vector Body::getPosition() const
	{
		return newVector(body->GetPosition().x - this->getWidth()/2, body->GetPosition().y - this->getHeight()/2);
	}

	double Body::getCenterX() const
	{
		return body->GetPosition().x;
	}

	double Body::getCenterY() const
	{
		return body->GetPosition().y;
	}

	Vector Body::getCenter() const
	{
		return newVector(body->GetPosition().x, body->GetPosition().y);
	}

	double Body::getWidth() const
	{
		return getDimension(static_cast<b2FixtureDef*>(bodyDef->userData), false);
	}

	double Body::getHeight() const
	{
		return getDimension(static_cast<b2FixtureDef*>(bodyDef->userData), true);
	}

	Vector Body::getVelocity() const
	{
		return toVector(body->GetLinearVelocity());
	}

	float Body::getAngle() const
	{
		return body->GetAngle();
	}

	void Body::move(const Vector& displacement)
	{
		body->SetTransform(toB2Vec2(displacement), body->GetAngle());
	}

	void Body::rotate(const double& angle)
	{
		body->SetTransform(body->GetPosition(), angle);
	}

	void Body::transform(const Vector& displacement, const double& angle)
	{
		body->SetTransform(toB2Vec2(displacement), angle);
	}

	bool Body::isRotationFixed() const
	{
		return body->IsFixedRotation();
	}

	void Body::setFixedRotation(bool choice)
	{
		body->SetFixedRotation(choice);
	}

	void Body::applyImpulse(const Vector& impulse, const Vector& point)
	{
		body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(point.x, point.y), true);
	}

	void Body::applyForceToCenter(const Vector& force)
	{
		body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// World class

	World::World(Vector gravity)
	{
		b2world = new b2World(b2Vec2(gravity.x, gravity.y));
	}

	World::~World()
	{
		/* FIXME this destructor does not call Body::~Body, it only destroys b2Body objects directly
		 * This causes some b2BodyDef* and b2FixtureDef* instances to leak.
		 * To solve this, we need to be able to reference the Body instance from the b2Body* within it. */

		b2Body* body = this->b2world->GetBodyList();

		while(body != null)
		{
			b2world->DestroyBody(body);
			body = body->GetNext();
		}

		delete b2world;
	}

 	void World::addBody(Body* b)
 	{
 		if(b->body != null)
 			throw std::runtime_error("error: trying to add a body to a world for a second time!");

 		if(b->tmpPos != null)
		{
			float* pos = b->tmpPos;
			b->bodyDef->position.Set(pos[0], pos[1]);
			delete[] b->tmpPos;
			b->tmpPos = null;
		}

 		b->body = this->b2world->CreateBody(b->bodyDef);
 		b->body->CreateFixture(static_cast<b2FixtureDef*>(b->bodyDef->userData));
 	}

 	void World::destroyBody(Body* b)
 	{
 		if(not isBodyDefCached(b->bodyDef))
 		{
 			delete static_cast<b2FixtureDef*>(b->bodyDef->userData);
			b->bodyDef->userData = null;

			delete b->bodyDef;
 		}
 		b2world->DestroyBody(b->body);
 	}

 	void World::step(float timeStep, int velocityIterations, int positionIterations)
 	{
 		b2world->Step(timeStep, velocityIterations, positionIterations);
 	}
}

#endif //USING_PHYSICS_BOX2D
