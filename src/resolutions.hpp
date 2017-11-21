/*
 * resolutions.hpp
 *
 *  Created on: 2 de out de 2017
 *      Author: carlosfaruolo
 */

#ifndef RESOLUTIONS_HPP_
#define RESOLUTIONS_HPP_
#include <ciso646>

#include <vector>
#include <string>
#include <utility>

struct Resolution
{
	unsigned width, height;
	std::pair<unsigned, unsigned> aspect;
	std::string description;

	Resolution(unsigned width, unsigned height, std::pair<unsigned, unsigned> aspect=std::make_pair(0, 0), std::string description="");

	static std::vector<Resolution>
	get(std::pair<unsigned, unsigned> aspect=std::make_pair(0, 0));

	static std::vector< std::pair<unsigned, unsigned> > getAspects();
};

#endif /* RESOLUTIONS_HPP_ */
