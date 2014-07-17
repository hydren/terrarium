#ifndef SDL_HPP_INCLUDED
#define SDL_HPP_INCLUDED

#include <cstdlib>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "util.hpp"
#include "texture.hpp"

class SDL
{
	public:

	/* Screen referencing the game window. Global and static.
	 */ 
	static SDL_Surface* screen;
	
	/* Frames per second to try to achieve when using SDL::rest().
	 */
    static int cycleFrequency;

	/* Function to initialize SDL. Use it before using any SDL-related functions. 
	 */
	static void initialize();
	
	/* Function to finalize SDL. Use it when you stop using SDL-related functions.
	 */ 
	static void finalize();

    /**
      * Returns the time, in milliseconds, since SDL initialization
      */
    static Uint32 getTimeSinceSDLInitialization();
	
	/* Function to make the game thread sleep. 
	 * The amount of sleept time will be choosen in a way that attempt to respect the game's setted FPS.
	 */
	static void rest();
	
	// Function to make the game thread sleep for the specified amount of milliseconds.
	static void rest(const int& mils);
	
	/* Function to load a image to a SDL_Surface, and automatically checks for errors. Throws a Exception when something go wrong.
	 */
	static SDL_Surface* loadImage(const String filename);
	
	/* Function to load a sound to a Mix_Chunk, and automatically checks for errors. Throws a Exception when something go wrong.
	 */
	static Mix_Chunk* loadSound(const String filename);
	
	/* Function to load a song to a Mix_Music, and automatically checks for errors. Throws a Exception when something go wrong.
	 */
	static Mix_Music* loadSong(const String filename);
	
	/* Function to load a font to a TTF_Font, and automatically checks for errors. Throws a Exception when something go wrong.
	 */
	static TTF_Font* loadFont(const String filename, int size);

    //Returns a SDL_Color structure representing the color specified by the given flag. See SDL::Color for the flag values.
    static SDL_Color getColor(int color_flag);

    //Returns a Uint32 representing the color given by the flag
    static int getColorNumber(int color_flag);

	/* Class to organize window-related tasks, like creating and setting it fullscreen.
	 */
	class Window
	{
		//most important flags
		static bool hardwareStored, fullscreen, doubleBuffered, resizable, openGLRendered;

		//misc flags
		static bool framed, asynchronousBlitted, hardwarePalette, anyPixelFormat;
		
		//experimental, not sure if works...
		static bool hardwareAccelerated;
		
		/* Function to set a window, checking all boolean flags to use in function. for errors. Throws a Exception when something go wrong.
		   Not meant to be used outside class
		 */ 
		static void createCheckingFlags(int width, int height,int quality);
		
		public:
		
		/* Function to set a window. Throws a Exception when something go wrong.
		*/
		static void create(int width, int height);
		
		/* Function to set a window, specifying also the quality.
		 * Use it only when you know what you're doing.
		 * Throws a Exception when something go wrong.
		*/
		static void create(int width, int height, int quality);
		
		/* Function to set a window, specifying also the quality and flags.
		 * Use it only when you know what you're doing.
		 * Throws a Exception when something go wrong.
		*/
		static void create(int width, int height, int quality, int flags);
		
		/* Cleans the whole window, painting it black. Rolling Stones.
		 */ 
		static void clean();
		
		/* Function to refresh/flush all thing rendered in the screen. Without this you'll probably get a black screen showing nothing.
		* It is recommended that you call this function after you done all drawing needed in the current game cycle.
		*/
		static void refresh();
		
		/* Function to change the window title and iconified(minimized) title.
		 */
		static void setCaption(const String& title, const String& iconified_title);
		
		/* Sets the icon for the display window. 
		 * Win32 icons must be 32x32.
		 * This function must be called before the first call to Window::create() (SDL_SetVideoMode).
		 * This function may throw an Exception if the filename is not a file, or does not exist on the current folder. 
		 * */
		static void setIcon(const String& filename);
		
		/* Function to set to the current window the SDL_ANYFORMAT flag.
		 * This flag makes SDL window use any native pixel format it gets from the OS, instead of emulating it.
		 * Normally, if a video surface of the requested bits-per-pixel (bpp) is not available, SDL will emulate one with a shadow surface. 
		 * Passing SDL_ANYFORMAT prevents this and causes SDL to use the video surface, regardless of its pixel depth.
		*/
		static void setAnyPixelFormat(bool anyPixelFormat);
		
		/* Function to set to the current window the SDL_ASYNCBLIT flag.
		 * Enables the use of asynchronous updates of the display surface. 
		 * This will usually slow down blitting on single CPU machines, but may provide a speed increase on SMP systems. 
		*/
		static void setAsynchronousBlitted(bool asynchronousBlitted);
		
		/* Function to set to the current window the SDL_DOUBLEBUF flag. 
		 * This activates double-buffering in window rendering. Only valid with SDL_HWSURFACE. 
		 * Calling SDL_Flip will flip the buffers and update the screen. 
		 * All drawing will take place on the surface that is not displayed at the moment. If double buffering could not be enabled then SDL_Flip will just perform a SDL_UpdateRect on the entire screen.
		*/
		static void setDoubleBuffered(bool doubleBuffered);
		
		/* Function to set to the current window the SDL_NOFRAME flag.
		 * If it is setted false, the flag SDL_NOFRAME will be added.
		 * SDL_NOFRAME causes, if possible, SDL to create a window with no title bar or frame decoration.
		 * Throws a Exception when something go wrong.
		*/
		static void setFramed(bool framed);
		
		/* Function to set to the current window the SDL_FULLSCREEN flag.
		 * If it is setted true, it will make the window fullscreen, stretching anything inside.
		 * If a hardware resolution change is not possible (for whatever reason), the next higher resolution will be used and the display window centered on a black background.
		 * It also automatically adds the SDL_NOFRAME flag to the screen, but does not alter the boolean variable.
		 * Throws a Exception when something go wrong.
		*/
		static void setFullscreen(bool fullscreen);
		
		/* Function to set to the current window the SDL_HWPALETTE flag.
		 * Give SDL exclusive palette access. 
		 * Without this flag you may not always get the colors you request with SDL_SetColors or SDL_SetPalette.
		 * Throws a Exception when something go wrong.
		*/
		static void setHardwarePalette(bool hardwarePalette);
		
		/* Function to set to the current window the SDL_HWSURFACE flag.
		 * Create the video surface in video memory.
		 * Use SDL_SWSURFACE if you plan on doing per-pixel manipulations, or blit surfaces with alpha channels, and require a high framerate.
		 * When you use hardware surfaces (by passing the flag SDL_HWSURFACE as parameter), SDL copies the surfaces from video memory to system memory when you lock them, and back when you unlock them. This can cause a major performance hit. 
		 * Be aware that you may request a hardware surface, but receive a software surface because the video driver doesn't support hardware surface. 
		 * Many platforms can only provide a hardware surface when using SDL_FULLSCREEN. The SDL_HWSURFACE flag is best used when the surfaces you'll be blitting can also be stored in video memory. 
		*/
		static void setHardwareStored(bool hardwareStored);
		
		/* Function to set to the current window the SDL_RESIZABLE flag.
		 * Makes the window resizable. 
		 * When the window is resized by the user a SDL_VIDEORESIZE event is generated and SDL_SetVideoMode can be called again with the new size.
		 * Throws a Exception when something go wrong.
		*/
		static void setResizable(bool resizable);
		
		/* Function to set to the current window the SDL_OPENGL flag.
		 * Create an OpenGL rendering context. 
		 * You should have previously set OpenGL video attributes with SDL_GL_SetAttribute.
		 * Throws a Exception when something go wrong.
		*/
		static void setOpenGLRendered(bool openGLRendered);
		
		/* Function to set to the current window the SDL_HWACCEL flag.
		 * Use hardware acceleration blit. Experimental.
		 * Throws a Exception when something go wrong.
		*/
		static void setHardwareAccelerated(bool hardwareAccelerated);
		
		static bool isAnyPixelFormat();
		static bool isAsynchronousBlitted();
		static bool isDoubleBuffered();
		static bool isFramed();
		static bool isFullscreen();
		static bool isHardwarePalette();
		static bool isHardwareStored();
		static bool isResizable();
		static bool isOpenGLRendered();
		static bool isHardwareAccelerated();
		
	};

    class Color
    {
        public: enum
        {
            BLACK, WHITE, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW,
            GREY,  SILVER, MAROON, LIME, NAVY, TEAL, PINK, ORANGE,
            BEIGE , BROWN, SCARLET, MINT, AZURE, TURQUOISE, VIOLET, GOLD,
            SALMON, BRONZE, WINE, OLIVE, INDIGO, CELESTE, PURPLE, FLAME,
            CREAM, CARAMEL, RUBY, JADE, CERULEAN,  AQUA , FUSCHIA, PUMPKIN
        };
    };

};

class SDLTexture extends public Texture
{
	private:
	/* Surface containing the image.
	*/
	SDL_Surface* surface;

    public:

	/* Constructor
	 * Loads the image file using the "filename" String
	*/
    SDLTexture(const String filename);
	
	/* Constructor
     * Creates a new SDLTexture, with a true copy of the image from this Texture.
	 * If this Texture is not a SDLTexture, this constructor throws an Exception.
	 * This is it because Texture is an abstract class, and some troll could pass an Direct3DTexture, which this constructor can't handle.
	*/
	SDLTexture(const Texture& believedToBeSDLTexture);

    /* Constructor
     * Creates a SDLTexture from this SDL_Surface
    */
    SDLTexture(SDL_Surface* surf);

    //destructor
    ~SDLTexture();

    /*
      Assignment operators
      Basically just tries to make a shallow copy (does not make a copy of the surfaces).
    */
    Texture& operator = (const Texture& texture);
    SDLTexture& operator = (const SDLTexture& sdltexture);

	//classes to draw on the screen
	
	/* Render this whole Texture in the game screen, from left-up to right-down. This is equivalent to call "render(0,0)"
	 */
	void render();
	
	/* Render this whole Texture in the game screen, positioning the texture in the given coordinates.
	 */
	void render(int x, int y);
	
	/* Render an area of this Texture in the game screen, positioning the texture in the given coordinates.
	 * The area selected is selected following the given parameters.
	 * from_x and from_y specifies the position, w and h specifies the size of the selected area.
	 */
	void render(int x, int y, int from_x, int from_y, int w, int h);
	
	/* Render this whole Texture in Texture tex2, from left-up to right-down. This is equivalent to call "paste(0,0)"
	 * The passed Texture should be a SDLTexture, or an Exception will be thrown.
	 */
	void paste(const Texture& tex2);
	
	/* Render this whole Texture in Texture tex2, positioning the texture in the given coordinates.
	 * The passed Texture should be a SDLTexture, or an Exception will be thrown.
	 */
	void paste(const Texture& tex2, int x, int y);
	
	/* Render an area of this Texture in Texture tex2, positioning the texture in the given coordinates.
	 * The area selected is selected following the given parameters.
	 * from_x and from_y specifies the position, w and h specifies the size of the selected area.
	 * The passed Texture should be a SDLTexture, or an Exception will be thrown.
	 */
	void paste(const Texture& tex2, int x, int y, int from_x, int from_y, int w, int h);

    /*
      Fill this entire surface with the given color
    */
    void fillWithColor(const int color_flag);
};

class SDLFont
{
    enum { DEFAULT_FONT_SIZE=14 };

    TTF_Font* font;
    int fontSize, outline;
    String fontFileName;

    public:

    enum { LATIN1, UTF8 };
    enum { SOLID, SHADED, BLENDED };

    //kinda auto-explicative
    bool isBold, isItalic, isUnderlined;

    //Controls the type of the text encoding to parse from the String.
    //Use flags LATIN1 and UFT8
    int encoding;

    //Controls how this SDLFont render the text.
    //SOLID is the faster but the least good-looking. Just renders the text.
    //BLENDED is the slowest, but the best-looking. It renders the text and smooth the pixels with alpha channel.
    //SHADED is the medium term. It renders the text and smooth the pixels with the backgroundColor. Currently only black.
    int renderingMode;

    //Get the font size set for this SDLFont
    int getFontSize();

    //Set the font size set for this SDLFont. This is slower than the getter method.
    void setFontSize(int size);

    /*Get the maximum pixel height of all glyphs of the loaded font.
      You may use this height for rendering text as close together vertically as possible, though adding at least one pixel height to it will space it so they can't touch.
      Remember that SDL_ttf doesn't handle multiline printing, so you are responsible for line spacing. */
    int getMaximumPixelHeight();

    /*Get the recommended pixel height of a rendered line of text of the loaded font.
      This is usually larger than the height of the font.
    */
    int getLineSkip();

    /*Contructs a SDLFont from a TrueType font (*.ttf) with the given path
      This constructor assumes default size font 14  */
    SDLFont(const String& filename);

    //Destructor
    ~SDLFont();

    //Creates a SDLTexture with the image generated by drawing a text specified by the given String
    SDLTexture renderTexture(const String& filename, SDL_Color color);
};

#endif // SDL_HPP_INCLUDED
