/*
 * Finder.h
 *
 *  Created on: Jun 23, 2009
 *      Author: santiago
 */

#ifndef FINDER_H_
#define FINDER_H_

#include <iostream>
#include <list>

extern "C" {
#include "mountlist.h"
}

namespace imediafinder {

struct MediaInfo {
	std::string device_name;
	std::string mount_path;
	std::string format_type;
	std::string label;
};

class Finder {
public:
	Finder();
	virtual ~Finder();

	static std::list<MediaInfo> Find();
};

}

#endif /* FINDER_H_ */
