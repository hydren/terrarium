/*
 * block.hpp
 *
 *  Created on: Jun 18, 2013
 *      Author: felipe
 */

#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#define BLOCK_SIZE 16

#include "entity.hpp"

class Block extends public Entity
{
	public:

	// id of the block type
	int typeID;

	// grid position of this block
	const unsigned gridX, gridY;

	// constructors
	Block(Animation* anim, unsigned gridX, unsigned gridY, int typeID, bool ignoreCollision=false);

	void draw(const Rectangle& visibleArea);

	// creates a proper block-type animation set based on the given arguments.
	static StackedSingleSheetAnimation* createBlockAnimationSet(Image* sheet, unsigned frameCount=1, float frameInterval=-1);

	// holds animation indexes
	struct Anim
	{
		// possible animation indexes
		enum
		{
			FULL_COVER,
			TOP_FREE,
			BOTTOM_FREE,
			LEFT_FREE,
			RIGHT_FREE,
			TOP_LEFT_FREE,
			BOTTOM_LEFT_FREE,
			TOP_RIGHT_FREE,
			BOTTOM_RIGHT_FREE,
			HORIZONTAL_FREE,
			VERTICAL_FREE,
			BOTTOM_COVERED,
			TOP_COVERED,
			RIGHT_COVERED,
			LEFT_COVERED,
			FULL_FREE
		};
	};

	// holds some logic information about a type ID
	struct Type
	{
		unsigned id;
		std::string name, description;
		bool pickaxeMinerable;
		int detatchedItemTypeId;

		enum Passability { PASSABILITY_NONE, PASSABILITY_FULL } passability;

		enum Precipitability {
			PRECIPITABILITY_NONE,
			PRECIPITABILITY_LIQUIDOUS,
			PRECIPITABILITY_ARENACEOUS
		} precipitability;
	};
};

#endif /* BLOCK_HPP_ */
