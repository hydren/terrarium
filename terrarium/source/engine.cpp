/*
 * engine.cpp
 *
 *  Created on: 28/04/2013
 *      Author: felipe
 */

#include "../header/engine.hpp"

/** Engine code based on Allegro5. */

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

Engine::Display* Engine::display; //declaration of display (because it is an EXTERN variable)

namespace Engine
{
	/** * Definition of the "implementation" struct's (experimental) * */
	// PS: Felipe, don't be scared, it is innofensive. ^^
	struct Display::Implementation
	{
		ALLEGRO_DISPLAY* allegroDisplay;
	};

	struct Image::Implementation
	{
		ALLEGRO_BITMAP* bitmap;
	};

	struct Event::Implementation
	{
		ALLEGRO_EVENT* allegroEvent;
	};

	struct EventQueue::Implementation
	{
		ALLEGRO_EVENT_QUEUE* allegroEventQueue;
		ALLEGRO_EVENT* allegroEvent;
	};

	struct Font::Implementation
	{
		ALLEGRO_FONT* allegroFont;
	};

	// initialize all allegro stuff
	void initialize()
	{
		al_init();
		al_init_image_addon();
		al_init_font_addon();
		al_init_ttf_addon();
		al_init_primitives_addon();

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

	list<string> getFilenamesWithinDirectory(const string& directoryPath)
	{
		ALLEGRO_FS_ENTRY* directory = al_create_fs_entry(directoryPath.c_str());
		al_open_directory(directory);

		list<string> filenames;
		for(ALLEGRO_FS_ENTRY* entry = al_read_directory(directory); entry != null; entry = al_read_directory(directory))
		{
			filenames.push_back(al_get_fs_entry_name(entry));
		}
		return filenames;
	}


	Display::Display(int width, int height, string title, Image* icon)
	{
		this->implementation = new Implementation;
		this->implementation->allegroDisplay = al_create_display(width, height);

		if(this->implementation->allegroDisplay == NULL)
			throw Exception("Could not create display! Error "+al_get_errno());

		al_set_window_title(this->implementation->allegroDisplay, title.c_str());

		if(icon != NULL)
			al_set_display_icon(this->implementation->allegroDisplay, icon->implementation->bitmap);
	}

	Display::~Display()
	{
		al_destroy_display(this->implementation->allegroDisplay);
	}

	void Display::setTitle(const string& title)
	{
		al_set_window_title(this->implementation->allegroDisplay, title.c_str());
	}

	void Display::setIcon(Image* image)
	{
		al_set_display_icon(this->implementation->allegroDisplay, image->implementation->bitmap);
	}

	void Display::refresh()
	{
		al_flip_display();
	}

	void Display::clear()
	{
		al_clear_to_color(al_map_rgb(0,0,0));
	}

	Image::Image(string filename)
	{
		this->implementation = new Implementation;
		this->implementation->bitmap = al_load_bitmap(filename.c_str());
		if ( this->implementation->bitmap == null)
			throw Exception("AllegroAPI Constructor - Could not load image: " + filename);
	}

	/**
	 * Creates a colored shape. Depending on the shape, differents arguments are used.
	 *
	 * RECTANGLE				width, height, thickness
	 * FILLED_RECTANGLE			width, height
	 *
	 * All of them must be int.
	 * */
	Image::Image(Shape shape, Color color, float arg1, float arg2, float arg3)
	{
		this->implementation = new Implementation;
		this->implementation->bitmap = null;

		ALLEGRO_COLOR c =  al_map_rgb(color.r, color.g, color.b);
		switch(shape)
		{
			case Image::RECTANGLE:
			{
				float width = arg1;
				float height = arg2;
				float thickness = arg3;

				ALLEGRO_BITMAP* b = al_create_bitmap(width, height);
				al_set_target_bitmap(b);
				al_draw_rectangle(0, 0, width, height, c, thickness);
				al_set_target_backbuffer(display->implementation->allegroDisplay);
				this->implementation->bitmap = b;
				break;
			}
			case Image::FILLED_RECTANGLE:
			{
				float width = arg1;
				float height = arg2;

				ALLEGRO_BITMAP* b = al_create_bitmap(width, height);
				al_set_target_bitmap(b);
				al_draw_filled_rectangle(0, 0, width, height, c);
				al_set_target_backbuffer(display->implementation->allegroDisplay);
				this->implementation->bitmap = b;
				break;
			}
			//TODO finish other cases with other shapes

			default: break;
		}
	}

	Image::~Image()
	{
		al_destroy_bitmap(this->implementation->bitmap);
	}

	void Image::draw(float x, float y, float from_x, float from_y, float w, float h)
	{
		//COUT << x << " " << y << " " << from_x << " " << from_y << " " << w << " " << h << " " << ENDL;
		if(w == -1 && h == -1) //draw all source region
			al_draw_bitmap(this->implementation->bitmap, x, y, 0);
		else
			al_draw_bitmap_region(this->implementation->bitmap, from_x, from_y, w, h, x, y, 0);
	}

	void Image::draw_rotated(float x, float y, float ax, float ay, float angle, float from_x, float from_y, float w, float h)
	{
		if(w == -1 && h == -1) //draw all source region
			al_draw_rotated_bitmap(this->implementation->bitmap, ax, ay, x, y, angle, 0);
		else
			al_draw_tinted_scaled_rotated_bitmap_region(this->implementation->bitmap, from_x, from_y, w, h, al_map_rgba_f(1, 1, 1, 1), ax, ay, x, y, 1, 1, angle, 0);
	}

	void Image::blit(Image& img2, float x, float y, float from_x, float from_y, float h, float w)
	{
		al_set_target_bitmap( img2.implementation->bitmap );

		if(w == -1 && h == -1) //draw all source region
			al_draw_bitmap(this->implementation->bitmap, x, y, 0);
		else
			al_draw_bitmap_region(this->implementation->bitmap, from_x, from_y, w, h, x, y, 0);

		al_set_target_backbuffer(al_get_current_display());
	}

	float Image::getWidth()  { return al_get_bitmap_width(this->implementation->bitmap); } //TODO possibly incorrect!
	float Image::getHeight() { return al_get_bitmap_height(this->implementation->bitmap); }

	Event::Event()
	{
		this->implementation = new Implementation;
		this->implementation->allegroEvent = new ALLEGRO_EVENT;
	}



	Event::Type::value Event::getEventType()
	{
		switch(this->implementation->allegroEvent->type)
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
		switch(this->implementation->allegroEvent->keyboard.keycode)
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
		switch(this->implementation->allegroEvent->mouse.button)
		{
			case 1:		return Event::MouseButton::LEFT;
			case 2:		return Event::MouseButton::RIGHT;
			case 3:		return Event::MouseButton::MIDDLE;

			default:	return Event::MouseButton::UNKNOWN;
		}
	}

	int Event::getEventMouseX()
	{
		return this->implementation->allegroEvent->mouse.x;
	}

	int Event::getEventMouseY()
	{
		return this->implementation->allegroEvent->mouse.y;
	}

	EventQueue::EventQueue()
	{
		this->implementation = new Implementation;

		this->implementation->allegroEvent = new ALLEGRO_EVENT;
		this->implementation->allegroEventQueue = al_create_event_queue();

		if(this->implementation->allegroEventQueue == NULL)
			throw Exception("Could not create event queue");

		al_register_event_source(this->implementation->allegroEventQueue, al_get_display_event_source(Engine::display->implementation->allegroDisplay));
		al_register_event_source(this->implementation->allegroEventQueue, al_get_keyboard_event_source());
		al_register_event_source(this->implementation->allegroEventQueue, al_get_mouse_event_source());
	}

	bool EventQueue::isEmpty()
	{
		return al_is_event_queue_empty(this->implementation->allegroEventQueue);
	}

	Event* EventQueue::waitForEvent()
	{
		al_wait_for_event(this->implementation->allegroEventQueue, this->implementation->allegroEvent);
		Event* ev = new Event;
		ev->implementation->allegroEvent = this->implementation->allegroEvent;
		return ev;
	}

	void EventQueue::waitForEvent(Event* container)
	{
		al_wait_for_event(this->implementation->allegroEventQueue, container->implementation->allegroEvent);
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



	Font::Font(string filename, int size, bool antialiasing, bool hinting, bool kerning)
	{
		this->implementation = new Implementation;

		//I know, pretty odd...
		this->implementation->allegroFont = al_load_ttf_font(filename.c_str(), size, (antialiasing? 0 : ALLEGRO_TTF_MONOCHROME)	| (hinting? 0 : ALLEGRO_TTF_NO_AUTOHINT) | (kerning? 0 : ALLEGRO_TTF_NO_KERNING));

		if(this->implementation->allegroFont == null)
			throw Exception("Font"+filename+" could not be loaded!");
	}

	void Font::draw_text(string text, float x, float y, Color color)
	{
		al_draw_text(this->implementation->allegroFont, al_map_rgb(color.r, color.g, color.b), x, y, ALLEGRO_ALIGN_LEFT, text.c_str());
	}

	int Font::getSize() const
	{
		return al_get_font_line_height(this->implementation->allegroFont);
	}
}


