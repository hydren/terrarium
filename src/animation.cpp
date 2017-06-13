/*
 * animation.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#include "animation.hpp"

AnimationSet::AnimationSet(Image* sheet) :
sheet(sheet), sprites(), currentAnim("default")
{
	sprites["default"] = new Sprite(sheet, 0, 0);
}

void AnimationSet::add(string tag, int width, int height, double interval, int frames, bool setCurrent)
{
	if(height == -1) height = sheet->getHeight();
	if(width == -1) width = sheet->getWidth();
	int sum=0;
	for( map<string, Sprite*>::iterator it = sprites.begin(); it != sprites.end(); ++it )
	{
		//gets the height of each animation and sums up
		sum += it->second->height;
	}

	sprites[tag] = new Sprite(sheet, width, height, interval, frames, 0, sum);
	if(setCurrent) this->setCurrent(tag);
}

Sprite& AnimationSet::ref(string tag)
{
	return *(sprites[tag]);
}

Sprite& AnimationSet::operator[](string tag)
{
	return *(sprites[tag]);
}

Sprite& AnimationSet::current()
{
	return *(sprites[currentAnim]);
}

void AnimationSet::setCurrent(string tag)
{
	currentAnim = tag;
}

void AnimationSet::draw(float x, float y, float angle)
{
	sprites[currentAnim]->computeCurrentFrame();
	sprites[currentAnim]->angle = angle;
	sprites[currentAnim]->draw(x, y);
}

unsigned AnimationSet::spriteCount()
{
	return sprites.size();
}
