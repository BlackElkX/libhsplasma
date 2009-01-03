#include <Python.h>
#include <PRP/Avatar/plMatrixChannel.h>
#include "pyAGChannel.h"
#include "../pyCreatable.h"
#include "../../Math/pyGeometry3.h"

extern "C" {

static PyObject* pyMatrixChannel_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    pyMatrixChannel* self = (pyMatrixChannel*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->fThis = new plMatrixChannel();
        self->fPyOwned = true;
    }
    return (PyObject*)self;
}

static PyObject* pyMatrixChannel_Convert(PyObject*, PyObject* args) {
    pyCreatable* cre;
    if (!PyArg_ParseTuple(args, "O", &cre)) {
        PyErr_SetString(PyExc_TypeError, "Convert expects a plCreatable");
        return NULL;
    }
    if (!pyCreatable_Check((PyObject*)cre)) {
        PyErr_SetString(PyExc_TypeError, "Convert expects a plCreatable");
        return NULL;
    }
    return pyMatrixChannel_FromMatrixChannel(plMatrixChannel::Convert(cre->fThis));
}

static PyObject* pyMatrixChannel_getAffine(pyMatrixChannel* self, void*) {
    return pyAffineParts_FromAffineParts(self->fThis->getAffine());
}

static int pyMatrixChannel_setAffine(pyMatrixChannel* self, PyObject* value, void*) {
    if (value == NULL || !pyAffineParts_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "result should be an hsAffineParts");
        return -1;
    }
    self->fThis->setAffine(*((pyAffineParts*)value)->fThis);
    return 0;
}

static PyMethodDef pyMatrixChannel_Methods[] = {
    { "Convert", (PyCFunction)pyMatrixChannel_Convert, METH_VARARGS | METH_STATIC,
      "Convert a Creatable to a plMatrixChannel" },
    { NULL, NULL, 0, NULL }
};

static PyGetSetDef pyMatrixChannel_GetSet[] = {
    { "affine", (getter)pyMatrixChannel_getAffine, (setter)pyMatrixChannel_setAffine, NULL, NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyMatrixChannel_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                  /* ob_size */
    "PyPlasma.plMatrixChannel",         /* tp_name */
    sizeof(pyMatrixChannel),            /* tp_basicsize */
    0,                                  /* tp_itemsize */

    NULL,                               /* tp_dealloc */
    NULL,                               /* tp_print */
    NULL,                               /* tp_getattr */
    NULL,                               /* tp_setattr */
    NULL,                               /* tp_compare */
    NULL,                               /* tp_repr */
    NULL,                               /* tp_as_number */
    NULL,                               /* tp_as_sequence */
    NULL,                               /* tp_as_mapping */
    NULL,                               /* tp_hash */
    NULL,                               /* tp_call */
    NULL,                               /* tp_str */
    NULL,                               /* tp_getattro */
    NULL,                               /* tp_setattro */
    NULL,                               /* tp_as_buffer */

    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    "plMatrixChannel wrapper",          /* tp_doc */

    NULL,                               /* tp_traverse */
    NULL,                               /* tp_clear */
    NULL,                               /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    NULL,                               /* tp_iter */
    NULL,                               /* tp_iternext */

    pyMatrixChannel_Methods,            /* tp_methods */
    NULL,                               /* tp_members */
    pyMatrixChannel_GetSet,             /* tp_getset */
    NULL,                               /* tp_base */
    NULL,                               /* tp_dict */
    NULL,                               /* tp_descr_get */
    NULL,                               /* tp_descr_set */
    0,                                  /* tp_dictoffset */

    NULL,                               /* tp_init */
    NULL,                               /* tp_alloc */
    pyMatrixChannel_new,                /* tp_new */
    NULL,                               /* tp_free */
    NULL,                               /* tp_is_gc */

    NULL,                               /* tp_bases */
    NULL,                               /* tp_mro */
    NULL,                               /* tp_cache */
    NULL,                               /* tp_subclasses */
    NULL,                               /* tp_weaklist */
};

PyObject* Init_pyMatrixChannel_Type() {
    pyMatrixChannel_Type.tp_base = &pyAGChannel_Type;
    if (PyType_Ready(&pyMatrixChannel_Type) < 0)
        return NULL;

    Py_INCREF(&pyMatrixChannel_Type);
    return (PyObject*)&pyMatrixChannel_Type;
}

int pyMatrixChannel_Check(PyObject* obj) {
    if (obj->ob_type == &pyMatrixChannel_Type
        || PyType_IsSubtype(obj->ob_type, &pyMatrixChannel_Type))
        return 1;
    return 0;
}

PyObject* pyMatrixChannel_FromMatrixChannel(class plMatrixChannel* chan) {
    if (chan == NULL) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    pyMatrixChannel* pyobj = PyObject_New(pyMatrixChannel, &pyMatrixChannel_Type);
    pyobj->fThis = chan;
    pyobj->fPyOwned = false;
    return (PyObject*)pyobj;
}

}