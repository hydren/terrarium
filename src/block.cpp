/*
 * block.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: felipe
 */

#include "block.hpp"

using Physics::convertToPixels;

/** Caution: the given animation should be correctly configured for blocks */
Block::Block(AnimationSet* anim, int x_grid, int y_grid, int typeID, bool ignoreCollision):
Entity(anim, new Body(Physics::convertToMeters(x_grid*BLOCK_SIZE), Physics::convertToMeters(y_grid*BLOCK_SIZE), Physics::convertToMeters(BLOCK_SIZE), ignoreCollision)),
x_grid_pos(x_grid),
y_grid_pos(y_grid),
typeID(typeID)
{
	animation->setCurrent("full_free");
}

int Block::getX() {
	return x_grid_pos;
}

int Block::getY() {
	return y_grid_pos;
}

void Block::draw(const Rectangle* visibleAreaPosition)
{
	float offx = 0, offy = 0;
	if(visibleAreaPosition != NULL)
	{
		offx += -visibleAreaPosition->x;
		offy += -visibleAreaPosition->y;
	}

	offx += (convertToPixels(body->getWidth()) - animation->current().width)/2;
	offy +=  convertToPixels(body->getHeight()) - animation->current().height;

	animation->draw(convertToPixels(body->getX()) + offx, convertToPixels(body->getY()) + offy);
}

// static
AnimationSet* Block::createBlockAnimationSet(Image* sheet, unsigned frameCount, float frameInterval)
{
	AnimationSet* animation = new AnimationSet(sheet);

	animation->add("full_cover",        BLOCK_SIZE, BLOCK_SIZE, frameInterval, frameCount);
	animation->add("top_free",          BLOCK_SIZE, BLOCK_SIZE, frameInterval, frameCount);
	animation->add("bottom_free",       BLOCK_SIZE, BLOCK_SIZE, frameInterval, frameCount);
	animation->add("left_free",         BLOCK_SIZE, BLOCK_SIZE, frameInterval, frameCount);
	animation->add("right_free",        BLOCK_SIZE, BLOCK_SIZE, frameInterval, frameCount);
	animation->add("top_left_free",     BLOCK_SIZE, BLOCK_SIZE, frameInterval, frameCount);
	animation->add("bottom_left_free",  BLOCK_SIZE, BLOCK_SIZE, frameInterval, frameCount);
	animation->add("top_right_free",    BLOCK_SIZE, BLOCK_SIZE, frameInterval, frameCount);
	animation->add("bottom_right_free", BLOCK_SIZE, BLOCK_SIZE, frameInterval, frameCount);
	animation->add("horizontal_free",   BLOCK_SIZE, BLOCK_SIZE, frameInterval, frameCount);
	animation->add("vertical_free",     BLOCK_SIZE, BLOCK_SIZE, frameInterval, frameCount);
	animation->add("bottom_covered",    BLOCK_SIZE, BLOCK_SIZE, frameInterval, frameCount);
	animation->add("top_covered",       BLOCK_SIZE, BLOCK_SIZE, frameInterval, frameCount);
	animation->add("right_covered",     BLOCK_SIZE, BLOCK_SIZE, frameInterval, frameCount);
	animation->add("left_covered",      BLOCK_SIZE, BLOCK_SIZE, frameInterval, frameCount);
	animation->add("full_free",         BLOCK_SIZE, BLOCK_SIZE, frameInterval, frameCount);

	return animation;
}
