#include "map.hpp"

Map::Map(Texture* tex, int width, int height):
Entity(tex),
width(width),
height(height),
visibleArea(Rectangle(0,0,width,height))
{
    //grid = std::vector< std::vector<Block*> >(this->width/DEFAULT_BLOCK_SIZE, std::vector<Block*>(this->height/DEFAULT_BLOCK_SIZE, NULL));

    for ( int i=0; i<this->width/DEFAULT_BLOCK_SIZE; i++ )
    {
        grid.push_back( std::vector<Block*>(this->height/DEFAULT_BLOCK_SIZE) );
    }
}

void Map::draw()
{
    Entity::draw();
    if( visibleArea.area() != 0 )
    {
        //draws all blocks that can be fully or partially seen, but not the ones that can't be seen
        int start_i_index = visibleArea.x / DEFAULT_BLOCK_SIZE;
        int start_j_index = visibleArea.y / DEFAULT_BLOCK_SIZE;
        int finish_i_index = (visibleArea.x + visibleArea.w) / DEFAULT_BLOCK_SIZE;
        int finish_j_index = (visibleArea.y + visibleArea.h) / DEFAULT_BLOCK_SIZE;

        int grid_number_of_lines = width/DEFAULT_BLOCK_SIZE;
        int grid_number_of_columns = height/DEFAULT_BLOCK_SIZE;

        //just to be safe
        if( start_i_index < 0 ) start_i_index = 0;
        else if( start_i_index >  grid_number_of_lines -1 ) start_i_index =  grid_number_of_lines -1;
        if( start_j_index < 0 ) start_j_index = 0;
        else if(start_j_index > grid_number_of_columns -1 ) start_j_index = grid_number_of_columns - 1;
        if( finish_i_index < 0 ) finish_i_index = 0;
        else if ( finish_i_index >  grid_number_of_lines -1 ) finish_i_index =  grid_number_of_lines -1;
        if( finish_j_index < 0 ) finish_j_index = 0;
        else if ( finish_j_index > grid_number_of_columns -1 ) finish_j_index = grid_number_of_columns-1;

        //draw the corresponding blocks
        for( int i = start_i_index ; i <= finish_i_index ; i++)
         for( int j = start_j_index ; j <= finish_j_index ; j++)
          if( grid[i][j] != NULL )
           {
               grid[i][j]->draw(visibleArea);
           }
    }
}
