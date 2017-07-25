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
		unsigned id;

		// ============================== GENERAL CHARACTERISTICS

		/// the maximum amount of items of this type that can be stacked.
		unsigned stackingLimit;

		/// the mass of a item of this type
		float mass;

		/// this item's icon
		fgeal::Sprite* icon;

		// ============================== DIGGER CHARACTERISTICS

		/// tells whether this item is a digging tool
		bool isDiggingTool;

		// ============================== CONTAINER CHARACTERISTICS

		/// the amount of item slots this type of item has, if this type of item is a container-type
		unsigned itemSlotCount;

		// ============================== NON-POD CHARACTERISTICS (END OF STRUCT)

		/// a human-readable name of this item type
		std::string name;

		/// a human-readable description of this item type
		std::string description;

		// ============================================================

		/// constructor for block types
		static Type createBlockType(const std::string& name, const std::string& desc, float mass, unsigned stackLimit);

		/// constructor for tool types
		static Type createToolType(const std::string& name, const std::string& desc, float mass, unsigned stackLimit, bool isDigger);

		/// constructor for containers types
		static Type createContainerType(const std::string& name, const std::string& desc, float mass, unsigned stackLimit, unsigned itemSlotCount);

		/// returns true if this type of item is a container-type
		inline bool isContainer() const { return itemSlotCount > 0; }

		inline bool operator == (const Type& type) const { return this->id == type.id; }
	};

	/// the ID of this item
	const unsigned id;

	// ============================== GENERAL PROPERTIES

	/// this item's type (reference)
	const Type& type;

	/// the amount of this item. if it is bigger than 1, it means this item is a stack.
	unsigned amount;

	// ============================== CONTAINER PROPERTIES

	/// the items inside this item, if this type of item is a container-type
	std::vector<Item*> items;

	// ============================================================

	Item(const Type& type);

	/// Returns true if it is possible to add this item to this inventory.
	bool canAdd(Item* item);

	inline bool isStack() { return amount > 1; }
};

struct Inventory
{
	fgeal::Rectangle bounds;
	fgeal::Font* font;
	fgeal::Color color, colorFont;

	Item* container;

	Inventory(const fgeal::Rectangle& bounds, fgeal::Font* font, Item* container);

	~Inventory();

	inline bool canAdd(Item* item) { return container->type.isContainer() and container->canAdd(item); }

	inline std::vector<Item*>& items() { return container->items; }

	/// Adds the given item to this inventory. CAUTION! THE GIVEN ITEM CAN BE DELETED AFTER THIS CALL (which means it was stacked).
	void add(Item* item);

	//todo a put method, which adds in a specific slot

	bool isPointWithin(float ptx, float pty);

	Item* getItemInSlotPointedBy(float ptx, float pty);

	/// Draws this inventory on screen
	void draw();
};

#endif /* INVENTORY_HPP_ */
