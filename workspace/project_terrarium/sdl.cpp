#include "sdl.hpp"

//Static variables needs to be declarated when used. Herp derp...
SDL_Surface* SDL::screen;
int SDL::fps;
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
	
	SDL::fps=60;
	
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

void SDL::rest(const int& ms)
{
	SDL_Delay(ms>0? ms : 1);
}

void SDL::rest()
{
	SDL_Delay( 1000/SDL::fps - ( SDL_GetTicks()% (1000/SDL::fps)) );
}

SDL_Surface* SDL::loadImage(const String filename)
{
    SDL_Surface* surf;
    if( (surf = IMG_Load(filename.c_str() ))==NULL)
    {
		String msg("Nao foi possivel carregar o arquivo de imagem: ");
        throw Exception( ( msg += filename) += IMG_GetError() );
        COUT << msg << filename << " " << IMG_GetError() << ENDL;
    }
    return surf;
}

Mix_Chunk* SDL::loadSound(const String filename)
{
    Mix_Chunk* sound;
    if( (sound =  Mix_LoadWAV(filename.c_str()) )==NULL) 
	{
		String msg("Nao foi possivel carregar o arquivo de som: ");
        throw Exception( ( msg += filename) += Mix_GetError() );
        COUT << msg << filename  <<  " " << Mix_GetError() << ENDL;
    }
    return sound;
}

Mix_Music* SDL::loadSong(const String filename)
{
    Mix_Music* music;
    if( (music =  Mix_LoadMUS(filename.c_str()) )==NULL)
	{
		String msg("Nao foi possivel carregar o arquivo de musica: ");
        throw Exception( ( msg += filename)+= Mix_GetError()  );
        COUT << msg << filename << " " << Mix_GetError() << ENDL;
    }
    return music;
}

TTF_Font* SDL::loadFont(const String filename, int size)
{
    TTF_Font* fonte;

    if( (fonte = TTF_OpenFont(filename.c_str(), size))==NULL)
    {
		String msg("Nao foi possivel carregar o arquivo de imagem: ");
        throw Exception(( msg += filename )+= TTF_GetError() );
        COUT << msg << filename << " "<< TTF_GetError()<< ENDL;
    }
    return fonte;
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
		this->surface = tex.surface;
	}
	else throw Exception("FATAL ERROR: TRYING TO CAST INCOMPATIBLE TEXTURES: SDLTexture from Texture");
}

SDLTexture::~SDLTexture()
{
	//its safe to pass a NULL pointer
	SDL_FreeSurface(surface);
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
	catch(Exception e)
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
	catch(Exception e)
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
	catch(Exception e)
	{
		COUT << e.message() << ENDL;
	}
}
