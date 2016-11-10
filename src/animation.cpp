/*
 * animation.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#include "animation.hpp"

AnimationSet::AnimationSet(Image* sheet) :
sheet(sheet), current("default"), positionOf(), framesOf(), intervalOf(), dimensionsOf(), timeCounter(-1)
{
	framesOf["default"] = 1;
	intervalOf["default"] = -1;
	dimensionsOf["default"].width = dimensionsOf["default"].height = 0;
}

void AnimationSet::add(string tag, int width, int height, double interval, int frames, bool setCurrent)
{
	if(height == -1) height = sheet->getHeight();
	if(width == -1) width = sheet->getWidth();
	int sum=0;
	for( map<string, Dimensions>::iterator it = dimensionsOf.begin(); it != dimensionsOf.end(); ++it )
	{
		//pega a altura de cada animação e soma
		sum += it->second.height;

	}
	positionOf[tag] = sum;
	dimensionsOf[tag].width = width;
	dimensionsOf[tag].height = height;
	intervalOf[tag] = interval;
	framesOf[tag] = frames;
	if(setCurrent) this->setCurrent(tag);
}

void AnimationSet::draw(float x, float y, float angle)
{
	if(intervalOf[current] == -1)
	{
		this->sheet->drawRotated(x, y, dimensionsOf[current].width/2, dimensionsOf[current].height/2, angle, 0, positionOf[current], dimensionsOf[current].width, dimensionsOf[current].height);
	}
	else
	{
		//COUT << al_get_time()-timeCounter << '\n'<< ((double)framesOf[current])*intervalOf[current] << ENDL;
		if(timeCounter==-1)
			timeCounter = fgeal::uptime();

		while(fgeal::uptime()-timeCounter >= ((double)framesOf[current])*intervalOf[current])
			timeCounter += ((double)framesOf[current])*intervalOf[current];

		this->sheet->drawRotated(x, y, dimensionsOf[current].width/2, dimensionsOf[current].height/2, angle, (int)((fgeal::uptime()-timeCounter)/intervalOf[current])*dimensionsOf[current].width, positionOf[current], dimensionsOf[current].width, dimensionsOf[current].height);
	}
}

void AnimationSet::draw()
{ draw(sheet->getWidth()/2, sheet->getHeight()/2); }


void AnimationSet::setCurrent(string tag)
{
	current = tag;
}


