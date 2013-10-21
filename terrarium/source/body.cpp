/*
 * body.cpp
 *
 *  Created on: 28/04/2013
 *      Author: felipe
 */

#include "../header/body.hpp"

/**
 * Everything must be passed in meters.
 * Constructor used for creating non Block bodies.
 */
Body::Body(double x, double y, double width, double height)
: bodyDef(), polygon(), chain(), fixtureDef(), body(NULL)
{
	bodyDef.position.Set(x+(width/2.0f), y+(height/2.0f));

	polygon.SetAsBox((width)/2.0f, (height)/2.0f);

	fixtureDef.shape = &polygon;
}

//Constructor used by Block class to create a edge chain. Used on map creation.
Body::Body(double x, double y, double size):
bodyDef(), polygon(), chain(), fixtureDef(), body(NULL)
{
	bodyDef.position.Set(x+(size/2.0f), y+(size/2.0f));

	b2Vec2 vs[4];

	vs[0].Set(-size/2.0f, -size/2.0f);
	vs[1].Set(size/2.0f, -size/2.0f);
	vs[2].Set(size/2.0f, size/2.0f);
	vs[3].Set(-size/2.0f, size/2.0f);

	chain.CreateLoop(vs, 4);

	fixtureDef.shape = &chain;
}

Body::~Body()
{
	//TODO
}

double Body::getX() const
{
	return body->GetPosition().x;
}

double Body::getY() const
{
	return body->GetPosition().y;
}

double Body::getWidth() const
{
	return 0; //TODO
}

double Body::getHeight() const
{
	return 0; //TODO

}

void Body::setX(double new_x)
{
	this->bodyDef.position.x = new_x;
}

void Body::setY(double new_y)
{
	this->bodyDef.position.y = new_y;
}

void Body::addBodytoWorld(b2World* w)
{
	fixtureDef.density = 0.1f;
	fixtureDef.friction = 0.5f;

	body = w->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
}

void Body::destroyBody(b2World* w)
{
	w->DestroyBody(body);
}

void Body::setDynamic()
{
	bodyDef.type = b2_dynamicBody;
}

b2Vec2 Body::getVelocity()
{
	return this->body->GetLinearVelocity();
}
