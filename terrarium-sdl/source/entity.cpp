#include "entity.hpp"

Entity::Entity(Texture* tex) :
texture(tex)
{}

Entity::~Entity()
{}

void Entity::draw()
{
    texture->render();
}

Entity& Entity::operator =(const Entity& ent)
{
    this->texture = ent.texture;
    return *this;
}
