//============================================================================
// Name        : prueba.cpp
// Author      : Santiago Iturriaga
// Version     :
// Copyright   :
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <list>
using namespace std;

#include "Finder.h"
using namespace imediafinder;

int main() {
	list<MediaInfo> mediaAvailable = Finder::Find();

	cout << endl << "List:" << endl;
	list<MediaInfo>::iterator iter = mediaAvailable.begin();

	while (iter != mediaAvailable.end()) {
		MediaInfo media = *iter;
		cout << ">> " << media.device_name << endl;
		cout << "   " << media.format_type << endl;
		cout << "   " << media.label << endl;
		cout << "   " << media.mount_path << endl;
		iter++;
	}
	cout << endl;

	return 0;
}
