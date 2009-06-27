/*
 * wrapper.cpp
 *
 *  Created on: Jun 25, 2009
 *      Author: santiago
 */

#include <python2.6/Python.h>

#include <iostream>
#include <list>
using namespace std;

#include "Finder.h"
using namespace imediafinder;

static PyObject* pymediafinder_find(PyObject *self, PyObject *args)
{
	list<MediaInfo> result;
	result = Finder::Find();

	PyObject *py_result = PyList_New(result.size());

	list<MediaInfo>::iterator iter = result.begin();
	int pos = 0;

	while (iter != result.end()) {
		MediaInfo current = *iter;
		PyObject *tuple = Py_BuildValue("(ssss)", current.device_name.c_str(), current.mount_path.c_str(), current.format_type.c_str(), current.label.c_str());
		PyList_SetItem(py_result, pos, tuple);

		pos++;
		iter++;
	}

	//Py_RETURN_NONE;
	return py_result;
}

static PyMethodDef PyMediaFinderMethods[] = {
    {"find",  pymediafinder_find, METH_VARARGS,
     "Find available media devices."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC initpymediafinder(void)
{
    (void) Py_InitModule("pymediafinder", PyMediaFinderMethods);
}
