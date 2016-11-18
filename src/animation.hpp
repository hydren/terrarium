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

#include "extra/sprite.hpp"
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
	string currentAnim;

	public:
	AnimationSet(Image* sheet);

	void add(string tag, int width=-1, int height=-1, double interval=-1, int frames=1, bool setCurrent=true);

	Sprite& ref(string tag);

	Sprite& operator[](string tag);

	Sprite& current();

	void setCurrent(string tag);

	void draw(float x=0, float y=0, float angle=0);

	//TODO add methods for editing the animation set.
};


#endif /* ANIMATION_HPP_ */
