/*
 * image.hpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include "util.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

namespace Engine { class Display; }//needed for the friendship with Engine::Display

class Image
{
	private:
	ALLEGRO_BITMAP* bitmap;

	public:

	Image(String filename);
	~Image();

	float getWidth() { return al_get_bitmap_width(bitmap); } //TODO possibly incorrect!
	float getHeight() { return al_get_bitmap_height(bitmap); }

    //to be able to use it on std::vector
    Image& operator = (const Image& t)
    {
        //included the unnecessary 'if' block just to shut up the compiler warnings about unused variable
        if(this != &t) return (*this);
        return (*this);
    }

	/**
	 * Render an area of this Image in the game screen, positioning the Image in the given coordinates.
	 * The area selected is selected following the given parameters.
	 * from_x and from_y specifies the position, w and h specifies the size of the selected area.
	 */
	void draw(float x=0, float y=0, float from_x=0, float from_y=0, float w=-1, float h=-1);

	void draw_rotated(float x=0, float y=0, float ax=0, float ay=0, float angle=0, float from_x=0, float from_y=0, float w=-1, float h=-1);

	/**
	 * Render an area of this Image in Image img2, positioning the Image in the given coordinates.
	 * The area selected is selected following the given parameters.
	 * from_x and from_y specifies the position, w and h specifies the size of the selected area.
	 */
	void blit(Image& img2, float x=0, float y=0, float from_x=0, float from_y=0, float h=-1, float w=-1);

	friend class Engine::Display; //needed to give the Display access to the underlying bitmap
};


#endif /* IMAGE_HPP_ */
