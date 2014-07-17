#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "animated_entity.hpp"
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
	int x_grid_pos, y_grid_pos;
	
	public:
	Block(Texture& tex, int x_pos, int y_pos, int sprite_w, int sprite_h);
	
	~Block();
	
	//draw this block on the screen
	virtual void draw();

};

#endif // BLOCK_HPP
