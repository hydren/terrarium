#include "actor.hpp"

//Work in progress

Actor::Actor(Texture* tex, int pos_x, int pos_y, int width, int height) :
AnimatedEntity(tex, width, height),
bounds(pos_x,pos_y,width,height),
x_velocity(0),
y_velocity(0),
x_accel(0),
y_accel(0),
isCenterAligned(true)
{}

Actor::Actor(Texture* tex, int pos_x, int y_pos, int width, int height, int sprite_w, int sprite_h) :
AnimatedEntity(tex, sprite_w, sprite_h),
bounds(pos_x, y_pos, width, height),
x_velocity(0),
y_velocity(0),
x_accel(0),
y_accel(0),
isCenterAligned(true)
{}

Actor::~Actor()
{
}

void Actor::draw()
{
	//Simple draw from up-left
	
	int x, y;
	//align horizontally if sprite width differ from the bounding box width
    x = (int) bounds.x + ((int)bounds.w - spriteWidth)/2;
	//if set true, align vertically if sprite height differ from the bounding box height
    if(isCenterAligned) y = (int)bounds.y + ((int)bounds.h - spriteHeight)/2;
    else y = (int)bounds.y + ((int)bounds.h - spriteHeight);//if set false, draws vertically in the bottom
    texture->render(x, y, spriteWidth * animation.timeBasedIndex(), spriteHeight * animation.current() , spriteWidth, spriteHeight);
	
	 /*
	  * this version may be faster, but is less clean to understand:
	textura.render(
	bounds.x + (bounds.w - sprite_width)/2, 
	centerAlign? y = bounds.y + (bounds.h - sprite_height)/2 : y = bounds.y + (bounds.h - sprite_height), 
	sprite_width * animation.timeBasedIndex(), sprite_height * animation.current() , sprite_width, sprite_height);
	 */
}

void Actor::draw(Rectangle& visibleArea)
{
    int x, y;
    //align horizontally if sprite width differ from the bounding box width
    x = (int)bounds.x + ((int)bounds.w - spriteWidth)/2;
    //if set true, align vertically if sprite height differ from the bounding box height
    if(isCenterAligned) y = (int)bounds.y + ((int)bounds.h - spriteHeight)/2;
    else y = (int)bounds.y + ((int)bounds.h - spriteHeight);//if set false, draws vertically in the bottom

    x -= visibleArea.x;
    y -= visibleArea.y;

    texture->render(x, y, spriteWidth * animation.timeBasedIndex(), spriteHeight * animation.current() , spriteWidth, spriteHeight);
}

void Actor::moveTo(int posX, int posY)
{
	bounds.x = posX;
	bounds.y = posY;
}

