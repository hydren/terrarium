/*
 * engine.hpp
 *
 *  Created on: 28/04/2013
 *      Author: felipe
 */

#ifndef ENGINE_HPP_
#define ENGINE_HPP_

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "util.hpp"
#include "image.hpp"

namespace Engine
{
	void initialize();

	void finalize();

	void rest(double seconds=0.05f);

	struct Color
	{
		unsigned char r, g, b;

		Color(unsigned char r=0, unsigned char g=0, unsigned char b=0);

		static const Color
		// basic paint colors
		WHITE, GREY, BLACK, RED, MAROON, YELLOW, OLIVE, GREEN, DARK_GREEN, CYAN, TEAL, BLUE, NAVY, MAGENTA, PURPLE,

		// basic extra colors
		LIGHT_GREY, DARK_GREY, ORANGE, PINK, BROWN
		;
	};

	class Event
	{
		private:
		ALLEGRO_EVENT* allegroEvent;

		Event();
		Event(ALLEGRO_EVENT* ev);

		public:

		struct Type{ enum value
		{
			KEY_PRESS,
			KEY_RELEASE,
			MOUSE_BUTTON_PRESS,
			MOUSE_BUTTON_RELEASE,
			MOUSE_MOTION,
			DISPLAY_CLOSURE,
			NOTHING
		};};

		struct Key{ enum value
		{
			A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
			_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
			ARROW_UP,
			ARROW_DOWN,
			ARROW_LEFT,
			ARROW_RIGHT,
			ENTER,
			SPACE,
			ESCAPE,
			UNKNOWN
		};};

		struct MouseButton{ enum value{
			LEFT,
			RIGHT,
			MIDDLE,
			UNKNOWN
		};};

		Event::Type::value getEventType();

		Event::Key::value getEventKeyCode();

		Event::MouseButton::value getEventMouseButton();

		int getEventMouseX();

		int getEventMouseY();

		class Queue
		{
			private:
			ALLEGRO_EVENT_QUEUE* allegroEventQueue;
			ALLEGRO_EVENT* allegroEvent;

			public:

			Queue();

			bool isEmpty();

			Event* waitForEvent();
			void waitForEvent(Event* container);

		};
	};

	/**
	 * An engine display.
	 * */
	class Display
	{
		private:

		ALLEGRO_DISPLAY* allegroDisplay;

		public:

		/**
		 * Creates a display with the given size, title and icon.
		 * */
		Display(int width=800, int height=600, String title="", Image* icon=NULL);

		//delete the display
		~Display();

		/** Change the window title. */
		void setTitle(const String& title);

		/** Change the window icon. */
		void setIcon(Image* image);

		/**
		 * Function to refresh/flush all thing rendered in the screen. Without this you'll probably get a black screen showing nothing.
		 * It is recommended that you call this function after you done all drawing needed in the current game cycle.
		 */
		void refresh();

		/**
		 *  Cleans the whole window, painting it black. Rolling Stones.
		 */
		void clear();

		friend class Event::Queue;
	};

	/** A pointer to the main display instance */
	extern Display* display; //set to extern to avoid multiple definition when including this header (like a static variable, but without a class; it is inside a namespace)

	class Font
	{
		private:

		ALLEGRO_FONT* allegroFont;

		public:

		Font(String filename, int size=12, bool antialiasing=true, bool hinting=true, bool kerning=true);

		void draw_text(String text, float x=0, float y=0, Color color=Color::BLACK);
	};

}


#endif /* ENGINE_HPP_ */
