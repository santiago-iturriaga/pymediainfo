/*
 * ISO9660Reader.h
 *
 *  Created on: Jun 23, 2009
 *      Author: santiago
 */

#ifndef ISO9660READER_H_
#define ISO9660READER_H_

#include <iostream>
#include <string.h>

namespace imediafinder {

class ISO9660Reader {
public:
	ISO9660Reader(std::string dev_name);
	virtual ~ISO9660Reader();

	std::string GetLabel();
	std::string GetDeviceName();
private:
	bool _loaded;
	std::string _dev_name;
	std::string _label;

	void Load();
};

}

#endif /* ISO9660READER_H_ */
