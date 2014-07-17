#ifndef ANIMATED_ENTITY_HPP_INCLUDED
#define ANIMATED_ENTITY_HPP_INCLUDED

#include "entity.hpp"
#include "util.hpp"

#include <SDL/SDL.h>
#include <map>

class AnimatedEntity extends public Entity
{
	protected:
	
	/* Class with animations related tasks. This class uses a convention borrowed from SokoBanisher.
	 * The texture should contains all sprites needed for the animation. 
	 * They should be organized in a way that all sprites for ONE animation must be put side by side in one row, and each row corresponds to a different animation.
	 * So, in this way, you need to insert entries in the maps within the Animation class to tell the game about the sprites in the Texture.
	 * The "framesOf" map contains info about how many frames have in the respective "key" animations. 
	 * Ex: animation.framesOf[3] returns an int, the number of frames of animation #3.
	 * The "intervalOf" map contains info about how much time it takes to change the frames of the respective "key" animations.
	 * Ex: animation.intervalOf[1] return an int, which is the number of miliseconds between the frames of animation 1.
	 * The "currentIndex" is the current choosen animation. You can change it to change the current animation with setCurrent() method.
	 * The timeBasedIndex() is used in the "draw()" method to select the correct sprite to show, based on time.
	 */
	class Animation
	{
		int currentIndex;
		
		public:
		
		//contains info about the animation, like the number of frames, interval per frame
		std::map<int, int> framesOf;
		std::map<int, int> intervalOf;
		
		Animation();
		
		/* Returns a index for the current animation, based on time intervals for the current animation.
		*/
		int timeBasedIndex();
		
		int current();
		void setCurrent(int new_index);
	};
	
	public:
	
	//contains all data about the actor animations - number of frames, frame interval etc.
	Animation animation;
	
	/* Size of the sprites. This is used to selected a piece of the texture/sheet to draw. The piece will have this size. 
	 * If you want to have smaller sprites, just center-align the sprites on the image, when creating the sheet.
	 * In others words, set this size to be the size of the biggest sprite, and then align the other sprites accordingly.
	 */
	int spriteWidth, spriteHeight;
	
	AnimatedEntity(Texture& tex, int sprite_width, int sprite_height);
	
	/* Draws this Actor on the screen;
	 * */
	virtual void draw();
};


#endif // ANIMATED_ENTITY_HPP_INCLUDED