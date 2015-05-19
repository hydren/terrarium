/*
 * engine.hpp
 *
 *  Created on: 28/04/2013
 *      Author: felipe
 */

#ifndef ENGINE_HPP_
#define ENGINE_HPP_

#include "util.hpp"

namespace Engine
{
	//pre-declaring these classes to avoid annoying declaration order issues...
	class Image;
	class EventQueue;
	class Color;

	/** Initialize underlying engine software. You MUST call this before trying anything */
	void initialize(); //TODO throw an exception if any Engine method is called before one call to initialize() method

	/** Shutdown underlying engine software. */
	void finalize();

	void rest(double seconds=0.05f);

	list<string> getFilenamesWithinDirectory(const string& directoryPath);

	/**
	 * A class representing a display.
	 * */
	class Display
	{
		friend class EventQueue; //needed to give the event queue access to the internal display
		friend class Image; //needed to give access to the image for bliting operations

		struct Implementation;
		Implementation* implementation;

		public:

		/**
		 * Creates a display with the given size, title and icon.
		 * */
		Display(int width=800, int height=600, string title="", Image* icon=NULL);

		//delete the display
		~Display();

		/** Change the window title. */
		void setTitle(const string& title);

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

	};

	/** A pointer to the main display instance */
	extern Display* display; //set to extern to avoid multiple definition when including this header (like a static variable, but without a class; it is inside a namespace)

	class Image
	{
		friend class Display; //needed to give the Display access to the underlying bitmap

		struct Implementation;
		Implementation* implementation;

		public:

		enum Shape
		{
			TRIANGLE, FILLED_TRIANGLE, RECTANGLE, FILLED_RECTANGLE, ROUNDED_RECTANGLE, FILLED_ROUNDED_RECTANGLE, ELLIPSE, FILLED_ELLIPSE, CIRCLE, FILLED_CIRCLE
		};

		Image(Shape shape, Color color, float arg1=-1, float arg2=-1, float arg3=-1);
		Image(string filename);
		~Image();

		float getWidth();
		float getHeight();

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

	};

	/**
	 * A class representing an event from the engine (keyboard, mouse, etc).
	 * */
	class Event
	{
		friend class EventQueue; //event queue must be able to access the inner event object

		struct Implementation;
		Implementation* implementation;

		Event();
		~Event();

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
	};

	class EventQueue
	{
		struct Implementation;
		Implementation* implementation;

		public:

		EventQueue();
		~EventQueue();

		bool isEmpty();

		Event* waitForEvent();
		void waitForEvent(Event* container);

		void ignoreEvents();
		void listenEvents();
		void flushEvents();
	};

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

	class Font
	{
		struct Implementation;
		Implementation* implementation;

		public:

		Font(string filename, int size=12, bool antialiasing=true, bool hinting=true, bool kerning=true);
		~Font();

		void draw_text(string text, float x=0, float y=0, Color color=Color::BLACK);

		int getSize() const;
	};

}


#endif /* ENGINE_HPP_ */
