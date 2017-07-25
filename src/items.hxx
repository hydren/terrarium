/*
 * items.hxx
 *
 *  Created on: 25 de jul de 2017
 *      Author: carlosfaruolo
 */

// xxx hardcoded item types should later be specified by a external file
Item::Type

	// block items
	ITEM_TYPE_BLOCK_STONE (999, 20.0, "Dirt block",  "A dirt block is a dirt block. Get over it."),
	ITEM_TYPE_BLOCK_DIRT  (999, 10.0, "Stone block", "A stone block is a stone block. Rolling Stones!"),

	// tool items

	ITEM_TYPE_PICKAXE_DEV (1, 50, "Banana pickaxe", "A special developer-only banana pickaxe. It can mine EVERYTHING."),

	// container items
	ITEM_TYPE_BAG         ( 50,  0.5, 32, "Bag", "A simple bag that can carry some items.");
