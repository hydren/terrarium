/*
 * block.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: felipe
 */

#include "block.hpp"

using Physics::convertToPixels;

/** Caution: the given animation should be correctly configured for blocks */
Block::Block(Animation* anim, int x_grid, int y_grid, int typeID, bool ignoreCollision):
Entity(anim, new Body(Physics::convertToMeters(x_grid*BLOCK_SIZE), Physics::convertToMeters(y_grid*BLOCK_SIZE), Physics::convertToMeters(BLOCK_SIZE), ignoreCollision)),
typeID(typeID),
gridX(x_grid),
gridY(y_grid)
{}

void Block::draw(const Rectangle& visibleArea)
{
	float offx = 0, offy = 0;
	offx += -visibleArea.x;
	offy += -visibleArea.y;

	offx += (convertToPixels(body->getWidth()) - animation->current().width)/2;
	offy +=  convertToPixels(body->getHeight()) - animation->current().height;

	animation->draw(convertToPixels(body->getX()) + offx, convertToPixels(body->getY()) + offy);
}

// static
StackedSingleSheetAnimation* Block::createBlockAnimationSet(Image* sheet, unsigned frameCount, float frameInterval)
{
	StackedSingleSheetAnimation* animation = new StackedSingleSheetAnimation(sheet);

	animation->addSprite(BLOCK_SIZE, BLOCK_SIZE, frameCount, frameInterval);  // FULL_COVER
	animation->addSprite(BLOCK_SIZE, BLOCK_SIZE, frameCount, frameInterval);  // TOP_FREE
	animation->addSprite(BLOCK_SIZE, BLOCK_SIZE, frameCount, frameInterval);  // BOTTOM_FREE
	animation->addSprite(BLOCK_SIZE, BLOCK_SIZE, frameCount, frameInterval);  // LEFT_FREE
	animation->addSprite(BLOCK_SIZE, BLOCK_SIZE, frameCount, frameInterval);  // RIGHT_FREE
	animation->addSprite(BLOCK_SIZE, BLOCK_SIZE, frameCount, frameInterval);  // TOP_LEFT_FREE
	animation->addSprite(BLOCK_SIZE, BLOCK_SIZE, frameCount, frameInterval);  // BOTTOM_LEFT_FREE
	animation->addSprite(BLOCK_SIZE, BLOCK_SIZE, frameCount, frameInterval);  // TOP_RIGHT_FREE
	animation->addSprite(BLOCK_SIZE, BLOCK_SIZE, frameCount, frameInterval);  // BOTTOM_RIGHT_FREE
	animation->addSprite(BLOCK_SIZE, BLOCK_SIZE, frameCount, frameInterval);  // HORIZONTAL_FREE
	animation->addSprite(BLOCK_SIZE, BLOCK_SIZE, frameCount, frameInterval);  // VERTICAL_FREE
	animation->addSprite(BLOCK_SIZE, BLOCK_SIZE, frameCount, frameInterval);  // BOTTOM_COVERED
	animation->addSprite(BLOCK_SIZE, BLOCK_SIZE, frameCount, frameInterval);  // TOP_COVERED
	animation->addSprite(BLOCK_SIZE, BLOCK_SIZE, frameCount, frameInterval);  // RIGHT_COVERED
	animation->addSprite(BLOCK_SIZE, BLOCK_SIZE, frameCount, frameInterval);  // LEFT_COVERED
	animation->addSprite(BLOCK_SIZE, BLOCK_SIZE, frameCount, frameInterval);  // FULL_FREE

	return animation;
}
