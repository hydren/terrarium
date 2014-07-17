/*
 * allegro_api.cpp
 *
 *  Created on: Apr 17, 2013
 *      Author: felipe
 */
#include "../header/allegro_api.hpp"

//////////////////////   AllegroImage methods
void AllegroImage::draw(float x, float y, float from_x, float from_y, float w, float h){
	if(w == -1 && h == -1){
		al_draw_bitmap(this->img, x, y, 0);
	}else{
		al_draw_bitmap_region(this->img, from_x, from_y, w, h, x, y, 0);
	}
}

void AllegroImage::draw_rotated(float x, float y, float ax, float ay, float angle, float from_x, float from_y, float w, float h){
	if(w == -1 && h == -1){
		al_draw_rotated_bitmap(this->img, ax, ay, x, y, angle, 0);
	}else{

		/*
		//works similar
		ALLEGRO_BITMAP *temp = al_create_bitmap(w, h);
		al_set_target_bitmap(temp);
		al_draw_bitmap_region(this->img, from_x, from_y, w, h, 0, 0, 0);
		al_set_target_backbuffer(al_get_current_display());
		al_draw_rotated_bitmap(temp, ax, ay, x, y, angle, 0);
		al_destroy_bitmap(temp);
		*/

		al_draw_tinted_scaled_rotated_bitmap_region(this->img, from_x, from_y, w, h, al_map_rgba_f(1, 1, 1, 1), ax, ay, x, y, 1, 1, angle, 0);
	}
}

AllegroImage::AllegroImage(string filename)
{
	if ( ( img = al_load_bitmap(filename.c_str()) ) == NULL){
		throw Exception("AllegroAPI Constructor - Could not load image: " + filename);
	}
}

AllegroImage::~AllegroImage(){
	al_destroy_bitmap(this->img);
}
