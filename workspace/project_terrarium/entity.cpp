#include "entity.hpp"

Entity::Entity(Texture& tex) :
texture(tex)
{}

Entity::~Entity()
{}

void Entity::draw()
{
	texture.render();
}