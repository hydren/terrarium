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
using fgeal::Graphics;
using fgeal::Color;
using fgeal::Font;
using fgeal::Point;
using std::string;
using futil::remove_element;

static const Rectangle inventorySlotSize = {0, 0, BLOCK_SIZE * 1.5, BLOCK_SIZE * 1.5};

Item::Item(const Type& type)
: id(type.id), type(type), amount(1)
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
			y + 0.95*inventorySlotSize.h - font->getHeight(), colorFont);
}

Item::Type Inventory::GLOBAL_INVENTORY_ITEM_TYPE;

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
				it->amount += item->amount;
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
				return container->items[i];
		}
	}
	return null;
}

void Inventory::draw()
{
	Graphics::drawFilledRectangle(bounds.x, bounds.y, bounds.w, bounds.h, color);
	Graphics::drawRectangle(bounds.x, bounds.y, bounds.w, bounds.h, Color::GREY);

	const int slotsPerLine = (int) (bounds.w / inventorySlotSize.w);
	for(unsigned i = 0; i < container->type.itemSlotCount; i++)
	{
		const float x = bounds.x + inventorySlotSize.w * (i % slotsPerLine) + 1;
		const float y = bounds.y + inventorySlotSize.h * (i / slotsPerLine) + 1;
		Graphics::drawRectangle(x, y, inventorySlotSize.w, inventorySlotSize.h, Color::LIGHT_GREY);

		if(i < container->items.size())
		{
			Item* item = container->items[i];

			if(item->type.icon != null)
				item->type.icon->draw(x + 0.5*(inventorySlotSize.w - item->type.icon->width),
									  y + 0.5*(inventorySlotSize.h - item->type.icon->height));

			if(item->amount > 1)
				font->drawText(futil::to_string(item->amount),
						x + 0.95*inventorySlotSize.w - font->getTextWidth(futil::to_string(item->amount)),
						y + 0.95*inventorySlotSize.h - font->getHeight(), colorFont);
		}
	}
}
