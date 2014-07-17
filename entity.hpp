#ifndef ENTITY_HPP_INCLUDED
#define ENTITY_HPP_INCLUDED

#include "texture.hpp"

class Entity
{
	protected:
	
	/* This entity Texture.  
	*/
    Texture* texture;
	
	public:
	
	/* Creates an Entity with the given Texture.
	 * */
    Entity(Texture* tex);

    Entity& operator = (const Entity& ent);
	
	/* Destroys this Entity.
	 * This needed to be virtual, so subclasses can be destroyed accordingly, even when referencied by pointers of the base class.
	 * Otherwise, subclasses would be destroyed using the destructor of the base class, which can make shit happen.
	*/
	virtual ~Entity();
	
	/* Draws this Entity on the screen;
	 * */
	virtual void draw();
	
};

#endif // ENTITY_HPP_INCLUDED
