#include "util.hpp"

bool BoundingBox::intersects(const BoundingBox& box )
{
	if
	(   (this->x + this->w) < box.x || //S1, S4, S7
		this->x  > (box.x + box.w)  || //S3, S6, S9
		(this->y + this->h) < box.y || //S1, S2, S3
		this->y  > (box.y + box.h)     //S7, S8, S9
	) return false;
	else return true;

/*

 * I know it isn't the most straightfoward way to do the check, but it uses less statments.
 * If it is on S1, S2, S3, S4, S6, S7, S8 or S9, it does not intersect, else it does.
     S1   |         S2          |   S3
          |                     |
       TOP|LEFT                 |
   -----------------------------------
          |                     |
          |                     |
          |                     |
     S4   |         S5          |   S6
          |                     |
          |                     |
   -----------------------------------
          |                RIGHT|BOTTOM
          |                     |
	 S7   |         S8          |   S9
*/
}

BoundingBox BoundingBox::intersection(const BoundingBox& box)
{
	int x, y, h, w;
	if(!intersects(box)) throw Exception("WARNING! Bounding boxes does not intersect!");
	if(this->x <= box.x){
		x = box.x;
		if(this->w >= box.x + box.w)
			w = box.w;
		else
			w = this->w - (box.x - this->x);
	}
	else{
		x = this->x;
		if(box.w >= this->x + this->w)
			w = this->w;
		else
			w = box.w - (this->x - box.x);
	}
	if(this->y <= box.y){
		y = box.y;
		if(this->h >= box.h + box.y)
			h = box.h;
		else
			h = this->h - (box.y - this->y);					
	}
	else{
		y = this->y;
		if(box.h >= this->h + this->y)
			h = this->h;
		else
			h = box.h - (this->y - box.y);			
	}
	BoundingBox intersectionBox(x,y,w,h);
	return intersectionBox;
}

int BoundingBox::area()
{
	return w*h; //you don't say
}