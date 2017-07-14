/*
 * inventory.cpp
 *
 *  Created on: 14 de jul de 2017
 *      Author: carlosfaruolo
 */

#include "inventory.hpp"

static unsigned ITEM_TYPE_ID_LAST = 0;
Item::Type::Type(unsigned stackLimit, float mass, const std::string& name, const std::string& desc)
: id(++ITEM_TYPE_ID_LAST), stackingLimit(stackLimit), mass(mass), name(name), description(desc)
{}

Item::Type
	Item::Type::BLOCK_STONE (999, 20.0, "Dirt block", "A dirt block is a dirt block. Get over it."),
	Item::Type::BLOCK_DIRT  (999, 10.0, "Stone block", "A stone block is a stone block. Rolling Stones!"),
	Item::Type::BAG         ( 50,  0.5, "Bag", "A simple bag that can carry some items.");

static unsigned ITEM_ID_LAST = 0;
Item::Item(const Type& type)
: id(++ITEM_ID_LAST), type(type)
{}

static unsigned CONTAINER_TYPE_ID_LAST = 0;
Container::Type::Type(const Item::Type& itemType, unsigned slotCount)
: id(++CONTAINER_TYPE_ID_LAST), itemType(itemType), itemSlotCount(slotCount)
{}

Container::Type
	Container::Type::BAG(Item::Type::BAG, 32);

Container::Container(const Type& type)
: Item(type.itemType), type(type), items()
{}

bool Container::canAdd(Item* item)
{
	return true;
}
