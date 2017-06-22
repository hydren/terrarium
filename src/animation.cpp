/*
 * animation.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#include "animation.hpp"

Animation::~Animation()
{
	for(unsigned i = 0; i < sprites.size(); i++)
		delete sprites[i];
}

Sprite& Animation::operator[](int index)
{
	return *sprites[index];
}

Sprite& Animation::ref(int index)
{
	return *sprites[index];
}

Sprite& Animation::current()
{
	return *sprites[currentIndex];
}

// convenience method to add a sprite to the list (it will be added to the end (back) of the list)
void Animation::add(Sprite* sprite, bool setCurrent)
{
	currentIndex = sprites.size();
	sprites.push_back(sprite);
}

// convenience method to delete a sprite in the list, given its index. if a -1 index is passed, it deletes the last sprite, if there is one.
void Animation::del(int index)
{
	if(index == -1)
		index = sprites.size()-1;

	if(index < 0 or index > -1 + (int) sprites.size())
		return;

	if(index == currentIndex)
		currentIndex--;

	delete sprites[index];
	sprites.erase(sprites.begin()+index);
}

void Animation::draw(float x, float y)
{
	sprites[currentIndex]->computeCurrentFrame();
	sprites[currentIndex]->draw(x, y);
}

//************************************************************

SingleSheetAnimation::SingleSheetAnimation(Image* sheet, bool sheetIsOwned)
: Animation(), sheet(sheet), sheetIsOwned(sheetIsOwned)
{}

SingleSheetAnimation::~SingleSheetAnimation()
{
	if(sheet != null and sheetIsOwned)
	{
		delete sheet;
		sheet = null;
	}
}

void SingleSheetAnimation::addSprite(int width, int height, int frameCount, double frameDuration, Point sheetOffset)
{
	if(height == -1) height = sheet->getHeight();
	if(width == -1) width = sheet->getWidth();
	this->add(new Sprite(sheet, width, height, frameDuration, frameCount, sheetOffset.x, sheetOffset.y, false));
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

StackedSingleSheetAnimation::StackedSingleSheetAnimation(Image* sheet, bool sheetIsOwned)
: SingleSheetAnimation(sheet, sheetIsOwned)
{}

void StackedSingleSheetAnimation::addSprite(int width, int height, int frameCount, double frameDuration, int sheetOffsetX)
{
	if(height == -1) height = sheet->getHeight();
	if(width == -1) width = sheet->getWidth();
	int sum=0;
	for(unsigned i = 0; i < sprites.size(); i++)
	{
		//gets the height of each animation and sums up
		sum += sprites[i]->height;
	}

	this->add(new Sprite(sheet, width, height, frameDuration, frameCount, sheetOffsetX, sum));
}
