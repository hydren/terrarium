#ifndef UTIL_HPP_INCLUDED
#define UTIL_HPP_INCLUDED

//utilities, multi-purpose things

#include <string>
#include <iostream>

// * * * * * Useful macros

//Same as std::cout
#define COUT std::cout

//Same as std::endl
#define ENDL std::endl

//Same as std::cerr
#define CERR std::cerr

/* Same as '= 0'
 * Used right after method declaration. Ex: int giveNumber(const int x) abstract;
 * Means that this function has no implementation, making the containing class abstract, impossible to instanciate it.
 * The only way to instanciate, is to instanciate a subclass of it that is not abstract.
 * You can, however, declare a pointer to this class, or a reference. 
 * Ex: Animal* cat = new Cat();
 * Dog dog;
 * Animal& another_dog = dog;
 */
#define abstract = 0

// Same as ':'.
#define extends :

// Same as std::string
typedef std::string String;

/* Class created to behave like the Java's Exception
 * Contains a String msg, with the error message
 */
class Exception
{
    private:

    String msg;

    public:

    Exception(std::string str)
    {
        msg = str;
    }

    std::string message() { return msg; }
};

/* Class to behave like a pointer, but with class capacities.
 */
template <typename Generic> class Pointer
{
    // o ponteiro é const pois: ao usar o operador '=', o operador '=' padrao fara uma copia da instancia de Pointer, copiando tambem o ponteiro
    // o problema é quando temos varias instancia de Pointer que tem o mesmo ponteiro, e alguem resolve mudar o ponteiro de um delas
    // neste caso, as outras instancias copias nao serao atualizadas e, logo, nao representarao o mesmo ponteiro: inconsistencia
    // isso tambem ocorreria muito ao colocar estas instancias em list, vector, etc, do STL, que na verdade fazerm copias dos elementos adicionados
    Generic* const ptr;

    public:

    Generic* const pointer() {return ptr;}

    Pointer(Generic *ptr2):
    ptr(ptr2)
    {}

    //parece que o destrutor padrao ainda eh chamado, fazendo delete ptr
    /*~Pointer()
    {
        delete ptr;
        ptr=NULL;
    }*/

    bool operator < (const Pointer<Generic>& b) const
    {
        return *ptr < *(b.ptr);
    }

    Generic* operator ->()
    {
        return ptr;
    }

};


/* Class that contains position and size of a box.
 * It has a method to check for intersection between other BoundingBox's.
 * Useful to simulate collision between bodies.
 */
class BoundingBox
{
	public:
	
	int x, y, w, h;
	
	//Creates a bounding box with the given parameters
	BoundingBox(int x1, int y1, int w1, int h1) :
	x(x1), y(y1), w(w1), h(h1) {}
	
	//Returns true if this bounding box intersects with the given bounding box.
	bool intersects(const BoundingBox& box );
	
	/* Returns a bounding box that represents the intersection of this bounding box with the given bounding box.
	 * If there is no intersection, throws an Exception.
	 * In other words, you should check if the bounding boxes intersects before trying to call this method, with the intersects() method.
	 */
	BoundingBox intersection(const BoundingBox& box);
	
	/* Returns the area of this bounding box.
	 * The calculation is pretty silly: w*h (width x height)
	 */
	int area();
};

class Util 
{
	public:
	inline static int abs(int i)
	{
		return i < 0 ? -i : i;
	}
		
};

/*NOTES: 
 * A "pure" virtual member is a virtual method that is declared to be = 0. Example:
 * 
 * virtual void foo() = 0;
 * 
 * To make the code more readable, there is a macro named "abstract" that is defined as "= 0".
 * With this you can declare pure virtual methods like this:
 * 
 * virtual void foo() abstract; //cool huh?
 * 
 * You can even make CodeLite recognize "abstract" as a keyword, making it appear with the same color as ther others keywords!
 * To do this go to Settings -> Syntax Highlighting and Fonts -> Customize -> C++ keyword -> Set 0, and add "abstract" to the list.
*/

#endif // UTIL_HPP_INCLUDED