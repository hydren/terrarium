/*
 * menu.cpp
 *
 *  Created on: 08/01/2014
 *      Author: carlosfaruolo
 */

#include "../header/menu.hpp"

Menu::Menu(Rect bounds, Engine::Font* font, Engine::Color color, bool gui, String title)
 : entries(),
   bg(null),
   title(null),
   selectedIndex(-1),
   font(font),
   fontColor(color),
   selectedColor(Engine::Color(255-color.r, 255-color.g, 255-color.b)),
   bounds(bounds)
{
	if(gui) //TODO NOT WORKING!!!
	{
		bg = new Engine::Image(Engine::Image::FILLED_RECTANGLE, selectedColor, bounds.w, bounds.h);
		if(title.compare("##NO_DEF##")==0)
		{
			Engine::Image* temp = new Engine::Image(Engine::Image::FILLED_RECTANGLE, Engine::Color(0,0,0), bounds.w-4, bounds.h-4);
			temp->blit(*bg, 2, 2);
			delete temp;
		}
		else
		{
			Engine::Image* temp = new Engine::Image(Engine::Image::FILLED_RECTANGLE, Engine::Color(0,0,0), bounds.w-4, bounds.h-4 - font->getSize());
			temp->blit(*bg, 2, 2+font->getSize());
			this->title = new String(title);
			delete temp;
		}
	}
}

void Menu::addEntry(String label, int index)
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
	if(bg != null)
		bg->draw(bounds.x, bounds.y);

	float distanceBetween = (bounds.h-font->getSize()) / ((float) entries.size() + (title==null?0:1));

	float offset = (title==null?0:font->getSize());

	if(title != null)
		font->draw_text(*title, bounds.x, bounds.y, fontColor);


	for(unsigned i = 0; i < entries.size(); i++)
	{
		if(i == selectedIndex)
			font->draw_text(entries[i]->label, bounds.x +(bg!=null?2:0), bounds.y + offset, selectedColor);
		else
			font->draw_text(entries[i]->label, bounds.x +(bg!=null?2:0), bounds.y + offset, fontColor);

		offset += distanceBetween;
	}
}
