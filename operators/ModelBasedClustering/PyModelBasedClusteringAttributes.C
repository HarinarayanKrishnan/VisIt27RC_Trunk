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

#include <PyModelBasedClusteringAttributes.h>
#include <ObserverToCallback.h>
#include <stdio.h>
#include <snprintf.h>

// ****************************************************************************
// Module: PyModelBasedClusteringAttributes
//
// Purpose: 
//   ModelBasedClustering Attributes
//
// Note:       Autogenerated by xml2python. Do not modify by hand!
//
// Programmer: xml2python
// Creation:   omitted
//
// ****************************************************************************

//
// This struct contains the Python type information and a ModelBasedClusteringAttributes.
//
struct ModelBasedClusteringAttributesObject
{
    PyObject_HEAD
    ModelBasedClusteringAttributes *data;
    bool        owns;
    PyObject   *parent;
};

//
// Internal prototypes
//
static PyObject *NewModelBasedClusteringAttributes(int);

std::string
PyModelBasedClusteringAttributes_ToString(const ModelBasedClusteringAttributes *atts, const char *prefix)
{
    std::string str; 
    char tmpStr[1000]; 

    SNPRINTF(tmpStr, 1000, "%snumClusters = %d\n", prefix, atts->GetNumClusters());
    str += tmpStr;
    const char *clusterMethod_names = "EM, ACEM, APECM, APECMA, K_MEANS";
    switch (atts->GetClusterMethod())
    {
      case ModelBasedClusteringAttributes::EM:
          SNPRINTF(tmpStr, 1000, "%sclusterMethod = %sEM  # %s\n", prefix, prefix, clusterMethod_names);
          str += tmpStr;
          break;
      case ModelBasedClusteringAttributes::ACEM:
          SNPRINTF(tmpStr, 1000, "%sclusterMethod = %sACEM  # %s\n", prefix, prefix, clusterMethod_names);
          str += tmpStr;
          break;
      case ModelBasedClusteringAttributes::APECM:
          SNPRINTF(tmpStr, 1000, "%sclusterMethod = %sAPECM  # %s\n", prefix, prefix, clusterMethod_names);
          str += tmpStr;
          break;
      case ModelBasedClusteringAttributes::APECMA:
          SNPRINTF(tmpStr, 1000, "%sclusterMethod = %sAPECMA  # %s\n", prefix, prefix, clusterMethod_names);
          str += tmpStr;
          break;
      case ModelBasedClusteringAttributes::K_MEANS:
          SNPRINTF(tmpStr, 1000, "%sclusterMethod = %sK_MEANS  # %s\n", prefix, prefix, clusterMethod_names);
          str += tmpStr;
          break;
      default:
          break;
    }

    SNPRINTF(tmpStr, 1000, "%sthreshold = %g\n", prefix, atts->GetThreshold());
    str += tmpStr;
    SNPRINTF(tmpStr, 1000, "%spercentile = %g\n", prefix, atts->GetPercentile());
    str += tmpStr;
    SNPRINTF(tmpStr, 1000, "%svar2 = \"%s\"\n", prefix, atts->GetVar2().c_str());
    str += tmpStr;
    SNPRINTF(tmpStr, 1000, "%svar3 = \"%s\"\n", prefix, atts->GetVar3().c_str());
    str += tmpStr;
    return str;
}

static PyObject *
ModelBasedClusteringAttributes_Notify(PyObject *self, PyObject *args)
{
    ModelBasedClusteringAttributesObject *obj = (ModelBasedClusteringAttributesObject *)self;
    obj->data->Notify();
    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ModelBasedClusteringAttributes_SetNumClusters(PyObject *self, PyObject *args)
{
    ModelBasedClusteringAttributesObject *obj = (ModelBasedClusteringAttributesObject *)self;

    int ival;
    if(!PyArg_ParseTuple(args, "i", &ival))
        return NULL;

    // Set the numClusters in the object.
    obj->data->SetNumClusters((int)ival);

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ModelBasedClusteringAttributes_GetNumClusters(PyObject *self, PyObject *args)
{
    ModelBasedClusteringAttributesObject *obj = (ModelBasedClusteringAttributesObject *)self;
    PyObject *retval = PyInt_FromLong(long(obj->data->GetNumClusters()));
    return retval;
}

/*static*/ PyObject *
ModelBasedClusteringAttributes_SetClusterMethod(PyObject *self, PyObject *args)
{
    ModelBasedClusteringAttributesObject *obj = (ModelBasedClusteringAttributesObject *)self;

    int ival;
    if(!PyArg_ParseTuple(args, "i", &ival))
        return NULL;

    // Set the clusterMethod in the object.
    if(ival >= 0 && ival < 5)
        obj->data->SetClusterMethod(ModelBasedClusteringAttributes::clusterAlgorithmType(ival));
    else
    {
        fprintf(stderr, "An invalid clusterMethod value was given. "
                        "Valid values are in the range of [0,4]. "
                        "You can also use the following names: "
                        "EM, ACEM, APECM, APECMA, K_MEANS"
                        ".");
        return NULL;
    }

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ModelBasedClusteringAttributes_GetClusterMethod(PyObject *self, PyObject *args)
{
    ModelBasedClusteringAttributesObject *obj = (ModelBasedClusteringAttributesObject *)self;
    PyObject *retval = PyInt_FromLong(long(obj->data->GetClusterMethod()));
    return retval;
}

/*static*/ PyObject *
ModelBasedClusteringAttributes_SetThreshold(PyObject *self, PyObject *args)
{
    ModelBasedClusteringAttributesObject *obj = (ModelBasedClusteringAttributesObject *)self;

    double dval;
    if(!PyArg_ParseTuple(args, "d", &dval))
        return NULL;

    // Set the threshold in the object.
    obj->data->SetThreshold(dval);

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ModelBasedClusteringAttributes_GetThreshold(PyObject *self, PyObject *args)
{
    ModelBasedClusteringAttributesObject *obj = (ModelBasedClusteringAttributesObject *)self;
    PyObject *retval = PyFloat_FromDouble(obj->data->GetThreshold());
    return retval;
}

/*static*/ PyObject *
ModelBasedClusteringAttributes_SetPercentile(PyObject *self, PyObject *args)
{
    ModelBasedClusteringAttributesObject *obj = (ModelBasedClusteringAttributesObject *)self;

    double dval;
    if(!PyArg_ParseTuple(args, "d", &dval))
        return NULL;

    // Set the percentile in the object.
    obj->data->SetPercentile(dval);

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ModelBasedClusteringAttributes_GetPercentile(PyObject *self, PyObject *args)
{
    ModelBasedClusteringAttributesObject *obj = (ModelBasedClusteringAttributesObject *)self;
    PyObject *retval = PyFloat_FromDouble(obj->data->GetPercentile());
    return retval;
}

/*static*/ PyObject *
ModelBasedClusteringAttributes_SetVar2(PyObject *self, PyObject *args)
{
    ModelBasedClusteringAttributesObject *obj = (ModelBasedClusteringAttributesObject *)self;

    char *str;
    if(!PyArg_ParseTuple(args, "s", &str))
        return NULL;

    // Set the var2 in the object.
    obj->data->SetVar2(std::string(str));

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ModelBasedClusteringAttributes_GetVar2(PyObject *self, PyObject *args)
{
    ModelBasedClusteringAttributesObject *obj = (ModelBasedClusteringAttributesObject *)self;
    PyObject *retval = PyString_FromString(obj->data->GetVar2().c_str());
    return retval;
}

/*static*/ PyObject *
ModelBasedClusteringAttributes_SetVar3(PyObject *self, PyObject *args)
{
    ModelBasedClusteringAttributesObject *obj = (ModelBasedClusteringAttributesObject *)self;

    char *str;
    if(!PyArg_ParseTuple(args, "s", &str))
        return NULL;

    // Set the var3 in the object.
    obj->data->SetVar3(std::string(str));

    Py_INCREF(Py_None);
    return Py_None;
}

/*static*/ PyObject *
ModelBasedClusteringAttributes_GetVar3(PyObject *self, PyObject *args)
{
    ModelBasedClusteringAttributesObject *obj = (ModelBasedClusteringAttributesObject *)self;
    PyObject *retval = PyString_FromString(obj->data->GetVar3().c_str());
    return retval;
}



PyMethodDef PyModelBasedClusteringAttributes_methods[MODELBASEDCLUSTERINGATTRIBUTES_NMETH] = {
    {"Notify", ModelBasedClusteringAttributes_Notify, METH_VARARGS},
    {"SetNumClusters", ModelBasedClusteringAttributes_SetNumClusters, METH_VARARGS},
    {"GetNumClusters", ModelBasedClusteringAttributes_GetNumClusters, METH_VARARGS},
    {"SetClusterMethod", ModelBasedClusteringAttributes_SetClusterMethod, METH_VARARGS},
    {"GetClusterMethod", ModelBasedClusteringAttributes_GetClusterMethod, METH_VARARGS},
    {"SetThreshold", ModelBasedClusteringAttributes_SetThreshold, METH_VARARGS},
    {"GetThreshold", ModelBasedClusteringAttributes_GetThreshold, METH_VARARGS},
    {"SetPercentile", ModelBasedClusteringAttributes_SetPercentile, METH_VARARGS},
    {"GetPercentile", ModelBasedClusteringAttributes_GetPercentile, METH_VARARGS},
    {"SetVar2", ModelBasedClusteringAttributes_SetVar2, METH_VARARGS},
    {"GetVar2", ModelBasedClusteringAttributes_GetVar2, METH_VARARGS},
    {"SetVar3", ModelBasedClusteringAttributes_SetVar3, METH_VARARGS},
    {"GetVar3", ModelBasedClusteringAttributes_GetVar3, METH_VARARGS},
    {NULL, NULL}
};

//
// Type functions
//

static void
ModelBasedClusteringAttributes_dealloc(PyObject *v)
{
   ModelBasedClusteringAttributesObject *obj = (ModelBasedClusteringAttributesObject *)v;
   if(obj->parent != 0)
       Py_DECREF(obj->parent);
   if(obj->owns)
       delete obj->data;
}

static int
ModelBasedClusteringAttributes_compare(PyObject *v, PyObject *w)
{
    ModelBasedClusteringAttributes *a = ((ModelBasedClusteringAttributesObject *)v)->data;
    ModelBasedClusteringAttributes *b = ((ModelBasedClusteringAttributesObject *)w)->data;
    return (*a == *b) ? 0 : -1;
}

PyObject *
PyModelBasedClusteringAttributes_getattr(PyObject *self, char *name)
{
    if(strcmp(name, "numClusters") == 0)
        return ModelBasedClusteringAttributes_GetNumClusters(self, NULL);
    if(strcmp(name, "clusterMethod") == 0)
        return ModelBasedClusteringAttributes_GetClusterMethod(self, NULL);
    if(strcmp(name, "EM") == 0)
        return PyInt_FromLong(long(ModelBasedClusteringAttributes::EM));
    if(strcmp(name, "ACEM") == 0)
        return PyInt_FromLong(long(ModelBasedClusteringAttributes::ACEM));
    if(strcmp(name, "APECM") == 0)
        return PyInt_FromLong(long(ModelBasedClusteringAttributes::APECM));
    if(strcmp(name, "APECMA") == 0)
        return PyInt_FromLong(long(ModelBasedClusteringAttributes::APECMA));
    if(strcmp(name, "K_MEANS") == 0)
        return PyInt_FromLong(long(ModelBasedClusteringAttributes::K_MEANS));

    if(strcmp(name, "threshold") == 0)
        return ModelBasedClusteringAttributes_GetThreshold(self, NULL);
    if(strcmp(name, "percentile") == 0)
        return ModelBasedClusteringAttributes_GetPercentile(self, NULL);
    if(strcmp(name, "var2") == 0)
        return ModelBasedClusteringAttributes_GetVar2(self, NULL);
    if(strcmp(name, "var3") == 0)
        return ModelBasedClusteringAttributes_GetVar3(self, NULL);

    return Py_FindMethod(PyModelBasedClusteringAttributes_methods, self, name);
}

int
PyModelBasedClusteringAttributes_setattr(PyObject *self, char *name, PyObject *args)
{
    // Create a tuple to contain the arguments since all of the Set
    // functions expect a tuple.
    PyObject *tuple = PyTuple_New(1);
    PyTuple_SET_ITEM(tuple, 0, args);
    Py_INCREF(args);
    PyObject *obj = NULL;

    if(strcmp(name, "numClusters") == 0)
        obj = ModelBasedClusteringAttributes_SetNumClusters(self, tuple);
    else if(strcmp(name, "clusterMethod") == 0)
        obj = ModelBasedClusteringAttributes_SetClusterMethod(self, tuple);
    else if(strcmp(name, "threshold") == 0)
        obj = ModelBasedClusteringAttributes_SetThreshold(self, tuple);
    else if(strcmp(name, "percentile") == 0)
        obj = ModelBasedClusteringAttributes_SetPercentile(self, tuple);
    else if(strcmp(name, "var2") == 0)
        obj = ModelBasedClusteringAttributes_SetVar2(self, tuple);
    else if(strcmp(name, "var3") == 0)
        obj = ModelBasedClusteringAttributes_SetVar3(self, tuple);

    if(obj != NULL)
        Py_DECREF(obj);

    Py_DECREF(tuple);
    if( obj == NULL)
        PyErr_Format(PyExc_RuntimeError, "Unable to set unknown attribute: '%s'", name);
    return (obj != NULL) ? 0 : -1;
}

static int
ModelBasedClusteringAttributes_print(PyObject *v, FILE *fp, int flags)
{
    ModelBasedClusteringAttributesObject *obj = (ModelBasedClusteringAttributesObject *)v;
    fprintf(fp, "%s", PyModelBasedClusteringAttributes_ToString(obj->data, "").c_str());
    return 0;
}

PyObject *
ModelBasedClusteringAttributes_str(PyObject *v)
{
    ModelBasedClusteringAttributesObject *obj = (ModelBasedClusteringAttributesObject *)v;
    return PyString_FromString(PyModelBasedClusteringAttributes_ToString(obj->data,"").c_str());
}

//
// The doc string for the class.
//
#if PY_MAJOR_VERSION > 2 || (PY_MAJOR_VERSION == 2 && PY_MINOR_VERSION >= 5)
static const char *ModelBasedClusteringAttributes_Purpose = "ModelBasedClustering Attributes";
#else
static char *ModelBasedClusteringAttributes_Purpose = "ModelBasedClustering Attributes";
#endif

//
// The type description structure
//
static PyTypeObject ModelBasedClusteringAttributesType =
{
    //
    // Type header
    //
    PyObject_HEAD_INIT(&PyType_Type)
    0,                                   // ob_size
    "ModelBasedClusteringAttributes",                    // tp_name
    sizeof(ModelBasedClusteringAttributesObject),        // tp_basicsize
    0,                                   // tp_itemsize
    //
    // Standard methods
    //
    (destructor)ModelBasedClusteringAttributes_dealloc,  // tp_dealloc
    (printfunc)ModelBasedClusteringAttributes_print,     // tp_print
    (getattrfunc)PyModelBasedClusteringAttributes_getattr, // tp_getattr
    (setattrfunc)PyModelBasedClusteringAttributes_setattr, // tp_setattr
    (cmpfunc)ModelBasedClusteringAttributes_compare,     // tp_compare
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
    (reprfunc)ModelBasedClusteringAttributes_str,        // tp_str
    0,                                   // tp_getattro
    0,                                   // tp_setattro
    0,                                   // tp_as_buffer
    Py_TPFLAGS_CHECKTYPES,               // tp_flags
    ModelBasedClusteringAttributes_Purpose,              // tp_doc
    0,                                   // tp_traverse
    0,                                   // tp_clear
    0,                                   // tp_richcompare
    0                                    // tp_weaklistoffset
};

//
// Helper functions for object allocation.
//

static ModelBasedClusteringAttributes *defaultAtts = 0;
static ModelBasedClusteringAttributes *currentAtts = 0;

static PyObject *
NewModelBasedClusteringAttributes(int useCurrent)
{
    ModelBasedClusteringAttributesObject *newObject;
    newObject = PyObject_NEW(ModelBasedClusteringAttributesObject, &ModelBasedClusteringAttributesType);
    if(newObject == NULL)
        return NULL;
    if(useCurrent && currentAtts != 0)
        newObject->data = new ModelBasedClusteringAttributes(*currentAtts);
    else if(defaultAtts != 0)
        newObject->data = new ModelBasedClusteringAttributes(*defaultAtts);
    else
        newObject->data = new ModelBasedClusteringAttributes;
    newObject->owns = true;
    newObject->parent = 0;
    return (PyObject *)newObject;
}

static PyObject *
WrapModelBasedClusteringAttributes(const ModelBasedClusteringAttributes *attr)
{
    ModelBasedClusteringAttributesObject *newObject;
    newObject = PyObject_NEW(ModelBasedClusteringAttributesObject, &ModelBasedClusteringAttributesType);
    if(newObject == NULL)
        return NULL;
    newObject->data = (ModelBasedClusteringAttributes *)attr;
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
ModelBasedClusteringAttributes_new(PyObject *self, PyObject *args)
{
    int useCurrent = 0;
    if (!PyArg_ParseTuple(args, "i", &useCurrent))
    {
        if (!PyArg_ParseTuple(args, ""))
            return NULL;
        else
            PyErr_Clear();
    }

    return (PyObject *)NewModelBasedClusteringAttributes(useCurrent);
}

//
// Plugin method table. These methods are added to the visitmodule's methods.
//
static PyMethodDef ModelBasedClusteringAttributesMethods[] = {
    {"ModelBasedClusteringAttributes", ModelBasedClusteringAttributes_new, METH_VARARGS},
    {NULL,      NULL}        /* Sentinel */
};

static Observer *ModelBasedClusteringAttributesObserver = 0;

std::string
PyModelBasedClusteringAttributes_GetLogString()
{
    std::string s("ModelBasedClusteringAtts = ModelBasedClusteringAttributes()\n");
    if(currentAtts != 0)
        s += PyModelBasedClusteringAttributes_ToString(currentAtts, "ModelBasedClusteringAtts.");
    return s;
}

static void
PyModelBasedClusteringAttributes_CallLogRoutine(Subject *subj, void *data)
{
    ModelBasedClusteringAttributes *atts = (ModelBasedClusteringAttributes *)subj;
    typedef void (*logCallback)(const std::string &);
    logCallback cb = (logCallback)data;

    if(cb != 0)
    {
        std::string s("ModelBasedClusteringAtts = ModelBasedClusteringAttributes()\n");
        s += PyModelBasedClusteringAttributes_ToString(currentAtts, "ModelBasedClusteringAtts.");
        cb(s);
    }
}

void
PyModelBasedClusteringAttributes_StartUp(ModelBasedClusteringAttributes *subj, void *data)
{
    if(subj == 0)
        return;

    currentAtts = subj;
    PyModelBasedClusteringAttributes_SetDefaults(subj);

    //
    // Create the observer that will be notified when the attributes change.
    //
    if(ModelBasedClusteringAttributesObserver == 0)
    {
        ModelBasedClusteringAttributesObserver = new ObserverToCallback(subj,
            PyModelBasedClusteringAttributes_CallLogRoutine, (void *)data);
    }

}

void
PyModelBasedClusteringAttributes_CloseDown()
{
    delete defaultAtts;
    defaultAtts = 0;
    delete ModelBasedClusteringAttributesObserver;
    ModelBasedClusteringAttributesObserver = 0;
}

PyMethodDef *
PyModelBasedClusteringAttributes_GetMethodTable(int *nMethods)
{
    *nMethods = 1;
    return ModelBasedClusteringAttributesMethods;
}

bool
PyModelBasedClusteringAttributes_Check(PyObject *obj)
{
    return (obj->ob_type == &ModelBasedClusteringAttributesType);
}

ModelBasedClusteringAttributes *
PyModelBasedClusteringAttributes_FromPyObject(PyObject *obj)
{
    ModelBasedClusteringAttributesObject *obj2 = (ModelBasedClusteringAttributesObject *)obj;
    return obj2->data;
}

PyObject *
PyModelBasedClusteringAttributes_New()
{
    return NewModelBasedClusteringAttributes(0);
}

PyObject *
PyModelBasedClusteringAttributes_Wrap(const ModelBasedClusteringAttributes *attr)
{
    return WrapModelBasedClusteringAttributes(attr);
}

void
PyModelBasedClusteringAttributes_SetParent(PyObject *obj, PyObject *parent)
{
    ModelBasedClusteringAttributesObject *obj2 = (ModelBasedClusteringAttributesObject *)obj;
    obj2->parent = parent;
}

void
PyModelBasedClusteringAttributes_SetDefaults(const ModelBasedClusteringAttributes *atts)
{
    if(defaultAtts)
        delete defaultAtts;

    defaultAtts = new ModelBasedClusteringAttributes(*atts);
}

