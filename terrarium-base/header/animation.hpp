/*
 * animation.hpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_

#include "util.hpp"
#include "image.hpp"

struct Dimensions
{
	int width, height;
};

struct Animation
{
	private:
	Image* sheet;
	String current;

	//contains info about the animation, like the number of frames, interval per frame
	map<String, int> positionOf;
	map<String, int> framesOf;
	map<String, int> intervalOf;
	map<String, Dimensions> dimensionsOf;


	public:
	Animation(Image* sheet) :
	sheet(sheet), current("default"), positionOf(), framesOf(), intervalOf(), dimensionsOf()
	{
		framesOf["default"] = 1;
		intervalOf["default"] = -1;
		dimensionsOf["default"].width = dimensionsOf["default"].height = 0;
	}

	void add(String tag, int interval, int width, int height, int frames);

};


#endif /* ANIMATION_HPP_ */
