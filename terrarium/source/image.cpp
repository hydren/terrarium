/*
 * image.cpp
 *
 *  Created on: 28/04/2013
 *      Author: felipe
 */

#include "../header/image.hpp"

Image::Image(String filename)
: bitmap(al_load_bitmap(filename.c_str()))
{
	if ( bitmap == NULL)
		throw Exception("AllegroAPI Constructor - Could not load image: " + filename);
}

Image::~Image()
{
	al_destroy_bitmap(this->bitmap);
}

void Image::draw(float x, float y, float from_x, float from_y, float w, float h)
{
	//COUT << x << " " << y << " " << from_x << " " << from_y << " " << w << " " << h << " " << ENDL;
	if(w == -1 && h == -1) //draw all source region
		al_draw_bitmap(this->bitmap, x, y, 0);
	else
		al_draw_bitmap_region(this->bitmap, from_x, from_y, w, h, x, y, 0);
}

void Image::draw_rotated(float x, float y, float ax, float ay, float angle, float from_x, float from_y, float w, float h)
{
	if(w == -1 && h == -1) //draw all source region
		al_draw_rotated_bitmap(this->bitmap, ax, ay, x, y, angle, 0);
	else
		al_draw_tinted_scaled_rotated_bitmap_region(this->bitmap, from_x, from_y, w, h, al_map_rgba_f(1, 1, 1, 1), ax, ay, x, y, 1, 1, angle, 0);
}

void Image::blit(Image& img2, float x, float y, float from_x, float from_y, float h, float w)
{
	al_set_target_bitmap( img2.bitmap );

	if(w == -1 && h == -1) //draw all source region
		al_draw_bitmap(this->bitmap, x, y, 0);
	else
		al_draw_bitmap_region(this->bitmap, from_x, from_y, w, h, x, y, 0);

	al_set_target_backbuffer(al_get_current_display());
}
