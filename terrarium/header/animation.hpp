/*
 * animation.hpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_

#include "util.hpp"
#include "engine.hpp"

using Engine::Image;

struct Dimensions
{
	int width, height;
};

struct AnimationSet
{
	private:
	Image* sheet;
	String current;

	//contains info about the animation, like the number of frames, interval per frame
	map<String, int> positionOf;
	map<String, int> framesOf;
	map<String, double> intervalOf;
	map<String, Dimensions> dimensionsOf;
	double timeCounter;


	public:
	AnimationSet(Image* sheet);

	void add(String tag, int width=-1, int height=-1, double interval=-1, int frames=1, bool setCurrent=true);

	void draw(float x=0, float y=0, float angle=0);

	void setCurrent(String tag);
};


#endif /* ANIMATION_HPP_ */
