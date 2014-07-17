/*
 * allegro_api.hpp
 *
 *  Created on: Apr 17, 2013
 *      Author: felipe
 */

#ifndef ALLEGRO_API_HPP_
#define ALLEGRO_API_HPP_

#include "util.hpp"
#include "image.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class AllegroImage extends public Image
{
public:
	ALLEGRO_BITMAP* img;

	AllegroImage(string filename);
	~AllegroImage();

	void draw(float x=0, float y=0, float from_x=0, float from_y=0, float w=-1, float h=-1);
	void draw_rotated(float x=0, float y=0, float ax=0, float ay=0, float angle=0, float from_x=0, float from_y=0, float w=-1, float h=-1);

//	void blit(Image& img2, float x=0, float y=0, float from_x=0, float from_y=0, float h=-1, float w=-1);
};


#endif /* ALLEGRO_API_HPP_ */
