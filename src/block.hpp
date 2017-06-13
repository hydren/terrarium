/*
 * block.hpp
 *
 *  Created on: Jun 18, 2013
 *      Author: felipe
 */

#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#define BLOCK_SIZE 16

#include "entity.hpp"

class Block extends public Entity
{
	public:
	int x_grid_pos, y_grid_pos;

	int typeID;

	//constructors
	Block(AnimationSet* anim, int x_grid, int y_grid, int typeID, bool ignoreCollision=false);

	int getX();
	int getY();

	void draw(const Rectangle* visibleAreaPosition=NULL);

	// creates a proper block-type animation set based on the given arguments.
	static AnimationSet* createBlockAnimationSet(Image* sheet, unsigned frameCount=1, float frameInterval=-1);
};

#endif /* BLOCK_HPP_ */
