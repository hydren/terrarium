#ifndef MAP_HPP
#define MAP_HPP

#include "util.hpp"
#include "block.hpp"
#include "main_menu.hpp"

#include <vector>

class Map extends public Entity
{
private:
    int width, height;

public:
    Map(Texture* tex, int width, int height);

    std::vector< std::vector<Block*> > grid;
    Rectangle visibleArea;

    virtual void draw();

    const int& getWidth() {return this->width;}
    const int& getHeight() {return this->height;}
};

#endif // MAP_HPP
