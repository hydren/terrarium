/*
 * inventory.cpp
 *
 *  Created on: 14 de jul de 2017
 *      Author: carlosfaruolo
 */

#include "inventory.hpp"
#include "block.hpp"

static unsigned ITEM_TYPE_ID_LAST = 0;

Item::Type::Type(unsigned stackLimit, float mass,
				 const std::string& name, const std::string& desc)
: id(++ITEM_TYPE_ID_LAST),
  stackingLimit(stackLimit), mass(mass), icon(null),
  itemSlotCount(0),
  name(name), description(desc)
{}

Item::Type::Type(unsigned stackLimit, float mass,
		 	 	 unsigned itemSlotCount,
				 const std::string& name, const std::string& desc)
: id(++ITEM_TYPE_ID_LAST),
  stackingLimit(stackLimit), mass(mass), icon(null),
  itemSlotCount(itemSlotCount),
  name(name), description(desc)
{}

static unsigned ITEM_ID_LAST = 0;

Item::Item(const Type& type)
: id(++ITEM_ID_LAST), type(type)
{}

bool Item::canAdd(Item* item)
{
	// fixme make Containter::canAdd() take into account the stackingLimit
	return item != null and items.size() < type.itemSlotCount;
}

const fgeal::Color inventoryBgColor(50, 100, 150, 96);

Inventory::Inventory(const fgeal::Rectangle& bounds, Item* container)
: bounds(bounds), container(container)
{}

void Inventory::draw()
{
	using fgeal::Rectangle;
	using fgeal::Image;
	const Rectangle inventorySlotSize = {0, 0, BLOCK_SIZE * 1.5, BLOCK_SIZE * 1.5};

	Image::drawRectangle(inventoryBgColor, bounds.x, bounds.y, bounds.w, bounds.h);
	Image::drawRectangle(inventoryBgColor, bounds.x, bounds.y, bounds.w, bounds.h);

	const int slotsPerLine = (int) (bounds.w / inventorySlotSize.w);
	for(unsigned i = 0; i < container->type.itemSlotCount; i++)
	{
		const float x = bounds.x + inventorySlotSize.w * (i % slotsPerLine);
		const float y = bounds.y + inventorySlotSize.h * (i / slotsPerLine);
		Image::drawRectangle(inventoryBgColor, x, y, inventorySlotSize.w, inventorySlotSize.h, false);

		if(i < container->items.size() and container->items[i]->type.icon != null)
			container->items[i]->type.icon->draw(x, y);
	}
}
