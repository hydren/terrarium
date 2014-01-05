/*
 * main_menu.hpp
 *
 *  Created on: 01/10/2013
 *      Author: carlosfaruolo
 */

#ifndef MAIN_MENU_HPP_
#define MAIN_MENU_HPP_

#include "util.hpp"
#include "engine.hpp"

using Engine::Image;

class Menu
{
	struct Entry
	{
		String label;
		bool enabled;

		Entry(String str)
		: label(str),
		  enabled(true)
		{}
	};

	vector<Entry*> entries;

	public:

	unsigned selectedIndex;
	Engine::Font* font;
	Engine::Color color, selectedColor;
	Rect bounds;

	Menu(Rect bounds, Engine::Font* font, Engine::Color color)
	: entries(),
	  selectedIndex(-1),
	  font(font),
	  color(color),
	  selectedColor(Engine::Color(255-color.r, 255-color.g, 255-color.b)),
	  bounds(bounds)
	{}

	void addEntry(String label, int index=-1)
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

	void removeEntry(unsigned index)
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

	Entry* operator [] (int index)
	{
		return entries[index];
	}

	Entry* getSelectedEntry()
	{
		return entries[selectedIndex];
	}

	unsigned getNumberOfEntries()
	{
		return entries.size();
	}

	void setSelectedEntry(Entry* entry)
	{
		for(unsigned i = 0; i < entries.size(); i++)
			if(entries[i] == entry)
			{
				selectedIndex = i;
				break;
			}
	}

	/** Safe way to set the selected index */
	void setSelectedEntry(unsigned index)
	{
		if(index > entries.size()-1)
			return;

		selectedIndex = index;
	}


	/** Decrement the selected index in a safe way */
	Menu* operator --()
	{
		if(selectedIndex > 0)
			selectedIndex--;
		return this;
	}

	/** Increment the selected index in a safe way */
	Menu* operator ++()
	{
		if(selectedIndex < entries.size()-1)
			selectedIndex++;
		return this;
	}

	/** Draw the menu according the menu bounds and number of entries */
	void draw()
	{
		float distanceBetween = bounds.h / (float) entries.size();

		float offset = 0;

		for(unsigned i = 0; i < entries.size(); i++)
		{
			if(i == selectedIndex)
				font->draw_text(entries[i]->label, bounds.x, bounds.y + offset, selectedColor);
			else
				font->draw_text(entries[i]->label, bounds.x, bounds.y + offset, color);

			offset += distanceBetween;
		}
	}
};

class MainMenu
{
	private:
	//nothing yet

	Image* background;
	Engine::Font* mainFont;
	Engine::Font* minorFont;
	Engine::EventQueue* eventQueue;

	public:

	MainMenu();

	enum MENU_OPTION
	{
		NEW_GAME,
		LOAD_MAP_FROM_FILE,
		OPTIONS,
		EXIT,
		NOTHING
	};

	/** Returns the selected option */
	MENU_OPTION show();
};


#endif /* MAIN_MENU_HPP_ */
