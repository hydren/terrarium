/*
 * main.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#include <cstdlib>
#include "../header/util.hpp"
#include "../header/entity.hpp"
#include "../header/allegro_api.hpp"

using std::cout;
using std::endl;

int main(int argc, char** argv)
{
	try{
		al_init();
		al_init_image_addon();

		Image *i;
		cout << argv[0] << endl;

		i = new AllegroImage("eclipse-cdt.png");

		ALLEGRO_DISPLAY *display = NULL;

		display = al_create_display(800, 600);

		al_clear_to_color(al_map_rgb(0,0,0));

		i->draw_rotated(400,300,100,100, ALLEGRO_PI / 3.0f, 100, 100, 100, 100);

		al_flip_display();

		al_rest(4.0);

		al_destroy_display(display);

		delete i;
	}
	catch(Exception& e)
	{
		std::cout << e.message() << std::endl;
	}
	return EXIT_SUCCESS;
}
