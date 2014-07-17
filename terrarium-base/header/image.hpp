/*
 * image.hpp
 *
 *  Created on: Apr 10, 2013
 *      Author: felipe
 */

#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include "util.hpp"

class Image
{

	public:

	Image(){}
	virtual ~Image(){}

    //to be able to use it on std::vector
//    virtual Image& operator = (const Image& t)
//    {
//        //included the unnecessary 'if' block just to shut up the compiler warnings about unused variable
//        if(this != &t) return (*this);
//        return (*this);
//    }

	//  = = =  Classes to draw on the screen

	/* Render an area of this Image in the game screen, positioning the Image in the given coordinates.
	 * The area selected is selected following the given parameters.
	 * from_x and from_y specifies the position, w and h specifies the size of the selected area.
	 */
	virtual void draw(float x=0, float y=0, float from_x=0, float from_y=0, float w=-1, float h=-1) abstract;

	virtual void draw_rotated(float x=0, float y=0, float ax=0, float ay=0, float angle=0, float from_x=0, float from_y=0, float w=-1, float h=-1) abstract;

	//  = = =  Classes to "blit" on others Images, this name is temporary...

	/* Render an area of this Image in Image img2, positioning the Image in the given coordinates.
	 * The area selected is selected following the given parameters.
	 * from_x and from_y specifies the position, w and h specifies the size of the selected area.
	 */
//	virtual void blit(Image& img2, float x=0, float y=0, float from_x=0, float from_y=0, float h=-1, float w=-1) abstract;

};


#endif /* IMAGE_HPP_ */
