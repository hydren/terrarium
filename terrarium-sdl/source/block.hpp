#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "animated_entity.hpp"
#include "actor.hpp"
#include "util.hpp"

#include <SDL/SDL.h>

#define DEFAULT_BLOCK_SIZE 16

/* A block is an non-movable entity which is placed in the map grid. Therefore, it has grid coordinates instead of global.
 * Blocks can only be removed using special game items, or by destruction.
 *
 * This class shall contain data like:
 *   grid coordinates;
 *   texture;
 *   dimensions (width and height);
 * */
class Block extends public AnimatedEntity
{
	protected:

	private:


	public:

    int x_grid_pos, y_grid_pos;

    Block(Texture* tex, int x_pos, int y_pos, int sprite_w, int sprite_h);

	~Block();

	//draw this block on the screen
	virtual void draw();

    //draw this block on the screen, in relation to the given visible area.
    virtual void draw(Rectangle& visibleArea);

    //check whether this block intersects with the given bounding box
    bool intersects(Rectangle b);

    //returns a BoundingBox object representing the intersection of this block with the given bounding box
    Rectangle intersection(Rectangle b);

};

#endif // BLOCK_HPP
