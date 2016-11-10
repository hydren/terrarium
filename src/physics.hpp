/*
 * physics.hpp
 *
 *  Created on: 28/10/2013
 *      Author: felipe
 */

#ifndef PHYSICS_HPP_
#define PHYSICS_HPP_

namespace Physics
{
	//pre-declaring these classes to avoid annoying declaration order issues...
	class World;

	/** A simple 3D vector */
	struct Vector
	{
		float x, y, z;

		Vector();
		Vector(float x1=0, float y1=0, float z1=0);
		Vector(const Vector &v);
	};

	/** Represents a physical body */
	class Body
	{
		friend class World;

		struct Implementation;
		Implementation* implementation;


		public:

		Body(double x, double y, double size, bool ignoreCollisions=false);
		Body(double x, double y, double width, double height, bool isDynamic=true);
		~Body(); //TODO

		bool isDetached();
		double getX() const;
		double getY() const;
		double getWidth() const;
		double getHeight() const;
		Vector getPosition() const;
		Vector getVelocity() const;
		float getAngle() const;

		void setFixedRotation(bool choice=true);

		void applyImpulse(const Vector& impulse, const Vector& point);
		void applyForceToCenter(const Vector& force);
		void move(const Vector& displacement);
		void rotate(const double& angle);
		void transform(const Vector& displacement, const double& angle);
	};

	/** Represents the world */
	class World
	{
		struct Implementation;
		Implementation* implementation;

		public:

		World(Vector gravityAcceleration);
		~World();

		void addBody(Body* b);
		void destroyBody(Body* b);

		void step(float timeStep, int velocityIterations, int positionIterations);
	};

}


#endif /* PHYSICS_HPP_ */
