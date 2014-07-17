#include "block.hpp"

Block::Block(Texture* tex, int x_pos, int y_pos, int sprite_w, int sprite_h):
AnimatedEntity(tex, sprite_w, sprite_h),
x_grid_pos(x_pos),
y_grid_pos(y_pos)
{
}

Block::~Block()
{
}

void Block::draw()
{
    texture->render(x_grid_pos*DEFAULT_BLOCK_SIZE, y_grid_pos*DEFAULT_BLOCK_SIZE, spriteWidth * animation.timeBasedIndex(), spriteHeight * animation.current() , spriteWidth, spriteHeight);
}

void Block::draw(Rectangle& visibleArea)
{
    texture->render(x_grid_pos*DEFAULT_BLOCK_SIZE - visibleArea.x, y_grid_pos*DEFAULT_BLOCK_SIZE - visibleArea.y, spriteWidth * animation.timeBasedIndex(), spriteHeight * animation.current() , spriteWidth, spriteHeight);
}

bool Block::intersects(Rectangle b)
{
    Rectangle tmp(x_grid_pos*DEFAULT_BLOCK_SIZE, y_grid_pos*DEFAULT_BLOCK_SIZE, DEFAULT_BLOCK_SIZE, DEFAULT_BLOCK_SIZE);
    return b.intersects(tmp);
}

Rectangle Block::intersection(Rectangle b)
{
    Rectangle tmp(x_grid_pos*DEFAULT_BLOCK_SIZE, y_grid_pos*DEFAULT_BLOCK_SIZE, DEFAULT_BLOCK_SIZE, DEFAULT_BLOCK_SIZE);
    return b.intersection(tmp);
}

