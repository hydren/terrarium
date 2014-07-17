#ifndef ACTOR_HPP_DECLARED
#define ACTOR_HPP_DECLARED

#include "animated_entity.hpp"
#include "util.hpp"

#include <SDL/SDL.h>
#include <map>

#define GRAVITY_CONSTANT 3
#define MAX_SPEED 16

//Work in progress
class Actor extends public AnimatedEntity
{
	public:
	
	/* Contains the coodinates of this Actor.
	 * The sprites will be draw centered on this coordinates.
	 * This is also used to check for collisions.
	 */ 
	BoundingBox bounds;
	int x_velocity, y_velocity, x_accel, y_accel;
	
	// Control whether this Actor sprite will be also centered in relation to the vertical axis. 
	bool isCenterAligned;
	
	// Constructs an actor with the given texture and sets its box position and dimensions. By default the sprite's dimensions are the same as the box.
	Actor(Texture& tex, int x_pos, int y_pos, int width, int height);
	
	/*	Creates an Actor specifying its position, size and its sprites' size
	 */
	Actor(Texture& tex, int x_pos, int y_pos, int width, int height, int sprite_w, int sprite_h);
	
	virtual ~Actor();
	
	/* Draws this Actor on the screen;
	 * */
	virtual void draw();
	
	/* Changes this Actor's position.
	 * A call to this method is equivalent to:
	 * bounds.x = posX;
	 * bounds.y = poxY;
	 */
	void moveTo(int posX, int posY);

};

#endif // ACTOR_HPP_DECLARED
