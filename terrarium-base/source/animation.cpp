/*
 * animation.cpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#include "../header/animation.hpp"

void Animation::add(String tag, int interval, int width, int height, int frames)
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
}


