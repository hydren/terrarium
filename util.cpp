#include "util.hpp"

// ====================  adding almost java-like habilities to String

String operator + (String a, String b)
{
    std::stringstream ss;
    ss << a << b;
    return ss.str();
}

String operator + (String a, int b)
{
    std::stringstream ss;
    ss << a << b;
    return ss.str();
}

String operator + (String a, long b)
{
    std::stringstream ss;
    ss << a << b;
    return ss.str();
}

String operator + (String a, short b)
{
    std::stringstream ss;
    ss << a << b;
    return ss.str();
}

String operator + (String a, float b)
{
    std::stringstream ss;
    ss << a << b;
    return ss.str();
}

String operator + (String a, double b)
{
    std::stringstream ss;
    ss << a << b;
    return ss.str();
}

String operator + (String a, char b)
{
    std::stringstream ss;
    ss << a << b;
    return ss.str();
}

String operator + (String a, char* b)
{
    std::stringstream ss;
    ss << a << b;
    return ss.str();
}

//backwards versions

String operator + (int a, String b)
{
    std::stringstream ss;
    ss << a << b;
    return ss.str();
}

String operator + (long a, String b)
{
    std::stringstream ss;
    ss << a << b;
    return ss.str();
}

String operator + (short a, String b)
{
    std::stringstream ss;
    ss << a << b;
    return ss.str();
}

String operator + (float a, String b)
{
    std::stringstream ss;
    ss << a << b;
    return ss.str();
}

String operator + (double a, String b)
{
    std::stringstream ss;
    ss << a << b;
    return ss.str();
}

String operator + (char a, String b)
{
    std::stringstream ss;
    ss << a << b;
    return ss.str();
}

String operator +(char* a, String b)
{
    std::stringstream ss;
    ss << a << b;
    return ss.str();
}

//==============================================================================

Exception::Exception(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsprintf(buffer, format, args);
    msg = String(buffer);
    va_end(args);
}

FILE* Util::createFile(const String& str_filename, bool abortOnFailure)
{
    char* filename;
    filename = new char[str_filename.size()];
    filename = strcpy(filename, str_filename.c_str());
    //just to avoid some problems with fgets including '\n' in strings
    if(filename[strlen(filename)-1] == '\n') filename[strlen(filename)-1] = '\0';

    FILE* file;
    if( (file = fopen(filename, "w")) == NULL )
    {
        if(abortOnFailure) exit(-1);
        else
        {
            String msg="";
            msg = msg+"Couldn't load file "+filename;
            throw Exception(msg);
        }
    }

    return file;
}

FILE* Util::loadFile(const String& str_filename, bool abortOnFailure)
{
    char* filename;
    filename = new char[str_filename.size()];
    filename = strcpy(filename, str_filename.c_str());
    //just to avoid some problems with fgets including '\n' in strings
    if(filename[strlen(filename)-1] == '\n') filename[strlen(filename)-1] = '\0';

    FILE* file;
    if( (file = fopen(filename, "rw")) == NULL )
    {
        if(abortOnFailure) exit(-1);
        else
        {
            String msg="";
            msg = msg+"Couldn't load file "+filename;
            throw Exception(msg);
        }
    }

    return file;
}

void Util::closeFile(FILE *file)
{
    fclose(file);
}

bool Rectangle::intersects(const Rectangle& box )
{
	if
    (   ((int)this->x + (int)this->w) <= (int)box.x || //S1, S4, S7
        (int)this->x  >= ((int)box.x + (int)box.w)  || //S3, S6, S9
        ((int)this->y + (int)this->h) <= (int)box.y || //S1, S2, S3
        (int)this->y  >= ((int)box.y + (int)box.h)     //S7, S8, S9
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

Rectangle Rectangle::intersection(const Rectangle& box)
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
	Rectangle intersectionBox(x,y,w,h);
	return intersectionBox;
}

int Rectangle::area()
{
	return w*h; //you don't say
}
