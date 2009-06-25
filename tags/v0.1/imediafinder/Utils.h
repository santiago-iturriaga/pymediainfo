/*
 * Utils.h
 *
 *  Created on: Jun 24, 2009
 *      Author: santiago
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>

namespace imediafinder {
using namespace std;

class Utils {
public:
	/*
	 * Duplica un string quitando los caracteres ASCII no reprecentables.
	 */
	static string CleanString(char *array, int length);

private:
	Utils();
	virtual ~Utils();
};

}

#endif /* UTILS_H_ */
