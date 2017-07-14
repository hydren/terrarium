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

		// ============================== GENERAL CHARACTESTICS

		/// the maximum amount of items of this type that can be stacked.
		const unsigned stackingLimit;

		/// the mass of a item of this type
		const float mass;

		/// this item's icon
		fgeal::Sprite* icon;

		// ============================== CONTAINER CHARACTESTICS

		/// the amount of item slots this type of item has, if this type of item is a container-type
		const unsigned itemSlotCount;

		// ============================== NON-POD CHARACTESTICS (END OF STRUCT)

		/// a human-readable name of this item type
		const std::string name;

		/// a human-readable description of this item type
		const std::string description;

		// ============================================================

		/// constructor for non-containers types
		Type(unsigned stackLimit, float mass,
			 const std::string& name, const std::string& desc);

		/// constructor for containers types
		Type(unsigned stackLimit, float mass,
			 unsigned itemSlotCount,
			 const std::string& name, const std::string& desc);

		/// returns true if this type of item is a container-type
		inline bool isContainer() const { return itemSlotCount > 0; }
	};

	/// the ID of this item
	const unsigned id;

	// ============================== GENERAL PROPERTIES

	/// this item's type (reference)
	const Type& type;

	// ============================== CONTAINER PROPERTIES

	/// the items inside this item, if this type of item is a container-type
	std::vector<Item*> items;

	// ============================================================

	Item(const Type& type);

	/// Returns true if it is possible to add this item to this inventory.
	bool canAdd(Item* item);
};

struct Inventory
{
	fgeal::Rectangle bounds;
	Item* container;

	Inventory(const fgeal::Rectangle& bounds, Item* container);

	inline bool canAdd(Item* item) { return container->type.isContainer() and container->canAdd(item); }

	inline std::vector<Item*>& items() { return container->items; }

	/// Draws this inventory on screen
	void draw();
};

#endif /* INVENTORY_HPP_ */
