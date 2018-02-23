/*
 * statuses.hpp
 *
 *  Created on: 23 de fev de 2018
 *      Author: carlosfaruolo
 */

#ifndef STATUSES_HPP_
#define STATUSES_HPP_

#include "fgeal/fgeal.hpp"

#include <string>
#include <vector>

struct Status
{
	struct Definition
	{
		std::string label, description;
		int minimumBaseValue, maximumBaseValue;
		fgeal::Color color;

		private:
		static std::vector<Definition> statusesDefs;

		public:
		inline static const Definition& get(int index) { return statusesDefs[index]; }
	};

	Definition& def;
	int value;
};

#endif /* STATUSES_HPP_ */
