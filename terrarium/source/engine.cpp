/*
 * engine.cpp
 *
 *  Created on: 28/04/2013
 *      Author: felipe
 */

#include "../header/engine.hpp"

Engine::Display* Engine::display; //declaration of display (because it is an EXTERN variable)

namespace Engine
{
	void initialize()
	{
		al_init();
		al_init_image_addon();
		al_init_font_addon();
		al_init_ttf_addon();

		if(!al_install_keyboard())
		{
			throw(Exception("Could not install keyboard"));
		}

		if(!al_install_mouse())
		{
			throw(Exception("Could not install mouse"));
		}
	}

	void finalize()
	{
		//TODO delete other stuff
		//delete display;
	}

	void rest(double seconds)
	{
		al_rest(seconds);
	}

	Display::Display(int width, int height, String title, Image* icon)
	: allegroDisplay(al_create_display(width, height))
	{
		if(allegroDisplay == NULL) throw Exception("Could not create display! Error "+al_get_errno());
		al_set_window_title(allegroDisplay, title.c_str());
		if(icon != NULL) al_set_display_icon(allegroDisplay, icon->bitmap);
	}

	Display::~Display()
	{
		al_destroy_display(allegroDisplay);
	}

	void Display::setTitle(const String& title)
	{
		al_set_window_title(allegroDisplay, title.c_str());
	}

	void Display::setIcon(Image* image)
	{
		al_set_display_icon(allegroDisplay, image->bitmap);
	}

	void Display::refresh()
	{
		al_flip_display();
	}

	void Display::clear()
	{
		al_clear_to_color(al_map_rgb(0,0,0));
	}

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

	Event::Event()
	: allegroEvent(new ALLEGRO_EVENT())
	{}

	Event::Event(ALLEGRO_EVENT* ev)
	: allegroEvent(ev)
	{}

	Event::Type::value Event::getEventType()
	{
		switch(this->allegroEvent->type)
		{
			case ALLEGRO_EVENT_DISPLAY_CLOSE:		return Event::Type::DISPLAY_CLOSURE;
			case ALLEGRO_EVENT_KEY_DOWN: 			return Event::Type::KEY_PRESS;
			case ALLEGRO_EVENT_KEY_UP:				return Event::Type::KEY_RELEASE;
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: 	return Event::Type::MOUSE_BUTTON_PRESS;
			case ALLEGRO_EVENT_MOUSE_BUTTON_UP:		return Event::Type::MOUSE_BUTTON_RELEASE;

			//TODO map more events...

			default:								return Event::Type::NOTHING;
		}
	}

	Event::Key::value Event::getEventKeyCode()
	{
		switch(this->allegroEvent->keyboard.keycode)
		{
			case ALLEGRO_KEY_UP:	return Event::Key::ARROW_UP;
			case ALLEGRO_KEY_DOWN:	return Event::Key::ARROW_DOWN;
			case ALLEGRO_KEY_LEFT:	return Event::Key::ARROW_LEFT;
			case ALLEGRO_KEY_RIGHT:	return Event::Key::ARROW_RIGHT;

			case ALLEGRO_KEY_ENTER: return Event::Key::ENTER;
			case ALLEGRO_KEY_SPACE: return Event::Key::SPACE;
			case ALLEGRO_KEY_ESCAPE: return Event::Key::ESCAPE;

			//TODO map more buttons...

			default: 				return Event::Key::UNKNOWN;
		}
	}

	Event::MouseButton::value Event::getEventMouseButton()
	{
		switch(this->allegroEvent->mouse.button)
		{
			case ALLEGRO_MOUSE_BUTTON_LEFT:			return Event::MouseButton::LEFT;
			case ALLEGRO_MOUSE_BUTTON_RIGHT:		return Event::MouseButton::RIGHT;
			case ALLEGRO_MOUSE_BUTTON_MIDDLE:		return Event::MouseButton::MIDDLE;

			default:								return Event::MouseButton::UNKNOWN;
		}
	}

	int Event::getEventMouseX()
	{
		return this->allegroEvent->mouse.x;
	}

	int Event::getEventMouseY()
	{
		return this->allegroEvent->mouse.y;
	}

	EventQueue::EventQueue()
	: allegroEventQueue(al_create_event_queue()),
	  allegroEvent(new ALLEGRO_EVENT())
	{
		if(allegroEventQueue == NULL)
			throw Exception("Could not create event queue");

		al_register_event_source(allegroEventQueue, al_get_display_event_source(Engine::display->allegroDisplay));
		al_register_event_source(allegroEventQueue, al_get_keyboard_event_source());
		al_register_event_source(allegroEventQueue, al_get_mouse_event_source());
	}

	bool EventQueue::isEmpty()
	{
		return al_is_event_queue_empty(this->allegroEventQueue);
	}

	Event* EventQueue::waitForEvent()
	{
		al_wait_for_event(allegroEventQueue, allegroEvent);
		return new Event(allegroEvent);
	}

	void EventQueue::waitForEvent(Event* container)
	{
		al_wait_for_event(allegroEventQueue, container->allegroEvent);
	}

	Color::Color(unsigned char r, unsigned char g, unsigned char b)
	: r(r), g(g), b(b)
	{}

	const Color
	Color::WHITE		(255, 255, 255),
	Color::GREY			(127, 127, 127),
	Color::BLACK		(  0,   0,   0),
	Color::RED			(255,   0,   0),
	Color::MAROON		(127,   0,   0),
	Color::YELLOW		(255, 255,   0),
	Color::OLIVE		(127, 127,   0),
	Color::GREEN		(  0, 255,   0),
	Color::DARK_GREEN	(  0, 127,   0),
	Color::CYAN			(  0, 255, 255),
	Color::TEAL			(  0, 127, 127),
	Color::BLUE			(  0,   0, 255),
	Color::NAVY			(  0,   0, 127),
	Color::MAGENTA		(255,   0, 255),
	Color::PURPLE		(127,   0, 127),

	Color::LIGHT_GREY	(192, 192, 192),
	Color::DARK_GREY	( 96,  96,  96),
	Color::ORANGE		(255, 127,   0),
	Color::PINK			(255, 192, 192),
	Color::BROWN		(144,  92,  48);
	//TODO add more colors

	Font::Font(String filename, int size, bool antialiasing, bool hinting, bool kerning)
	: allegroFont( al_load_ttf_font(filename.c_str(), size, (antialiasing? 0 : ALLEGRO_TTF_MONOCHROME)	| (hinting? 0 : ALLEGRO_TTF_NO_AUTOHINT) | (kerning? 0 : ALLEGRO_TTF_NO_KERNING)) ) //I know, very weird...
	{
		if(this->allegroFont == null)
			throw Exception("Font"+filename+" could not be loaded!");
	}

	void Font::draw_text(String text, float x, float y, Color color)
	{
		al_draw_text(this->allegroFont, al_map_rgb(color.r, color.g, color.b), x, y, ALLEGRO_ALIGN_LEFT, text.c_str());
	}

}


