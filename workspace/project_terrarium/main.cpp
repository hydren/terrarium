#include "main.hpp"
#include "actor.hpp"
#include "block.hpp"

MACETE_ESCROTO_1_1;

int main(int argc, char **argv)
{	
	// initialize SDL
	SDL::initialize();
	MACETE_ESCROTO_1_2;
	
	SDL::fps = 30;

	SDL::Window::create(640, 480, 16);

	int x_vel = 0; int y_vel = 0;
	
	SDLTexture tex("../images/gb_walk.png");
	SDLTexture tex2("../images/lava.png");

	Actor player(tex, 100, 150, 104, 154);
	
	player.animation.framesOf[0] = 6;
	player.animation.framesOf[1] = 6;
	player.animation.framesOf[2] = 2;
	player.animation.intervalOf[0] = 150;
	player.animation.intervalOf[1] = 150;
	player.animation.intervalOf[2] = 300;
	
	player.animation.setCurrent(2);
	
	player.spriteWidth = 104;
	player.spriteHeight = 154;
	
	Block floor(tex2, 0, 0, 96, 96);
	floor.animation.framesOf[0] = 1;
	floor.animation.intervalOf[0] = 1000;
	floor.animation.setCurrent(0);
	
	floor.animation.framesOf[0] = 4;
	floor.animation.intervalOf[0] = 500;
	
	SDL_Event event;

	while(true) {
		while(SDL_PollEvent(&event)){
		switch (event.type) {
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_UP:
						//y_vel = -4;
						player.y_velocity -=48;
						break;
					case SDLK_DOWN:
						player.y_velocity +=16;
						break;
					case SDLK_LEFT:
						player.x_accel -=16;
						player.animation.setCurrent(1);
						break;
					case SDLK_RIGHT:
						player.x_accel +=16;
						player.animation.setCurrent(0);
						break;
					case SDLK_ESCAPE:
						exit(0);
					default:
						break;
				}
				printf("The %s key was pressed!\n", SDL_GetKeyName(event.key.keysym.sym));
				break;
			case SDL_KEYUP:
				switch(event.key.keysym.sym) {
					case SDLK_UP:
					case SDLK_DOWN:
						//nada
						break;
					case SDLK_LEFT:
					case SDLK_RIGHT:
						player.x_accel = 0;
						break;
					default:
						break;
				}
				printf("The %s key was released!\n", SDL_GetKeyName(event.key.keysym.sym));
				player.animation.setCurrent(2);
				break;
			case SDL_QUIT:
				exit(0);
			default:
				break;
			}
		}
		
		//atualizar posicao e checar colisao
		
		if(player.x_accel == 0) player.x_velocity *=0.9;
		else if(!((player.x_accel < 0 && player.x_velocity <= -MAX_SPEED) || (player.x_accel > 0 && player.x_velocity >= MAX_SPEED)))
		{
			player.x_velocity += player.x_accel;
			if(player.x_velocity > MAX_SPEED) player.x_velocity = MAX_SPEED;
			else if(player.x_velocity < -MAX_SPEED) player.x_velocity = -MAX_SPEED;
		}
	
		if(player.y_accel == 0) player.y_velocity *=0.9;
		else if(!((player.y_accel < 0 && player.y_velocity <= -MAX_SPEED) || (player.y_accel > 0 && player.y_velocity >= MAX_SPEED)))
		{
			player.y_velocity += player.y_accel;
			if(player.y_velocity > MAX_SPEED) player.y_velocity = MAX_SPEED;
			else if(player.y_velocity < -MAX_SPEED) player.y_velocity = -MAX_SPEED;
		}
	
		player.bounds.x += player.x_velocity;
		player.bounds.y += player.y_velocity;
		
		SDL::Window::clean();

		player.draw();
		floor.draw();

		SDL::Window::refresh();
		
		SDL::rest();
	
	}

	SDL::finalize();
	MACETE_ESCROTO_1_3;
	return 0;
}
