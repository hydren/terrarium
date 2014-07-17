#ifndef TEXTURE_HPP_INCLUDED
#define TEXTURE_HPP_INCLUDED

#include "util.hpp"

class Texture 
{	
	public:
	
	//saber o tipo de textura, para fins de debug
	const String texture_type;
	
	protected:
	
	//pra poder inicializar essa variavel nas subclasses
	Texture(String str) : texture_type(str) {}

public:
	
	//  = = =  Classes to draw on the screen
	
	/* Render this whole Texture in the game screen, from left-up to right-down. This is equivalent to call "render(0,0)"
	 */
	virtual void render() abstract;
	
	/* Render this whole Texture in the game screen, positioning the texture in the given coordinates.
	 */
	virtual void render(int x, int y) abstract;
	
	/* Render an area of this Texture in the game screen, positioning the texture in the given coordinates.
	 * The area selected is selected following the given parameters.
	 * from_x and from_y specifies the position, w and h specifies the size of the selected area.
	 */
	virtual void render(int x, int y, int from_x, int from_y, int w, int h) abstract;
	
	//  = = =  Classes to "blit" on others Textures, this name is temporary...
	
	/* Render this whole Texture in Texture tex2, from left-up to right-down. This is equivalent to call "paste(0,0)"
	 */
	virtual void paste(const Texture& tex2) abstract;
	
	/* Render this whole Texture in Texture tex2, positioning the texture in the given coordinates.
	 */
	virtual void paste(const Texture& tex2, int x, int y) abstract;
	
	/* Render an area of this Texture in Texture tex2, positioning the texture in the given coordinates.
	 * The area selected is selected following the given parameters.
	 * from_x and from_y specifies the position, w and h specifies the size of the selected area.
	 */
	virtual void paste(const Texture& tex2, int x, int y, int from_x, int from_y, int h, int w) abstract;

};

#endif // TEXTURE_HPP_INCLUDED
