/*
 * menu.hpp
 *
 *  Created on: 07/01/2014
 *      Author: carlosfaruolo
 */

#ifndef MENU_HPP_
#define MENU_HPP_

#include <vector>
using std::vector;

#include "fgeal.hpp"

class Menu
{
	struct Entry
	{
		string label;
		bool enabled;

		Entry(string str)
		: label(str),
		  enabled(true)
		{}
	};

	vector<Entry*> entries;
	string* title;

	public:

	unsigned selectedIndex;
	GameEngine::Font* font;
	GameEngine::Color bgColor, fontColor, selectedColor;
	Rect bounds;

	Menu(Rect bounds, GameEngine::Font* font, GameEngine::Color color, string title="##NO_DEF##" );


	void addEntry(string label, int index=-1);

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
