/*
 * items.hxx
 *
 *  Created on: 25 de jul de 2017
 *      Author: carlosfaruolo
 */

// xxx hardcoded item types should later be specified by a external file
Item::Type

	// block items
	ITEM_TYPE_BLOCK_STONE = Item::Type::createBlockType("Dirt block",  "A dirt block is a dirt block. Get over it.",      20.0, 999),
	ITEM_TYPE_BLOCK_DIRT  = Item::Type::createBlockType("Stone block", "A stone block is a stone block. Rolling Stones!", 10.0, 999),

	// tool items

	ITEM_TYPE_PICKAXE_DEV = Item::Type::createToolType("Banana pickaxe", "A special developer-only banana pickaxe. It can mine EVERYTHING.", 50.0, 1, true),

	// container items
	ITEM_TYPE_BAG = Item::Type::createContainerType("Bag", "A simple bag that can carry some items.", 0.5, 50, 32);
