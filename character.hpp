#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "util.hpp"
#include "actor.hpp"

/* A character is any player or non-player actor.
 * Corresponds to the PC's and NPC's.
 **/
class Character extends public Actor
{
protected:

private:
    int HP, MP;

public:
    Character(Texture *tex, int pos_x, int pos_y, int width, int height);
};

#endif // PLAYER_HPP
