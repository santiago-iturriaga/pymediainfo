/*
 * Finder.cpp
 *
 *  Created on: Jun 23, 2009
 *      Author: santiago
 */

#include "Finder.h"

#include <iostream>
#include <list>
using namespace std;

#include "UDFReader.h"
#include "ISO9660Reader.h"

extern "C" {
#include "mountlist.h"
}

#define ISO9660					"iso9660"
#define UDF 					"udf"

#define MAX_DEV_NAME_LENGTH 	255

namespace imediafinder {

Finder::Finder() {

}

Finder::~Finder() {

}

list<MediaInfo> Finder::Find() {
	list<MediaInfo> *result = new list<MediaInfo> ();

	struct mount_entry *mount_list;
	struct mount_entry *current;

	mount_list = read_file_system_list(1);

	if (mount_list != NULL) {
		for (current = mount_list; current; current = current->me_next) {
			if (!current->me_dummy && !current->me_remote) {
				string type = string(current->me_type);
				string dev_name = string(current->me_devname);
				string mount_path = string(current->me_mountdir);
				string label = "";

				if ((type.compare(ISO9660) == 0) || (type.compare(UDF) == 0)) {

					if (type.compare(ISO9660) == 0) {
						// El formato del medio es ISO9660
						ISO9660Reader reader = ISO9660Reader(dev_name);
						label = reader.GetLabel();
					} else if (type.compare(UDF) == 0) {
						// El formato del medio es UDF
						UDFReader reader = UDFReader(dev_name);
						label = reader.GetLabel();
					}

#ifdef __DEBUG__
					cout << endl << "dev_name: " << dev_name << endl;
					cout << "mountdir: " << current->me_mountdir << endl;
					cout << "type: " << type << endl;
					cout << "label: " << label << endl << endl;
#endif

					MediaInfo *info = new MediaInfo();
					info->device_name = dev_name;
					info->format_type = type;
					info->mount_path = mount_path;
					info->label = label;

					result->push_back(*info);
				} else {
#ifdef __DEBUG__
					cout << "ignored: " << type << endl;
#endif
				}
			}
		}
	}

	return *result;
}

}
