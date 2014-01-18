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
	string current;

	//contains info about the animation, like the number of frames, interval per frame
	map<string, int> positionOf;
	map<string, int> framesOf;
	map<string, double> intervalOf;
	map<string, Dimensions> dimensionsOf;
	double timeCounter;


	public:
	AnimationSet(Image* sheet);

	void add(string tag, int width=-1, int height=-1, double interval=-1, int frames=1, bool setCurrent=true);

	void draw(float x=0, float y=0, float angle=0);

	void setCurrent(string tag);

	//TODO add methods for editing the animation set.
};


#endif /* ANIMATION_HPP_ */
