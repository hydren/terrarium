/*
 * animation.hpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#ifndef ANIMATION_HPP_
#define ANIMATION_HPP_

#include "fgeal.hpp"
using fgeal::Image;

#include "sprite.hpp"
using fgeal::Sprite;

#include <map>
using std::map;

#include <string>
using std::string;

struct AnimationSet
{
	private:
	Image* sheet;
	map<string, Sprite*> sprites;
	string current;

	public:
	AnimationSet(Image* sheet);

	void add(string tag, int width=-1, int height=-1, double interval=-1, int frames=1, bool setCurrent=true);

	void draw(float x=0, float y=0, float angle=0);

	void setCurrent(string tag);

	int getCurrentWidth();
	int getCurrentHeight();

	//TODO add methods for editing the animation set.
};


#endif /* ANIMATION_HPP_ */
