/*
 * animation.hpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_

#include "fgeal/fgeal.hpp"
using fgeal::Image;
using fgeal::Point;

#include "fgeal/extra/sprite.hpp"
using fgeal::Sprite;

#include "futil/general/language.hpp"

//#include <map>
//using std::map;

#include <string>
using std::string;

#include <vector>
using std::vector;

// A class that represents a list of sprites in which one of them is set as the "current" sprite.
// This class is useful to organize all the sprites that one entity may have and control their exibition.
struct Animation
{
	// the list of sprites
	vector<Sprite*>& sprites;

	// the current sprite index
	unsigned currentIndex;

	// true if this animation is actually a flyweight and the 'sprites' vector is shared among other Animation instances.
	bool isFlyweight;

	// normal version
	Animation();

	// single sprite version
	Animation(Sprite* sprite);

	// flyweight version
	Animation(Animation* basedAnimation);

	~Animation();

	// convenience method to grab a sprite, given its index
	Sprite& operator[](unsigned index);

	// convenience method to grab a sprite, given its index
	Sprite& ref(unsigned index);

	// convenience method to grab the current sprite
	Sprite& current();

	// convenience method to add a sprite to the list (it will be added to the end (back) of the list)
	void add(Sprite* sprite, bool setCurrent=false);

	// convenience method to delete a sprite in the list, given its index.
	void del(unsigned index);

	// convenience method to draw the current sprite, at the specified coordinates
	void draw(float x=0, float y=0);
};

// This class is an animation in which all its sprites uses (and thus, shares) the same sheet image.
// All sprites created by this animation will not attempt to delete the sheet at their deletion.
// Instead, this will be done at the animation class destructor (unless 'sheetIsOwned' is set to false).
struct SingleSheetAnimation extends Animation
{
	Image* sheet;
	bool sheetIsOwned;

	SingleSheetAnimation(Image* sheet, bool sheetIsOwned=true);
	~SingleSheetAnimation();

	// creates and adds a sprite to the list, width the given size, frame count, frame duration and sheet offset.
	// this sprite will be added to the end (back) of the list.
	void addSprite(int width=-1, int height=-1, unsigned frameCount=1, double frameDuration=-1, Point sheetOffset=Point());
};

// This class is a single-sheet animation in which each sprite is stacked in a top-down manner.
// This variant assumes that there is no reuse of parts of the sheet that is already used by other sprite within the list.
struct StackedSingleSheetAnimation extends SingleSheetAnimation
{
	// Creates a single sheet animation in which each sprite is stacked in a top-down manner.
	StackedSingleSheetAnimation(Image* sheet, bool sheetIsOwned=true);

	// creates and adds a sprite to the list, width the given size, frame count, frame duration and sheet x-offset.
	// this sprites' y-sheet-offset will be inferred from the current added sprites as the sum of their heights.
	// this sprite will be added to the end (back) of the list.
	void addSprite(int width=-1, int height=-1, unsigned frameCount=1, double frameDuration=-1, int sheetOffsetX=0);
};

#endif /* ANIMATION_HPP_ */
