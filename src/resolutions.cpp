/*
 * resolutions.cpp
 *
 *  Created on: 2 de out de 2017
 *      Author: carlosfaruolo
 */

#include "resolutions.hpp"

using std::vector;
using std::pair;

#define aspectRatio(x, y) std::make_pair(x, y)

Resolution::Resolution(unsigned width, unsigned height, std::pair<unsigned, unsigned> aspect, std::string description)
: width(width), height(height), aspect(aspect), description(description)
{}

vector<Resolution> Resolution::get(pair<unsigned, unsigned> requestedAspect)
{
	const pair<unsigned, unsigned> undecidedAspect = std::make_pair(0, 0);
	pair<unsigned, unsigned> aspect;

	vector<Resolution> resolutions;

	aspect = aspectRatio(4, 3);
	if(aspect == requestedAspect or requestedAspect == undecidedAspect)
	{
		resolutions.push_back(Resolution( 320,  240, aspect, "QVGA"));
		resolutions.push_back(Resolution( 400,  300, aspect, "qSVGA"));
		resolutions.push_back(Resolution( 480,  360, aspect, "HVGA"));
		resolutions.push_back(Resolution( 512,  384, aspect, "Macintosh Color Classic"));
		resolutions.push_back(Resolution( 640,  480, aspect, "VGA / SD"));
		resolutions.push_back(Resolution( 800,  600, aspect, "SVGA / UVGA"));
		resolutions.push_back(Resolution( 832,  624, aspect, "SVGA, Macintosh LC III"));
		resolutions.push_back(Resolution( 960,  720, aspect, "720p DVCPRO HD"));
		resolutions.push_back(Resolution(1024,  768, aspect, "XGA"));
		resolutions.push_back(Resolution(1152,  864, aspect, "XGA+"));
		resolutions.push_back(Resolution(1200,  900, aspect, ""));
		resolutions.push_back(Resolution(1280,  960, aspect, "SXGA- / UVGA"));
		resolutions.push_back(Resolution(1360, 1020, aspect, ""));
		resolutions.push_back(Resolution(1400, 1050, aspect, "SXGA+"));
		resolutions.push_back(Resolution(1440, 1080, aspect, "HDV 1080i"));
		resolutions.push_back(Resolution(1600, 1200, aspect, "UXGA"));
		resolutions.push_back(Resolution(1920, 1440, aspect, ""));
		resolutions.push_back(Resolution(2048, 1536, aspect, "QXGA"));
		resolutions.push_back(Resolution(2560, 1920, aspect, "max. CRT"));
		resolutions.push_back(Resolution(2800, 2100, aspect, "QSXGA+"));
		resolutions.push_back(Resolution(3200, 2400, aspect, "QUXGA"));
		resolutions.push_back(Resolution(4096, 3072, aspect, "HXGA"));
		resolutions.push_back(Resolution(6400, 4800, aspect, "HUXGA"));
	}

	aspect = aspectRatio(16, 9);
	if(aspect == requestedAspect or requestedAspect == undecidedAspect)
	{
		resolutions.push_back(Resolution( 640,  360, aspect, "nHD"));
		resolutions.push_back(Resolution( 480,  270, aspect, "HVGA"));
		resolutions.push_back(Resolution( 854,  480, aspect, "FWVGA"));
		resolutions.push_back(Resolution( 960,  540, aspect, "qHD"));
		resolutions.push_back(Resolution(1024,  576, aspect, "WSVGA"));
		resolutions.push_back(Resolution(1280,  720, aspect, "HD / 720p / WXGA-H"));
		resolutions.push_back(Resolution(1360,  768, aspect, "WXGA*"));
		resolutions.push_back(Resolution(1366,  768, aspect, "WXGA"));
		resolutions.push_back(Resolution(1600,  900, aspect, "HD+ / 900p"));
		resolutions.push_back(Resolution(1920, 1080, aspect, "Full HD / 1080p"));
		resolutions.push_back(Resolution(2048, 1152, aspect, "QWXGA"));
		resolutions.push_back(Resolution(2560, 1440, aspect, "QHD / WQHD / 1440p"));
		resolutions.push_back(Resolution(3200, 1800, aspect, "QHD+ / WQXGA+"));
		resolutions.push_back(Resolution(3840, 2160, aspect, "4K UHD"));
		resolutions.push_back(Resolution(5120, 2880, aspect, "5K/UHD+"));
		resolutions.push_back(Resolution(7680, 4320, aspect, "8K UHD"));
	}

	aspect = aspectRatio(16, 10);
	if(aspect == requestedAspect or requestedAspect == undecidedAspect)
	{
		resolutions.push_back(Resolution( 320,  200, aspect, "CGA"));
		resolutions.push_back(Resolution( 384,  240, aspect, "WQVGA"));
		resolutions.push_back(Resolution( 768,  480, aspect, "WVGA"));
		resolutions.push_back(Resolution(1280,  800, aspect, "WXGA"));
		resolutions.push_back(Resolution(1440,  900, aspect, "WXGA+ / WSXGA"));
		resolutions.push_back(Resolution(1680, 1050, aspect, "WSXGA+"));
		resolutions.push_back(Resolution(1920, 1200, aspect, "WUXGA"));
		resolutions.push_back(Resolution(2560, 1600, aspect, "WQXGA"));
		resolutions.push_back(Resolution(3840, 2400, aspect, "WQUXGA"));
		resolutions.push_back(Resolution(5120, 3200, aspect, "WHXGA"));
		resolutions.push_back(Resolution(7680, 4800, aspect, "WHUXGA"));
	}

	aspect = aspectRatio(3, 2);
	if(aspect == requestedAspect or requestedAspect == undecidedAspect)
	{
		resolutions.push_back(Resolution(360, 240, aspect, "WQVGA"));
		resolutions.push_back(Resolution(480, 320, aspect, "HVGA"));
		resolutions.push_back(Resolution(720, 480, aspect, "WVGA"));
		resolutions.push_back(Resolution(960, 640, aspect, "DVGA"));
		resolutions.push_back(Resolution(2160, 1440, aspect, "Full HD+"));
	}

	aspect = aspectRatio(5, 3);
	if(aspect == requestedAspect or requestedAspect == undecidedAspect)
	{
		resolutions.push_back(Resolution(400, 240, aspect, "WQVGA"));
		resolutions.push_back(Resolution(800, 480, aspect, "WVGA"));
		resolutions.push_back(Resolution(1280, 768, aspect, "WXGA"));
	}

	aspect = aspectRatio(5, 4);
	if(aspect == requestedAspect or requestedAspect == undecidedAspect)
	{
		resolutions.push_back(Resolution(1280, 1024, aspectRatio( 5,  4), "SXGA"));
		resolutions.push_back(Resolution(2560, 2048, aspectRatio( 5,  4), "QSXGA"));
		resolutions.push_back(Resolution(5120, 4096, aspectRatio( 5,  4), "HSXGA"));
	}

	if(requestedAspect == undecidedAspect)
	{
		resolutions.push_back(Resolution( 432,  240, aspectRatio( 9,  5), "WQVGA"));
		resolutions.push_back(Resolution( 640,  350, aspectRatio(64, 35), "EGA"));
		resolutions.push_back(Resolution( 720,  348, aspectRatio(60, 29), "HGC"));
		resolutions.push_back(Resolution(1600,  768, aspectRatio(25, 12), "UWXGA"));
		resolutions.push_back(Resolution(1920, 1400, aspectRatio( 7,  5), "TXGA"));
		resolutions.push_back(Resolution(2048, 1080, aspectRatio(19, 10), "DCI 2K"));
		resolutions.push_back(Resolution(2560, 1080, aspectRatio(64, 27), "UW-UXGA"));
		resolutions.push_back(Resolution(3200, 2048, aspectRatio(25, 16), "WQSXGA"));
		resolutions.push_back(Resolution(3440, 1440, aspectRatio(43, 18), "UWQHD"));
		resolutions.push_back(Resolution(3840, 1600, aspectRatio(12,  5), "UW4K"));
		resolutions.push_back(Resolution(4096, 2160, aspectRatio(19, 10), "DCI 4K"));
		resolutions.push_back(Resolution(5120, 2160, aspectRatio(64, 27), "UW5K"));
		resolutions.push_back(Resolution(6400, 4096, aspectRatio(25, 16), "WHSXGA"));
		resolutions.push_back(Resolution(10240, 4320, aspectRatio(21, 9), "UW10K"));
	}

	aspect = std::make_pair(0, 0);
	if(aspect == requestedAspect or requestedAspect == undecidedAspect)
	{
		resolutions.push_back(Resolution(0000, 0000, aspect, ""));
	}

	return resolutions;
}
