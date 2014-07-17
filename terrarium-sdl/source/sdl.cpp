#include "sdl.hpp"

//Static variables needs to be declarated when used. Herp derp...
SDL_Surface* SDL::screen;
int SDL::cycleFrequency;
bool SDL::Window::hardwareStored;
bool SDL::Window::fullscreen;
bool SDL::Window::doubleBuffered;
bool SDL::Window::resizable;
bool SDL::Window::framed;
bool SDL::Window::asynchronousBlitted;
bool SDL::Window::hardwarePalette;
bool SDL::Window::anyPixelFormat;
bool SDL::Window::openGLRendered;
bool SDL::Window::hardwareAccelerated;

//SDL

void SDL::initialize()
{
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        COUT << "Nao foi possivel inicializar a SDL: " << SDL_GetError() << ENDL;
        throw Exception("FATAL ERROR: SDL could not be initialized!");
    }

	SDL::Window::setAnyPixelFormat(false);
	SDL::Window::setAsynchronousBlitted(false);
	SDL::Window::setDoubleBuffered(true);
	SDL::Window::setFramed(true);
	SDL::Window::setFullscreen(false);
	SDL::Window::setHardwareAccelerated(false);
	SDL::Window::setHardwarePalette(false);
	SDL::Window::setHardwareStored(true);
	SDL::Window::setOpenGLRendered(false);
	SDL::Window::setResizable(false);

    SDL::cycleFrequency=32;

	//inicializa o som
	if( Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096)== -1)
	{
		COUT << "Erro ao abrir o audio. Mix_error: " << Mix_GetError() << ENDL;
		throw Exception(Mix_GetError());
	}

	//inicializar o sistema de TTF
	if( TTF_Init()== -1)
	{
		std::cout << "Nao foi possivel inicializar a SDL TTF: " << TTF_GetError() << std::endl;
		throw Exception(TTF_GetError());
	}

	//makes the exit() call clean everything
	atexit(SDL::finalize);

}

void SDL::finalize()
{
    // while(Mix_Init(0)) Mix_Quit();
    SDL_Quit();
}

Uint32 SDL::getTimeSinceSDLInitialization()
{
    return SDL_GetTicks();
}

void SDL::rest(const int& ms)
{
	SDL_Delay(ms>0? ms : 1);
}

void SDL::rest()
{
    SDL_Delay( 1000/SDL::cycleFrequency - ( SDL_GetTicks()% (1000/SDL::cycleFrequency)) );
}

SDL_Surface* SDL::loadImage(const String filename)
{
    SDL_Surface* surf;
    if( (surf = IMG_Load(filename.c_str() ))==NULL)
        throw Exception("ERROR: could not load image from file \"%s\". %s", filename.c_str(), IMG_GetError());

    return surf;
}

Mix_Chunk* SDL::loadSound(const String filename)
{
    Mix_Chunk* sound;

    if( (sound =  Mix_LoadWAV(filename.c_str()) )==NULL)
        throw Exception("ERROR: could not load sound from file \"%s\". %s", filename.c_str(), Mix_GetError());

    return sound;
}

Mix_Music* SDL::loadSong(const String filename)
{
    Mix_Music* music;

    if( (music =  Mix_LoadMUS(filename.c_str()) )==NULL)
        throw Exception("ERROR: could not load music from file\"%s\". %s", filename.c_str(), Mix_GetError());

    return music;
}

TTF_Font* SDL::loadFont(const String filename, int size)
{
    TTF_Font* font;
    if( TTF_WasInit() )
    {
        if( ( font = TTF_OpenFont(filename.c_str(), size ) ) == NULL )
            throw Exception("ERROR: could not load font %s. %s", filename.c_str(), TTF_GetError());
    }
    else throw Exception("FATAL ERROR: attempt to use a SDL_ttf function before initializing TTF!");
    return font;
}

//SDL::Window

void SDL::Window::clean()
{
	if ( SDL_FillRect(SDL::screen, NULL, 0) == -1 )
	{
		String msg("SDL_FillRect error! ");
		throw Exception(msg += SDL_GetError() );
	}
}

void SDL::Window::refresh()
{
	if( isOpenGLRendered() )
	{
		//im not sure if this only is enough
		SDL_GL_SwapBuffers();
	}
	else
	{
		//flip surface if double-buffered, update rect if single-buffered instead
		if( SDL_Flip(SDL::screen) != 0 )
		{
			COUT << "Failed to swap the buffers: " << SDL_GetError() << ENDL;
			throw Exception(SDL_GetError());
		}
	}
}

void SDL::Window::setCaption(const String& title, const String& iconified_title)
{
	SDL_WM_SetCaption(title.c_str(), iconified_title.c_str());
}

void SDL::Window::setIcon(const String& filename)
{
	SDL_WM_SetIcon(SDL::loadImage(filename), NULL); // the loadImage may throw an Exception...
}

void SDL::Window::create(int width, int height)
{
	SDL::Window::createCheckingFlags(width, height, 0);

	/*
	//not needed, actually does not behave 100% correctly, because some flags may be set before creating window, not matching the default flags...
	if( ( SDL::screen = SDL_SetVideoMode(width, height, 0, SDL_HWSURFACE|SDL_DOUBLEBUF ) ) == NULL)
	{
		COUT << "Unable to set video.\n" << SDL_GetError() << ENDL;
		throw Exception(SDL_GetError());
	}
	 */
}

void SDL::Window::create(int width, int height, int quality)
{
	SDL::Window::createCheckingFlags(width, height, quality);
}

void SDL::Window::create(int width, int height, int quality, int flags)
{
	if( ( SDL::screen = SDL_SetVideoMode(width, height, quality, flags ) ) == NULL)
	{
		COUT << "Unable to set video.\n" << SDL_GetError() << ENDL;
		throw Exception(SDL_GetError());
	}
	SDL::Window::framed					= ((flags & SDL_NOFRAME) 	== 0) ? true : false;
	SDL::Window::anyPixelFormat 		= ((flags & SDL_ANYFORMAT) 	!= 0) ? true : false;
	SDL::Window::asynchronousBlitted 	= ((flags & SDL_ASYNCBLIT) 	!= 0) ? true : false;
	SDL::Window::doubleBuffered			= ((flags & SDL_DOUBLEBUF) 	!= 0) ? true : false;
	SDL::Window::fullscreen				= ((flags & SDL_FULLSCREEN)	!= 0) ? true : false;
	SDL::Window::hardwarePalette		= ((flags & SDL_HWPALETTE) 	!= 0) ? true : false;
	SDL::Window::hardwareStored			= ((flags & SDL_HWSURFACE) 	!= 0) ? true : false;
	SDL::Window::resizable				= ((flags & SDL_RESIZABLE) 	!= 0) ? true : false;
	SDL::Window::openGLRendered			= ((flags & SDL_OPENGL) 	!= 0) ? true : false;
	SDL::Window::hardwareAccelerated	= ((flags & SDL_HWACCEL)    != 0) ? true : false;
}

void SDL::Window::createCheckingFlags(int width, int height, int quality)
{
	Uint32 flags = 0;
	if(SDL::Window::anyPixelFormat) flags |= SDL_ANYFORMAT;
	if(SDL::Window::asynchronousBlitted) flags |= SDL_ASYNCBLIT;
	if(SDL::Window::doubleBuffered) flags |= SDL_DOUBLEBUF;
	if(!SDL::Window::framed) flags |= SDL_NOFRAME;
	if(SDL::Window::fullscreen) flags |= SDL_FULLSCREEN;
	if(SDL::Window::hardwarePalette) flags |= SDL_HWPALETTE;
	if(SDL::Window::hardwareStored) flags |= SDL_HWSURFACE;
	if(SDL::Window::resizable) flags |= SDL_RESIZABLE;
	if(SDL::Window::openGLRendered) flags |= SDL_OPENGL;

	//experimental, not sure if works...
	if(SDL::Window::hardwareAccelerated) flags |= SDL_HWACCEL;

	if( ( SDL::screen = SDL_SetVideoMode(width, height, quality, flags ) ) == NULL)
	{
		COUT << "Unable to set video.\n" << SDL_GetError() << ENDL;
		throw Exception(SDL_GetError());
	}
}


void SDL::Window::setAnyPixelFormat(bool my_anyPixelFormat)
{
	SDL::Window::anyPixelFormat = my_anyPixelFormat;
	//we need to reset the screen when its already running
	if(SDL::screen != NULL) SDL::Window::createCheckingFlags(SDL::screen->w, SDL::screen->h, SDL::screen->format->BitsPerPixel);
}
void SDL::Window::setAsynchronousBlitted(bool my_asynchronousBlitted) {

	SDL::Window::asynchronousBlitted = my_asynchronousBlitted;
	//we need to reset the screen when its already running
	if(SDL::screen != NULL) SDL::Window::createCheckingFlags(SDL::screen->w, SDL::screen->h, SDL::screen->format->BitsPerPixel);

}
void SDL::Window::setDoubleBuffered(bool my_doubleBuffered) {

	SDL::Window::doubleBuffered = my_doubleBuffered;
	//we need to reset the screen when its already running
	if(SDL::screen != NULL) SDL::Window::createCheckingFlags(SDL::screen->w, SDL::screen->h, SDL::screen->format->BitsPerPixel);

}
void SDL::Window::setFramed(bool my_framed) {

	SDL::Window::framed = my_framed;
	//we need to reset the screen when its already running
	if(SDL::screen != NULL) SDL::Window::createCheckingFlags(SDL::screen->w, SDL::screen->h, SDL::screen->format->BitsPerPixel);

}
void SDL::Window::setFullscreen(bool my_fullscreen) {

	SDL::Window::fullscreen = my_fullscreen;
	//we need to reset the screen when its already running
	if(SDL::screen != NULL) SDL::Window::createCheckingFlags(SDL::screen->w, SDL::screen->h, SDL::screen->format->BitsPerPixel);

}
void SDL::Window::setHardwarePalette(bool my_hardwarePalette) {

	SDL::Window::hardwarePalette = my_hardwarePalette;
	//we need to reset the screen when its already running
	if(SDL::screen != NULL) SDL::Window::createCheckingFlags(SDL::screen->w, SDL::screen->h, SDL::screen->format->BitsPerPixel);

}
void SDL::Window::setHardwareStored(bool my_hardwareStored) {

	SDL::Window::hardwareStored = my_hardwareStored;
	//we need to reset the screen when its already running
	if(SDL::screen != NULL) SDL::Window::createCheckingFlags(SDL::screen->w, SDL::screen->h, SDL::screen->format->BitsPerPixel);

}
void SDL::Window::setResizable(bool my_resizable) {

	SDL::Window::resizable = my_resizable;
	//we need to reset the screen when its already running
	if(SDL::screen != NULL) SDL::Window::createCheckingFlags(SDL::screen->w, SDL::screen->h, SDL::screen->format->BitsPerPixel);

}

void SDL::Window::setOpenGLRendered(bool my_openGLRendered) {

	SDL::Window::openGLRendered = my_openGLRendered;
	//we need to reset the screen when its already running
	if(SDL::screen != NULL) SDL::Window::createCheckingFlags(SDL::screen->w, SDL::screen->h, SDL::screen->format->BitsPerPixel);

}

void SDL::Window::setHardwareAccelerated(bool my_flag) {

	SDL::Window::hardwareAccelerated = my_flag;
	//we need to reset the screen when its already running
	if(SDL::screen != NULL) SDL::Window::createCheckingFlags(SDL::screen->w, SDL::screen->h, SDL::screen->format->BitsPerPixel);

}

bool SDL::Window::isAnyPixelFormat(){

	return SDL::Window::anyPixelFormat;

}
bool SDL::Window::isAsynchronousBlitted(){

	return SDL::Window::asynchronousBlitted;

}
bool SDL::Window::isDoubleBuffered(){

	return SDL::Window::doubleBuffered;

}
bool SDL::Window::isFramed(){

	return SDL::Window::framed;

}
bool SDL::Window::isFullscreen(){

	return SDL::Window::fullscreen;

}
bool SDL::Window::isHardwarePalette(){

	return SDL::Window::hardwarePalette;

}
bool SDL::Window::isHardwareStored(){

	return SDL::Window::hardwareStored;

}
bool SDL::Window::isResizable(){

	return SDL::Window::resizable;

}

bool SDL::Window::isOpenGLRendered(){

	return SDL::Window::openGLRendered;

}

bool SDL::Window::isHardwareAccelerated(){

	return SDL::Window::hardwareAccelerated;

}

//SDLTexture

SDLTexture::SDLTexture(const Texture& believedToBeSDLTexture):
Texture("SDL_TEXTURE")
{
    if(believedToBeSDLTexture.texture_type.compare("SDL_TEXTURE") == 0)
    {
        SDLTexture& tex = (SDLTexture&) believedToBeSDLTexture;

        // allocate a new copy of surface from tex
        SDL_Surface* new_surf = SDL_ConvertSurface(tex.surface, tex.surface->format, tex.surface->flags);
        if(new_surf == NULL) throw Exception("ALLOCATION ERROR: COULD NOT COPY SURFACE");

        this->surface = new_surf;
    }
    else throw Exception("FATAL ERROR: TRYING TO CREATE A SDLTEXTURE FROM A INCOMPATIBLE TEXTURE: %s", believedToBeSDLTexture.texture_type.c_str());
}

SDLTexture::SDLTexture(const String filename) :
Texture("SDL_TEXTURE"), //needed to initialize this attribute here, because it is const ¬¬
surface(SDL::loadImage(filename))
{}

SDLTexture::SDLTexture(SDL_Surface* surf) :
Texture("SDL_TEXTURE"), //needed to initialize this attribute here, because it is const ¬¬
surface(surf)
{}

SDLTexture::~SDLTexture()
{
	//its safe to pass a NULL pointer
	SDL_FreeSurface(surface);
}

Texture& SDLTexture::operator = (const Texture& texture)
{
    //check if texture matches type
    if(texture.texture_type.compare("SDL_TEXTURE") == 0)
    {
        //calls the SDLTexture assignment operator
        (*this) = (SDLTexture) texture;
    }
    else throw Exception("FATAL ERROR: TRYING TO ASSIGN INCOMPATIBLE TEXTURES: SDLTexture from %s",texture.texture_type.c_str());
    return *this;
}

SDLTexture& SDLTexture::operator = (const SDLTexture& sdltexture)
{
    if(this != &sdltexture) // protect against invalid self-assignment
    {
        //release old memory
        SDL_FreeSurface(surface);

        //points to the same memory as texture (shallow copy)
        this->surface = sdltexture.surface;
    }
    return *this;
}

void SDLTexture::render()
{
	SDL_BlitSurface(surface, NULL, SDL::screen,NULL);
}

void SDLTexture::render(int x, int y)
{
	SDL_Rect temp_rect = {x, y, 0, 0};
	SDL_BlitSurface(surface, NULL, SDL::screen, &temp_rect);
}

void SDLTexture::render(int x, int y, int from_x, int from_y, int w, int h)
{
	SDL_Rect temp_rect = {x, y, 0, 0};
	SDL_Rect from_rect = {from_x, from_y, w, h};
	SDL_BlitSurface(surface, &from_rect, SDL::screen, &temp_rect);
}

void SDLTexture::paste(const Texture& tex2)
{
	try
	{
		SDL_BlitSurface(surface, NULL, ( (SDLTexture) tex2).surface ,NULL);
	}
	catch(Exception& e)
	{
		COUT << e.message() << ENDL;
	}
}

void SDLTexture::paste(const Texture& tex2, int x, int y)
{
	SDL_Rect temp_rect = {x, y, 0, 0};
	try
	{
		SDL_BlitSurface(surface, NULL, ( (SDLTexture) tex2).surface , &temp_rect);
	}
	catch(Exception& e)
	{
		COUT << e.message() << ENDL;
	}
}
void SDLTexture::paste(const Texture& tex2, int x, int y, int from_x, int from_y, int w, int h)
{
	SDL_Rect temp_rect = {x, y, 0, 0};
	SDL_Rect from_rect = {from_x, from_y, w, h};
	try
	{
		SDL_BlitSurface(surface, &from_rect, ( (SDLTexture) tex2).surface , &temp_rect);
	}
	catch(Exception& e)
	{
		COUT << e.message() << ENDL;
	}
}

void SDLTexture::fillWithColor(const int color_flag)
{
    SDL_FillRect(this->surface, NULL, color_flag);
}

//SDLFont

SDLFont::~SDLFont()
{
    TTF_CloseFont(font);
    font=NULL;
}

SDLFont::SDLFont(const String& filename)
    : font(SDL::loadFont(filename, DEFAULT_FONT_SIZE)),
      fontSize(DEFAULT_FONT_SIZE),
      outline(0),
      fontFileName(filename),
      isBold(false),
      isItalic(false),
      isUnderlined(false),
      encoding(LATIN1),
      renderingMode(SOLID)
{}

int SDLFont::getFontSize()
{
    return fontSize;
}

void SDLFont::setFontSize(int size)
{
    fontSize = size;

    try
    {
        font = SDL::loadFont(fontFileName.c_str(), fontSize);
    }
    catch(const Exception& e)
    {
        throw Exception("ERROR: could not change font \"%s\" size. %s", fontFileName.c_str(), e.message().c_str());
    }
}

int SDLFont::getMaximumPixelHeight()
{
    return TTF_FontHeight(font);
}

int SDLFont::getLineSkip()
{
    return TTF_FontLineSkip(font);
}

SDLTexture SDLFont::renderTexture(const String& text, SDL_Color color)
{
    SDL_Surface* surface=NULL;
    int flags=TTF_STYLE_NORMAL;
    if(isBold) flags |= TTF_STYLE_BOLD;
    if(isItalic) flags |= TTF_STYLE_ITALIC;
    if(isUnderlined) flags |= TTF_STYLE_UNDERLINE;
    TTF_SetFontStyle(font, flags);

    switch(encoding)
    {
        case UTF8:
        {
            switch(renderingMode)
            {
                case SOLID:
                {
                    if((surface = TTF_RenderUTF8_Solid(font, text.c_str(), color))==NULL)
                        throw Exception("ERROR: COULD NOT RENDER A SDLTEXTURE FROM A TTF_RENDER CALL! %s.", TTF_GetError());
                    break;
                }
                case SHADED:
                {
                    SDL_Color bg; bg.b = bg.g = bg.r = 0;
                    if((surface = TTF_RenderUTF8_Shaded(font, text.c_str(), color, bg))==NULL)
                        throw Exception("ERROR: COULD NOT RENDER A SDLTEXTURE FROM A TTF_RENDER CALL! %s.", TTF_GetError());
                    break;
                }
                case BLENDED:
                {
                    if((surface = TTF_RenderUTF8_Blended(font, text.c_str(), color))==NULL)
                        throw Exception("ERROR: COULD NOT RENDER A SDLTEXTURE FROM A TTF_RENDER CALL! %s.", TTF_GetError());
                    break;
                }
            }
            break;
        }
        case LATIN1:
        default:
        {
            switch(renderingMode)
            {
                case SOLID:
                {
                    if((surface = TTF_RenderText_Solid(font, text.c_str(), color))==NULL)
                        throw Exception("ERROR: COULD NOT RENDER A SDLTEXTURE FROM A TTF_RENDER CALL! %s.", TTF_GetError());
                    break;
                }
                case SHADED:
                {
                    SDL_Color bg; bg.b = bg.g = bg.r = 0;
                    if((surface = TTF_RenderText_Shaded(font, text.c_str(), color, bg))==NULL)
                        throw Exception("ERROR: COULD NOT RENDER A SDLTEXTURE FROM A TTF_RENDER CALL! %s.", TTF_GetError());
                    break;
                }
                case BLENDED:
                default:
                {
                    if((surface = TTF_RenderText_Blended(font, text.c_str(), color))==NULL)
                        throw Exception("ERROR: COULD NOT RENDER A SDLTEXTURE FROM A TTF_RENDER CALL! %s.", TTF_GetError());
                    break;
                }
            }
            break;
        }
    }
    if(surface != NULL)
    {
        return *(new SDLTexture(surface));
    }
    else throw Exception("ERROR: COULD NOT RENDER A SDLTEXTURE FROM A TTF_RENDER CALL! UNKNOWN ERROR");
}

int SDL::getColorNumber(int color_flag)
{
    SDL_Color color = SDL::getColor(color_flag);
    return SDL_MapRGB(SDL::screen->format, color.r, color.g, color.b);
}

SDL_Color SDL::getColor(int color_flag)
{
    SDL_Color color;
    switch(color_flag){

    case Color::BLACK:
        color.r=color.g=color.b=0;
        break;

    case Color::AQUA:
        color.r=176;
        color.g=224;
        color.b=230;
        break;

    case Color::AZURE:
        color.r=0;
        color.g=127;
        color.b=255;
        break;

    case Color::BEIGE:
        color.r=245;
        color.g=245;
        color.b=220;
        break;

    case Color::BLUE:
        color.r=0;
        color.g=0;
        color.b=255;
        break;

    case Color::BRONZE:
        color.r=205;
        color.g=127;
        color.b=50;
        break;

    case Color::BROWN:
        color.r=150;
        color.g=75;
        color.b=0;
        break;

    case Color::CARAMEL:
        color.r=193;
        color.g=154;
        color.b=107;
        break;

    case Color::CELESTE:
        color.r=0;
        color.g=191;
        color.b=255;
        break;

    case Color::CERULEAN:
        color.r=0;
        color.g=123;
        color.b=167;
        break;

    case Color::CREAM:
        color.r=253;
        color.g=252;
        color.b=143;
        break;

    case Color::CYAN:
        color.r=0;
        color.g=255;
        color.b=255;
        break;

    case Color::FLAME:
        color.r=226;
        color.g=88;
        color.b=34;
        break;

    case Color::FUSCHIA:
        color.r=193;
        color.g=84;
        color.b=193;
        break;

    case Color::GOLD:
        color.r=255;
        color.g=215;
        color.b=0;
        break;

    case Color::GREEN:
        color.r=0;
        color.g=255;
        color.b=0;
        break;

    case Color::GREY:
        color.r=127;
        color.g=127;
        color.b=127;
        break;

    case Color::INDIGO:
        color.r=75;
        color.g=0;
        color.b=130;
        break;

    case Color::JADE:
        color.r=0;
        color.g=127;
        color.b=0;
        break;

    case Color::LIME:
        color.r=191;
        color.g=255;
        color.b=0;
        break;

    case Color::MAGENTA:
        color.r=255;
        color.g=0;
        color.b=255;
        break;

    case Color::MAROON:
        color.r=128;
        color.g=0;
        color.b=0;
        break;

    case Color::MINT:
        color.r=116;
        color.g=195;
        color.b=101;
        break;

    case Color::NAVY:
        color.r=0;
        color.g=0;
        color.b=127;
        break;

    case Color::OLIVE:
        color.r=127;
        color.g=127;
        color.b=0;
        break;

    case Color::ORANGE:
        color.r=255;
        color.g=165;
        color.b=0;
        break;

    case Color::PINK:
        color.r=255;
        color.g=192;
        color.b=203;
        break;

    case Color::PUMPKIN:
        color.r=255;
        color.g=127;
        color.b=0;
        break;

    case Color::PURPLE:
        color.r=127;
        color.g=0;
        color.b=127;
        break;

    case Color::RED:
        color.r=255;
        color.g=0;
        color.b=0;
        break;

    case Color::RUBY:
        color.r=224;
        color.g=17;
        color.b=95;
        break;

    case Color::SALMON:
        color.r=250;
        color.g=127;
        color.b=114;
        break;

    case Color::SCARLET:
        color.r=253;
        color.g=14;
        color.b=53;
        break;

    case Color::SILVER:
        color.r=192;
        color.g=192;
        color.b=192;
        break;

    case Color::TEAL:
        color.r=0;
        color.g=127;
        color.b=127;
        break;

    case Color::TURQUOISE:
        color.r=64;
        color.g=224;
        color.b=208;
        break;

    case Color::VIOLET:
        color.r=127;
        color.g=0;
        color.b=255;
        break;

    case Color::WINE:
        color.r=196;
        color.g=30;
        color.b=58;
        break;

    case Color::WHITE:
    default:
        color.r=color.g=color.b=255;

    }
    return color;
}

