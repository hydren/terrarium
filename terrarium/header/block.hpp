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


class Block : public Entity {
private:

public:
	int x_grid_pos, y_grid_pos;

	//constructors
	Block(Image* i, int x_grid, int y_grid, bool ignoreCollision=false);
	Block(AnimationSet* anim, int x_grid, int y_grid, bool ignoreCollision=false);

	~Block();

	int getX();
	int getY();

	void draw(const Rect* visibleAreaPosition=NULL);
};

#endif /* BLOCK_HPP_ */
