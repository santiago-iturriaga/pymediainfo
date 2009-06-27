/*
 * ISO9660Reader.cpp
 *
 *  Created on: Jun 23, 2009
 *      Author: santiago
 */

#include "ISO9660Reader.h"

#include <iostream>
#include <fcntl.h>
using namespace std;

#include "Utils.h"

#define ISO9660_LABEL_SEEK		32808
#define ISO9660_LABEL_LENGTH	32

namespace imediafinder {

ISO9660Reader::ISO9660Reader(string dev_name) {
	this->_dev_name = dev_name;
	this->_label = "";
	this->_loaded = false;
}

ISO9660Reader::~ISO9660Reader() {

}

string ISO9660Reader::GetDeviceName() {
	return this->_dev_name;
}

string ISO9660Reader::GetLabel() {
	if (!this->_loaded) Load();
	return this->_label;
}

void ISO9660Reader::Load() {
	if (!this->_loaded) {
		char label_buff[ISO9660_LABEL_LENGTH];
		memset(label_buff, '\0', ISO9660_LABEL_LENGTH);

		// Leo el label desde el medio
		int fd = open(this->_dev_name.c_str(), O_RDONLY);
		if (fd != -1) {
			int status = lseek(fd, ISO9660_LABEL_SEEK, SEEK_SET);
			int size;
			if (status != -1)
				size = read(fd, label_buff, ISO9660_LABEL_LENGTH);

			close(fd);
		}

		this->_label = Utils::CleanString(label_buff, ISO9660_LABEL_LENGTH);

#ifdef __DEBUG__
		cout << "orig: " << label_buff << endl;
		cout << "new: " << this->_label << endl;
#endif
	}
}

}
