/*
 * physics.cpp
 *
 *  Created on: 28/10/2013
 *      Author: felipe
 */

#include "physics.hpp"

namespace Physics
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Namespace functions

	float convertToMeters(float pixels) { return 0.01f * pixels; }
	float convertToPixels(float meters) { return 100.0f * meters;}

	Vector newVector(float x, float y)
	{
		Vector v = {x, y};
		return v;
	}
}
