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

Item::Type Item::Type::createBlockType(const std::string& name, const std::string& desc, float mass, unsigned stackLimit)
{
	Type t;
	t.id = ++ITEM_TYPE_ID_LAST;
	t.name = name;
	t.description = desc;

	t.stackingLimit = stackLimit;
	t.mass = mass;
	t.icon = null;

	t.isPlaceable = true;

	t.isDiggingTool = false;

	t.itemSlotCount = 0;

	return t;
}

Item::Type Item::Type::createToolType(const std::string& name, const std::string& desc, float mass, unsigned stackLimit, bool isDigger)
{
	Type t;
	t.id = ++ITEM_TYPE_ID_LAST;
	t.name = name;
	t.description = desc;

	t.stackingLimit = stackLimit;
	t.mass = mass;
	t.icon = null;

	t.isPlaceable = false;

	t.isDiggingTool = isDigger;

	t.itemSlotCount = 0;

	return t;
}

Item::Type Item::Type::createContainerType(const std::string& name, const std::string& desc, float mass, unsigned stackLimit, unsigned itemSlotCount)
{
	Type t;
	t.id = ++ITEM_TYPE_ID_LAST;
	t.name = name;
	t.description = desc;

	t.stackingLimit = stackLimit;
	t.mass = mass;
	t.icon = null;

	t.isPlaceable = false;

	t.isDiggingTool = false;

	t.itemSlotCount = itemSlotCount;

	return t;
}

static unsigned ITEM_ID_LAST = 0;

Item::Item(const Type& type)
: id(++ITEM_ID_LAST), type(type), amount(1)
{}

bool Item::canAdd(Item* item)
{
	// fixme make Containter::canAdd() take into account the stackingLimit
	return item != null and items.size() < type.itemSlotCount;
}

void Item::draw(float x, float y, Font* font, Color colorFont)
{
	if(type.icon != null)
		type.icon->draw(x, y);

	if(amount > 1)
		font->drawText(futil::to_string(amount),
			x + 0.95*inventorySlotSize.w - font->getTextWidth(futil::to_string(amount)),
			y + 0.95*inventorySlotSize.h - font->getFontHeight(), colorFont);
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

			if(item->amount > 1)
				font->drawText(futil::to_string(item->amount),
						x + 0.95*inventorySlotSize.w - font->getTextWidth(futil::to_string(item->amount)),
						y + 0.95*inventorySlotSize.h - font->getFontHeight(), colorFont);
		}
	}
}
