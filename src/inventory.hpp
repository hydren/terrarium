/*
 * inventory.hpp
 *
 *  Created on: 13 de jul de 2017
 *      Author: carlosfaruolo
 */

#ifndef INVENTORY_HPP_
#define INVENTORY_HPP_
#include <ciso646>

#include "fgeal/extra/sprite.hpp"

#include "futil/language.hpp"

#include <vector>
#include <string>

struct Item
{
	struct Type
	{
		/// this ID of this type
		const unsigned id;

		/// the maximum amount of items of this type that can be stacked.
		const unsigned stackingLimit;

		/// the mass of a item of this type
		const float mass;

		/// a human-readable name of this item type
		const std::string name;

		/// a human-readable description of this item type
		const std::string description;

		/// this item's icon
		fgeal::Sprite* icon;

		Type(unsigned stackLimit, float mass, const std::string& name, const std::string& desc);
	};

	/// the ID of this item
	const unsigned id;

	/// this item's type (reference)
	const Type& type;

	Item(const Type& type);
};

struct Container extends Item
{
	struct Type
	{
		/// this ID of this type
		const unsigned id;

		/// the item type id of this container id
		const Item::Type& itemType;

		/// the amount of slots on this container
		unsigned itemSlotCount;

		Type(const Item::Type& itemType, unsigned slotCount);
	};

	/// this container's type (reference)
	const Type& type;

	/// the items on this container
	std::vector<Item*> items;

	Container(const Type& type);

	/// Returns true if it is possible to add this item to this inventory.
	bool canAdd(Item* item);
};

struct Inventory
{
	fgeal::Rectangle bounds;
	Container* container;

	Inventory(const fgeal::Rectangle& bounds, Container* container);

	inline bool canAdd(Item* item) { return container->canAdd(item); }

	inline std::vector<Item*>& items() { return container->items; }

	/// Draws this inventory on screen
	void draw();
};

#endif /* INVENTORY_HPP_ */
