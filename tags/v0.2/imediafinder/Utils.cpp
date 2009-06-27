/*
 * Utils.cpp
 *
 *  Created on: Jun 24, 2009
 *      Author: santiago
 */

#include "Utils.h"

namespace imediafinder {

Utils::Utils() {

}

Utils::~Utils() {

}


string Utils::CleanString(char array[], int length) {
	unsigned int temp_ascii;
	char temp_char;

	string aux = "";

	for (int pos = 0; pos < length; pos++) {
		temp_ascii = (uint8_t) (array[pos]);
		temp_char = array[pos];

#ifdef __DEBUG__
		cout << "temp_ascii: " << temp_ascii << endl;
		cout << "temp_char: " << temp_char << endl;
#endif

		if (temp_ascii > 32) {
			aux += temp_char;
		}
	}

#ifdef __DEBUG__
		cout << "result string: " << aux << endl;
#endif

	return aux;
}

}
