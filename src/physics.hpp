/*
 * physics.hpp
 *
 *  Created on: 28/10/2013
 *      Author: felipe
 */

#ifndef PHYSICS_HPP_
#define PHYSICS_HPP_
#include <physics_box2d.hpp>
#include <ciso646>

#include "fgeal/core.hpp"
#include "fgeal/geometry.hpp"

//#include "physics_chipmunk.hxx"

namespace Physics
{
	typedef fgeal::Vector2D Vector;

	Vector newVector(float x, float y);

	float convertToMeters(float pixels);
	float convertToPixels(float meters);

	//pre-declaring these classes to avoid annoying declaration order issues...
	class World;

	/** Represents a physical body. Note that almost all methods will crash if called when the body is detatched. */
	class Body extends BodyImpl
	{
		friend class World;

		public:

		struct Type
		{
			enum
			{
				BLOCK = 2,  // blocks cannot be moved. blocks can be collided with or not.
				ACTOR = 4,  // actors can be moved. actors can be collided with or not.
				DROP  = 8,  // drops can be moved. drops collide only with blocks.
				BULLET= 16, // bullets can be moved. bullets collides with actors and blocks only. bullets are optimized for high speeds.
			};
		};

		/// General constructor. Everything must be passed in meters.
		Body(double x, double y, double width, double height, int type=Type::ACTOR, bool ignoreCollisions=false);

		~Body();

		/** Returns whether this body is already bound to a world instance. This method is safe to be called even when detatched. */
		bool isDetached();

		double getX() const;
		double getY() const;
		Vector getPosition() const;

		double getCenterX() const;
		double getCenterY() const;
		Vector getCenter() const;

		double getWidth() const;
		double getHeight() const;

		Vector getVelocity() const;
		float getAngle() const;

		bool isRotationFixed() const;
		void setFixedRotation(bool choice=true);

		void applyImpulse(const Vector& impulse, const Vector& point);
		void applyForceToCenter(const Vector& force);
		void move(const Vector& displacement);
		void rotate(const double& angle);
		void transform(const Vector& displacement, const double& angle);
	};

	/** Represents the world */
	class World extends WorldImpl
	{
		public:

		World(Vector gravityAcceleration);
		~World();

		void addBody(Body* b);
		void destroyBody(Body* b);

		void step(float timeStep, int velocityIterations, int positionIterations);
	};
}

#endif /* PHYSICS_HPP_ */
