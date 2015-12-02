/*
 * menu.cpp
 *
 *  Created on: 08/01/2014
 *      Author: carlosfaruolo
 */

#include "menu.hpp"

Menu::Menu(Rect bounds, GameEngine::Font* font, GameEngine::Color color, string title)
 : entries(),
   title(null),
   selectedIndex(-1),
   font(font),
   bgColor(GameEngine::Color::BLACK),
   fontColor(color),
   selectedColor(GameEngine::Color(255-color.r, 255-color.g, 255-color.b)),
   bounds(bounds)
{}

void Menu::addEntry(string label, int index)
{
	if(index < 0)
		entries.push_back(new Entry(label));
	else
		entries.insert(entries.begin()+index, new Entry(label));

	if(entries.size() == 1)
	{
		selectedIndex = 0;
	}
}

void Menu::removeEntry(unsigned index)
{
	if(index < 0 || index > entries.size()-1)
		return;

	if(index == selectedIndex)
	{
		if(entries.size() == 1)
		{
			selectedIndex = -1;
		}

		else if(index == entries.size()-1)
		{
			selectedIndex = index-1;
		}

		else
		{
			selectedIndex = index+1;
		}
	}
	entries.erase(entries.begin()+index);
}

void Menu::setSelectedEntry(Entry* entry)
{
	for(unsigned i = 0; i < entries.size(); i++)
		if(entries[i] == entry)
		{
			selectedIndex = i;
			break;
		}
}

/** Draw the menu according the menu bounds and number of entries */
void Menu::draw()
{
//	if(bg != null)
//		bg->draw(bounds.x, bounds.y);

	GameEngine::Image::draw_rectangle(selectedColor, bounds.x, bounds.y, bounds.w, bounds.h);
	GameEngine::Image::draw_rectangle(bgColor, bounds.x+2, bounds.y+2, bounds.w-4, bounds.h-4);

	float distanceBetween = (bounds.h-font->getSize()) / ((float) entries.size() + (title==null?0:1));

	float offset = (title==null?0:font->getSize());

	if(title != null)
		font->draw_text(*title, bounds.x, bounds.y, fontColor);


	for(unsigned i = 0; i < entries.size(); i++)
	{
		//quick dirty fix TODO remove this and do a better structure
		string str = entries[i]->label;
		if(str.length() > 30) do str = "..."+str.substr(4); while(str.length() > 30);

		if(i == selectedIndex)
			font->draw_text(str, bounds.x+2, bounds.y + offset, selectedColor);
		else
			font->draw_text(str, bounds.x+2, bounds.y + offset, fontColor);

		offset += distanceBetween;
	}
}
