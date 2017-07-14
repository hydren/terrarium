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

void Container::draw(float pane_x, float pane_y, float pane_w, float pane_h)
{
	using fgeal::Rectangle;
	using fgeal::Image;
	const Rectangle inventorySlotSize = {0, 0, BLOCK_SIZE * 1.5, BLOCK_SIZE * 1.5};

	Image::drawRectangle(inventoryBgColor, pane_x, pane_y, pane_w, pane_h);
	Image::drawRectangle(inventoryBgColor, pane_x, pane_y, pane_w, pane_h);

	const int slotsPerLine = (int) (pane_w / inventorySlotSize.w);
	for(unsigned i = 0; i < type.itemSlotCount; i++)
	{
		const float x = pane_x + inventorySlotSize.w * (i % slotsPerLine);
		const float y = pane_y + inventorySlotSize.h * (i / slotsPerLine);
		Image::drawRectangle(inventoryBgColor, x, y, inventorySlotSize.w, inventorySlotSize.h, false);

		if(i < items.size() and items[i]->type.icon != null)
			items[i]->type.icon->draw(x, y);
	}
}
