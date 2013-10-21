/*
 * body.hpp
 *
 *  Created on: 24/04/2013
 *      Author: carlosfaruolo
 */

#ifndef BODY_HPP_
#define BODY_HPP_

#include <Box2D/Box2D.h>
#include "util.hpp"

class Body
{
	private:
	b2BodyDef bodyDef;
	b2PolygonShape polygon;
	b2ChainShape chain;
	b2FixtureDef fixtureDef;

	public:
	b2Body *body;

	public:
	Body(double x, double y, double size);
	Body(double x, double y, double width, double height);
	~Body(); //TODO

	double getX() const;
	double getY() const;
	double getWidth() const;
	double getHeight() const;
	b2Vec2 getVelocity();
	void setX(double new_x);
	void setY(double new_y);
	void addBodytoWorld( b2World* w);
	void destroyBody( b2World* w );
	void setDynamic();

};

#endif /* BODY_HPP_ */
