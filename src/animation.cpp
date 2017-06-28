/*
 * animation.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#include "animation.hpp"

// not necessary (already in header), but included to settle a IDE indexing bug
using fgeal::Sprite;
using std::vector;

Animation::Animation()
: sprites(*new vector<Sprite*>), currentIndex(0), isFlyweight(false)
{}

Animation::Animation(Animation* flyweight)
: sprites(flyweight->sprites), currentIndex(0), isFlyweight(true)
{}

Animation::~Animation()
{
	if(not isFlyweight)
	{
		// delete sprites
		for(unsigned i = 0; i < sprites.size(); i++)
			delete sprites[i];

		// delete vector (since we own it)
		delete &sprites;
	}
}

Sprite& Animation::operator[](unsigned index)
{
	return *sprites[index];
}

Sprite& Animation::ref(unsigned index)
{
	return *sprites[index];
}

Sprite& Animation::current()
{
	return *sprites[currentIndex];
}

void Animation::add(Sprite* sprite, bool setCurrent)
{
	if(setCurrent)
		currentIndex = sprites.size();
	sprites.push_back(sprite);
}

void Animation::del(unsigned index)
{
	if(index + 1 > sprites.size())
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

void SingleSheetAnimation::addSprite(int width, int height, unsigned frameCount, double frameDuration, Point sheetOffset)
{
	if(height == -1) height = sheet->getHeight();
	if(width == -1) width = sheet->getWidth();
	this->add(new Sprite(sheet, width, height, frameDuration, frameCount, sheetOffset.x, sheetOffset.y, false));
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

StackedSingleSheetAnimation::StackedSingleSheetAnimation(Image* sheet, bool sheetIsOwned)
: SingleSheetAnimation(sheet, sheetIsOwned)
{}

void StackedSingleSheetAnimation::addSprite(int width, int height, unsigned frameCount, double frameDuration, int sheetOffsetX)
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
