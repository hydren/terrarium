/*
 * inventory.cpp
 *
 *  Created on: 14 de jul de 2017
 *      Author: carlosfaruolo
 */

#include "inventory.hpp"
#include "block.hpp"

static unsigned ITEM_TYPE_ID_LAST = 0;
Item::Type::Type(unsigned stackLimit, float mass, const std::string& name, const std::string& desc)
: id(++ITEM_TYPE_ID_LAST), stackingLimit(stackLimit), mass(mass), name(name), description(desc), icon(null)
{}

static unsigned ITEM_ID_LAST = 0;
Item::Item(const Type& type)
: id(++ITEM_ID_LAST), type(type)
{}

static unsigned CONTAINER_TYPE_ID_LAST = 0;
Container::Type::Type(const Item::Type& itemType, unsigned slotCount)
: id(++CONTAINER_TYPE_ID_LAST), itemType(itemType), itemSlotCount(slotCount)
{}

Container::Container(const Type& type)
: Item(type.itemType), type(type), items()
{}

bool Container::canAdd(Item* item)
{
	// fixme make Containter::canAdd() take into account the stackingLimit
	return item != null and items.size() < type.itemSlotCount;
}

const fgeal::Color inventoryBgColor(50, 100, 150, 96);

Inventory::Inventory(const fgeal::Rectangle& bounds, Container* container)
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
