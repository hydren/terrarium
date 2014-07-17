#ifndef PC_HPP
#define PC_HPP

#include "util.hpp"
#include "character.hpp"

/* This class defines the player character, which is playable. =)
 **/
class PC : public Character
{
public:
    PC(Texture* tex, int pos_x, int pos_y, int width, int height);
};

#endif // PC_HPP
