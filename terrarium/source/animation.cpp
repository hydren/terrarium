/*
 * animation.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#include "../header/animation.hpp"

#include <allegro5/allegro.h>

AnimationSet::AnimationSet(Image* sheet) :
sheet(sheet), current("default"), positionOf(), framesOf(), intervalOf(), dimensionsOf(), timeCounter(-1)
{
	framesOf["default"] = 1;
	intervalOf["default"] = -1;
	dimensionsOf["default"].width = dimensionsOf["default"].height = 0;
}

void AnimationSet::add(String tag, int width, int height, double interval, int frames, bool setCurrent)
{
		int sum=0;
		for( map<String, Dimensions>::iterator it = dimensionsOf.begin(); it != dimensionsOf.end(); ++it )
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
		this->sheet->draw_rotated(x, y, dimensionsOf[current].width/2, dimensionsOf[current].height/2, angle, 0, positionOf[current], dimensionsOf[current].width, dimensionsOf[current].height);
	}
	else
	{
		//COUT << al_get_time()-timeCounter << '\n'<< ((double)framesOf[current])*intervalOf[current] << ENDL;
		if(timeCounter==-1)
			timeCounter = al_get_time();

		while(al_get_time()-timeCounter >= ((double)framesOf[current])*intervalOf[current])
			timeCounter += ((double)framesOf[current])*intervalOf[current];

		this->sheet->draw_rotated(x, y, dimensionsOf[current].width/2, dimensionsOf[current].height/2, angle, (int)((al_get_time()-timeCounter)/intervalOf[current])*dimensionsOf[current].width, positionOf[current], dimensionsOf[current].width, dimensionsOf[current].height);
	}
}

void AnimationSet::setCurrent(String tag)
{
	current = tag;
}


