/*****************************************************************************
*
* Copyright (c) 2000 - 2013, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-442911
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

#include <PyFFTAttributes.h>
#include <ObserverToCallback.h>
#include <stdio.h>
#include <snprintf.h>

// ****************************************************************************
// Module: PyFFTAttributes
//
// Purpose: 
//   This class contains attributes for the FFT operator.
//
// Note:       Autogenerated by xml2python. Do not modify by hand!
//
// Programmer: xml2python
// Creation:   omitted
//
// ****************************************************************************

//
// This struct contains the Python type information and a FFTAttributes.
//
struct FFTAttributesObject
{
    PyObject_HEAD
    FFTAttributes *data;
    bool        owns;
    PyObject   *parent;
};

//
// Internal prototypes
//
static PyObject *NewFFTAttributes(int);

std::string
PyFFTAttributes_ToString(const FFTAttributes *atts, const char *prefix)
{
    std::string str; 
    char tmpStr[1000]; 

    SNPRINTF(tmpStr, 1000, "%sdummy = %d\n", prefix, atts->GetDummy());
    str += tmpStr;
    return str;
}

static PyObject *
FFTAttributes_Notify(PyObject *self, PyObject *args)
{
    FFTAttributesObject *obj = (FFTAttributesObject *)self;
    obj->data->Notify();
    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
FFTAttributes_SetDummy(PyObject *self, PyObject *args)
{
    FFTAttributesObject *obj = (FFTAttributesObject *)self;

    int ival;
    if(!PyArg_ParseTuple(args, "i", &ival))
        return NULL;

    // Set the dummy in the object.
    obj->data->SetDummy((int)ival);

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
FFTAttributes_GetDummy(PyObject *self, PyObject *args)
{
    FFTAttributesObject *obj = (FFTAttributesObject *)self;
    PyObject *retval = PyInt_FromLong(long(obj->data->GetDummy()));
    return retval;
}



PyMethodDef PyFFTAttributes_methods[FFTATTRIBUTES_NMETH] = {
    {"Notify", FFTAttributes_Notify, METH_VARARGS},
    {"SetDummy", FFTAttributes_SetDummy, METH_VARARGS},
    {"GetDummy", FFTAttributes_GetDummy, METH_VARARGS},
    {NULL, NULL}
};

//
// Type functions
//

static void
FFTAttributes_dealloc(PyObject *v)
{
   FFTAttributesObject *obj = (FFTAttributesObject *)v;
   if(obj->parent != 0)
       Py_DECREF(obj->parent);
   if(obj->owns)
       delete obj->data;
}

static int
FFTAttributes_compare(PyObject *v, PyObject *w)
{
    FFTAttributes *a = ((FFTAttributesObject *)v)->data;
    FFTAttributes *b = ((FFTAttributesObject *)w)->data;
    return (*a == *b) ? 0 : -1;
}

PyObject *
PyFFTAttributes_getattr(PyObject *self, char *name)
{
    if(strcmp(name, "dummy") == 0)
        return FFTAttributes_GetDummy(self, NULL);

    return Py_FindMethod(PyFFTAttributes_methods, self, name);
}

int
PyFFTAttributes_setattr(PyObject *self, char *name, PyObject *args)
{
    // Create a tuple to contain the arguments since all of the Set
    // functions expect a tuple.
    PyObject *tuple = PyTuple_New(1);
    PyTuple_SET_ITEM(tuple, 0, args);
    Py_INCREF(args);
    PyObject *obj = NULL;

    if(strcmp(name, "dummy") == 0)
        obj = FFTAttributes_SetDummy(self, tuple);

    if(obj != NULL)
        Py_DECREF(obj);

    Py_DECREF(tuple);
    if( obj == NULL)
        PyErr_Format(PyExc_RuntimeError, "Unable to set unknown attribute: '%s'", name);
    return (obj != NULL) ? 0 : -1;
}

static int
FFTAttributes_print(PyObject *v, FILE *fp, int flags)
{
    FFTAttributesObject *obj = (FFTAttributesObject *)v;
    fprintf(fp, "%s", PyFFTAttributes_ToString(obj->data, "").c_str());
    return 0;
}

PyObject *
FFTAttributes_str(PyObject *v)
{
    FFTAttributesObject *obj = (FFTAttributesObject *)v;
    return PyString_FromString(PyFFTAttributes_ToString(obj->data,"").c_str());
}

//
// The doc string for the class.
//
#if PY_MAJOR_VERSION > 2 || (PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION >= 5)
static const char *FFTAttributes_Purpose = "This class contains attributes for the FFT operator.";
#else
static char *FFTAttributes_Purpose = "This class contains attributes for the FFT operator.";
#endif

//
// The type description structure
//
static PyTypeObject FFTAttributesType =
{
    //
    // Type header
    //
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                   // ob_size
    "FFTAttributes",                    // tp_name
    sizeof(FFTAttributesObject),        // tp_basicsize
    0,                                   // tp_itemsize
    //
    // Standard methods
    //
    (destructor)FFTAttributes_dealloc,  // tp_dealloc
    (printfunc)FFTAttributes_print,     // tp_print
    (getattrfunc)PyFFTAttributes_getattr, // tp_getattr
    (setattrfunc)PyFFTAttributes_setattr, // tp_setattr
    (cmpfunc)FFTAttributes_compare,     // tp_compare
    (reprfunc)0,                         // tp_repr
    //
    // Type categories
    //
    0,                                   // tp_as_number
    0,                                   // tp_as_sequence
    0,                                   // tp_as_mapping
    //
    // More methods
    //
    0,                                   // tp_hash
    0,                                   // tp_call
    (reprfunc)FFTAttributes_str,        // tp_str
    0,                                   // tp_getattro
    0,                                   // tp_setattro
    0,                                   // tp_as_buffer
    Py_TPFLAGS_CHECKTYPES,               // tp_flags
    FFTAttributes_Purpose,              // tp_doc
    0,                                   // tp_traverse
    0,                                   // tp_clear
    0,                                   // tp_richcompare
    0                                    // tp_weaklistoffset
};

//
// Helper functions for object allocation.
//

static FFTAttributes *defaultAtts = 0;
static FFTAttributes *currentAtts = 0;

static PyObject *
NewFFTAttributes(int useCurrent)
{
    FFTAttributesObject *newObject;
    newObject = PyObject_NEW(FFTAttributesObject, &FFTAttributesType);
    if(newObject == NULL)
        return NULL;
    if(useCurrent && currentAtts != 0)
        newObject->data = new FFTAttributes(*currentAtts);
    else if(defaultAtts != 0)
        newObject->data = new FFTAttributes(*defaultAtts);
    else
        newObject->data = new FFTAttributes;
    newObject->owns = true;
    newObject->parent = 0;
    return (PyObject *)newObject;
}

static PyObject *
WrapFFTAttributes(const FFTAttributes *attr)
{
    FFTAttributesObject *newObject;
    newObject = PyObject_NEW(FFTAttributesObject, &FFTAttributesType);
    if(newObject == NULL)
        return NULL;
    newObject->data = (FFTAttributes *)attr;
    newObject->owns = false;
    newObject->parent = 0;
    return (PyObject *)newObject;
}

///////////////////////////////////////////////////////////////////////////////
//
// Interface that is exposed to the VisIt module.
//
///////////////////////////////////////////////////////////////////////////////

PyObject *
FFTAttributes_new(PyObject *self, PyObject *args)
{
    int useCurrent = 0;
    if (!PyArg_ParseTuple(args, "i", &useCurrent))
    {
        if (!PyArg_ParseTuple(args, ""))
            return NULL;
        else
            PyErr_Clear();
    }

    return (PyObject *)NewFFTAttributes(useCurrent);
}

//
// Plugin method table. These methods are added to the visitmodule's methods.
//
static PyMethodDef FFTAttributesMethods[] = {
    {"FFTAttributes", FFTAttributes_new, METH_VARARGS},
    {NULL,      NULL}        /* Sentinel */
};

static Observer *FFTAttributesObserver = 0;

std::string
PyFFTAttributes_GetLogString()
{
    std::string s("FFTAtts = FFTAttributes()\n");
    if(currentAtts != 0)
        s += PyFFTAttributes_ToString(currentAtts, "FFTAtts.");
    return s;
}

static void
PyFFTAttributes_CallLogRoutine(Subject *subj, void *data)
{
    FFTAttributes *atts = (FFTAttributes *)subj;
    typedef void (*logCallback)(const std::string &);
    logCallback cb = (logCallback)data;

    if(cb != 0)
    {
        std::string s("FFTAtts = FFTAttributes()\n");
        s += PyFFTAttributes_ToString(currentAtts, "FFTAtts.");
        cb(s);
    }
}

void
PyFFTAttributes_StartUp(FFTAttributes *subj, void *data)
{
    if(subj == 0)
        return;

    currentAtts = subj;
    PyFFTAttributes_SetDefaults(subj);

    //
    // Create the observer that will be notified when the attributes change.
    //
    if(FFTAttributesObserver == 0)
    {
        FFTAttributesObserver = new ObserverToCallback(subj,
            PyFFTAttributes_CallLogRoutine, (void *)data);
    }

}

void
PyFFTAttributes_CloseDown()
{
    delete defaultAtts;
    defaultAtts = 0;
    delete FFTAttributesObserver;
    FFTAttributesObserver = 0;
}

PyMethodDef *
PyFFTAttributes_GetMethodTable(int *nMethods)
{
    *nMethods = 1;
    return FFTAttributesMethods;
}

bool
PyFFTAttributes_Check(PyObject *obj)
{
    return (obj->ob_type == &FFTAttributesType);
}

FFTAttributes *
PyFFTAttributes_FromPyObject(PyObject *obj)
{
    FFTAttributesObject *obj2 = (FFTAttributesObject *)obj;
    return obj2->data;
}

PyObject *
PyFFTAttributes_New()
{
    return NewFFTAttributes(0);
}

PyObject *
PyFFTAttributes_Wrap(const FFTAttributes *attr)
{
    return WrapFFTAttributes(attr);
}

void
PyFFTAttributes_SetParent(PyObject *obj, PyObject *parent)
{
    FFTAttributesObject *obj2 = (FFTAttributesObject *)obj;
    obj2->parent = parent;
}

void
PyFFTAttributes_SetDefaults(const FFTAttributes *atts)
{
    if(defaultAtts)
        delete defaultAtts;

    defaultAtts = new FFTAttributes(*atts);
}

