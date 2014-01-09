/*
 * menu.hpp
 *
 *  Created on: 07/01/2014
 *      Author: carlosfaruolo
 */

#ifndef MENU_HPP_
#define MENU_HPP_

#include "util.hpp"
#include "engine.hpp"

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
	Engine::Image* bg;
	String* title;

	public:

	unsigned selectedIndex;
	Engine::Font* font;
	Engine::Color fontColor, selectedColor;
	Rect bounds;

	Menu(Rect bounds, Engine::Font* font, Engine::Color color, bool gui=false, String title="##NO_DEF##" );


	void addEntry(String label, int index=-1);

	void removeEntry(unsigned index);

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

	void setSelectedEntry(Entry* entry);

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
	void draw();
};


#endif /* MENU_HPP_ */
