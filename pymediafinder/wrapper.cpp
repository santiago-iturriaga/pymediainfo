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

	Py_RETURN_NONE;
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
