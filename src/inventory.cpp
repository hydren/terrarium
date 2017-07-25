/*
 * inventory.cpp
 *
 *  Created on: 14 de jul de 2017
 *      Author: carlosfaruolo
 */

#include "inventory.hpp"
#include "block.hpp"

#include "futil/string_actions.hpp"
#include "futil/collection_actions.hpp"

using fgeal::Rectangle;
using fgeal::Image;
using fgeal::Color;
using fgeal::Font;
using fgeal::Point;
using std::string;
using futil::remove_element;

static unsigned ITEM_TYPE_ID_LAST = 0;

static const Rectangle inventorySlotSize = {0, 0, BLOCK_SIZE * 1.5, BLOCK_SIZE * 1.5};

Item::Type::Type(unsigned stackLimit, float mass, const string& name, const string& desc)
: id(++ITEM_TYPE_ID_LAST),
  stackingLimit(stackLimit), mass(mass), icon(null),
  itemSlotCount(0),
  name(name), description(desc)
{}

Item::Type::Type(unsigned stackLimit, float mass, unsigned itemSlotCount, const string& name, const string& desc)
: id(++ITEM_TYPE_ID_LAST),
  stackingLimit(stackLimit), mass(mass), icon(null),
  itemSlotCount(itemSlotCount),
  name(name), description(desc)
{}

static unsigned ITEM_ID_LAST = 0;

Item::Item(const Type& type)
: id(++ITEM_ID_LAST), type(type), amount(1)
{}

bool Item::canAdd(Item* item)
{
	// fixme make Containter::canAdd() take into account the stackingLimit
	return item != null and items.size() < type.itemSlotCount;
}

Inventory::Inventory(const Rectangle& bounds, Font* font, Item* container)
: bounds(bounds), font(font), color(128, 128, 128, 128), colorFont(Color::BLACK), container(container)
{}

Inventory::~Inventory()
{
	foreach(Item*, item, vector<Item*>, items())
	{
		delete item;
	}

	delete container;
	container = null;
}

void Inventory::add(Item* item)
{
	if(item->type.stackingLimit > 1)  // if can be stacked, attempt stacking
	{
		foreach(Item*, it, vector<Item*>, container->items)
		{
			if(it->type == item->type and it->amount < it->type.stackingLimit)
			{
				it->amount++;
				delete item;
				return;
			}
		}
	}

	// if we got here, then we couldn't stack the given item
	container->items.push_back(item);
}

bool Inventory::isPointWithin(float ptx, float pty)
{
	return ptx > bounds.x and ptx < bounds.x + bounds.w
	   and pty > bounds.y and pty < bounds.y + bounds.h;
}

Item* Inventory::getItemInSlotPointedBy(float ptx, float pty)
{
	if(isPointWithin(ptx, pty))
	{
		const int slotsPerLine = (int) (bounds.w / inventorySlotSize.w);
		for(unsigned i = 0; i < container->type.itemSlotCount; i++)
		{
			const float x = bounds.x + inventorySlotSize.w * (i % slotsPerLine) + 1;
			const float y = bounds.y + inventorySlotSize.h * (i / slotsPerLine) + 1;

			if(ptx > x and ptx < x + inventorySlotSize.w and
			   pty > y and pty < y + inventorySlotSize.h and
			   i < container->items.size())
			{
				Item* item = container->items[i];
				remove_element(container->items, item);
				return item;
			}
		}
	}
	return null;
}

void Inventory::draw()
{
	Image::drawRectangle(color, bounds.x, bounds.y, bounds.w, bounds.h);
	Image::drawRectangle(Color::GREY, bounds.x, bounds.y, bounds.w, bounds.h, false);

	const int slotsPerLine = (int) (bounds.w / inventorySlotSize.w);
	for(unsigned i = 0; i < container->type.itemSlotCount; i++)
	{
		const float x = bounds.x + inventorySlotSize.w * (i % slotsPerLine) + 1;
		const float y = bounds.y + inventorySlotSize.h * (i / slotsPerLine) + 1;
		Image::drawRectangle(Color::LIGHT_GREY, x, y, inventorySlotSize.w, inventorySlotSize.h, false);

		if(i < container->items.size())
		{
			Item* item = container->items[i];

			if(item->type.icon != null)
				item->type.icon->draw(x + 0.5*(inventorySlotSize.w - item->type.icon->width),
									  y + 0.5*(inventorySlotSize.h - item->type.icon->height));
			else
			{
				Image::drawCircle(Color::LIGHT_GREY, x + 0.5*inventorySlotSize.w, y + 0.5*inventorySlotSize.h, 0.5*inventorySlotSize.h);
				font->drawText(item->type.name, x, y, colorFont);
			}

			if(item->amount > 1)
				font->drawText(futil::to_string(item->amount),
						x + 0.95*inventorySlotSize.w - font->getTextWidth(futil::to_string(item->amount)),
						y + 0.95*inventorySlotSize.h - font->getFontHeight(), colorFont);
		}
	}
}
