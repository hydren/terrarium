#include "animated_entity.hpp"

//lol
AnimatedEntity::Animation::Animation()
{
	currentIndex = 0;
	framesOf[0] = 1;
	intervalOf[0] = 1000;
}

void AnimatedEntity::Animation::setCurrent(int new_index)
{
	//if animation exists
	if(framesOf.find(new_index) != framesOf.end())
	{
		currentIndex = new_index;
	}
	else //if it does not exist, set to zero
	{
		currentIndex = 0;
		throw Exception("WARNING: setting current animation to be an absent one. Zero setted instead.");
	}
}

int AnimatedEntity::Animation::current()
{
	return currentIndex;
}

int AnimatedEntity::Animation::timeBasedIndex()
{
	/* Returns a index varying from 0 to the number of frames given.
	 * The returned index is time bases, using the current animation interval.
	 * With this, the rendered sprite changes every interval, which is an animation.
	 * */
	return ( SDL_GetTicks()/intervalOf[currentIndex]) % framesOf[currentIndex];
}

//o construtor vai inicializar a variavel animation automaticamente, usando o construtor definido aqui em cima
AnimatedEntity::AnimatedEntity(Texture* tex, int sprite_width, int sprite_height) :
Entity(tex),
spriteWidth(sprite_width),
spriteHeight(sprite_height)
{}

void AnimatedEntity::draw()
{
    texture->render(0, 0, spriteWidth * animation.timeBasedIndex(), spriteHeight * animation.current(), spriteWidth, spriteHeight);
}
