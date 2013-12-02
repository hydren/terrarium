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
	Entry* selected;

	public:

	Engine::Font* font;
	Engine::Color color;
	Rect bounds;

	Menu(Rect bounds, Engine::Font* font, Engine::Color color)
	: entries(),
	  selected(null),
	  font(font),
	  color(color),
	  bounds(bounds)
	{}

	void addEntry(String label, int index=-1)
	{
		if(index < 0)
			entries.push_back(new Entry(label));
		else
			entries.insert(entries.begin()+index, new Entry(label));
	}

	void removeEntry(unsigned index)
	{
		if(entries[index] == selected)
		{
			if(entries.size() == 1)
				selected = null;

			else if(index == entries.size()-1)
				selected = entries[index-1];

			else
				selected = entries[index+1];
		}
		entries.erase(entries.begin()+index);
	}

	Entry* operator [] (int index)
	{
		return entries[index];
	}

	void draw()
	{
		float distanceBetween = bounds.h / (float) entries.size();

		float offset = 0;

		for(unsigned i = 0; i < entries.size(); i++)
		{
			font->draw_text(entries[i]->label, bounds.x, bounds.y + offset, color);
			offset += distanceBetween;
		}
	}
};

class MainMenu
{
	private:
	//nothing yet

	static const int NUMBER_OF_OPTIONS;

	Image* bg;
	Engine::Font* font;
	Engine::Font* minorFont;
	Engine::EventQueue* eventQueue;
	int selected;

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
