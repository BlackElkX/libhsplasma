/* This file is part of HSPlasma.
 *
 * HSPlasma is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HSPlasma is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HSPlasma.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pySpanTemplate.h"

#include <PRP/Geometry/plSpanTemplate.h>
#include "Stream/pyStream.h"

extern "C" {

PY_PLASMA_NEW_MSG(SpanTemplate, "Cannot create plSpanTemplate objects from Python")

PY_METHOD_VA(SpanTemplate, read,
    "Params: stream\n"
    "Reads this object from the stream")
{
    pyStream* stream;
    if (!PyArg_ParseTuple(args, "O", &stream)) {
        PyErr_SetString(PyExc_TypeError, "read expects a hsStream");
        return NULL;
    }
    if (!pyStream_Check((PyObject*)stream)) {
        PyErr_SetString(PyExc_TypeError, "read expects a hsStream");
        return NULL;
    }
    self->fThis->read(stream->fThis);
    Py_RETURN_NONE;
}

PY_METHOD_VA(SpanTemplate, write,
    "Params: stream\n"
    "Writes this object to the stream")
{
    pyStream* stream;
    if (!PyArg_ParseTuple(args, "O", &stream)) {
        PyErr_SetString(PyExc_TypeError, "write expects a hsStream");
        return NULL;
    }
    if (!pyStream_Check((PyObject*)stream)) {
        PyErr_SetString(PyExc_TypeError, "write expects a hsStream");
        return NULL;
    }
    self->fThis->write(stream->fThis);
    Py_RETURN_NONE;
}

static PyObject* pySpanTemplate_getVerts(pySpanTemplate* self, void*) {
    std::vector<plSpanTemplate::Vertex> verts = self->fThis->getVertices();
    PyObject* list = PyList_New(verts.size());
    for (size_t i=0; i<verts.size(); i++)
        PyList_SET_ITEM(list, i, pySpanTemplateVertex_FromSpanTemplateVertex(&verts[i]));
    return list;
}

static PyObject* pySpanTemplate_getIndices(pySpanTemplate* self, void*) {
    const unsigned short* indices = self->fThis->getIndices();
    size_t numIndices = self->fThis->getNumTris() * 3;
    PyObject* list = PyList_New(numIndices);
    for (size_t i=0; i<numIndices; i++)
        PyList_SET_ITEM(list, i, pyPlasma_convert(indices[i]));
    return list;
}

static int pySpanTemplate_setVerts(pySpanTemplate* self, PyObject* value, void*) {
    std::vector<plSpanTemplate::Vertex> verts;
    if (value == NULL) {
        self->fThis->setVertices(verts);
        return 0;
    }
    if (!PyList_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "vertices should be a list of plSpanTemplateVertex objects");
        return -1;
    }
    verts.resize(PyList_Size(value));
    for (size_t i=0; i<verts.size(); i++) {
        PyObject* itm = PyList_GetItem(value, i);
        if (!pySpanTemplateVertex_Check(itm)) {
            PyErr_SetString(PyExc_TypeError, "vertices should be a list of plSpanTemplateVertex objects");
            return -1;
        }
        verts[i] = *((pySpanTemplateVertex*)itm)->fThis;
    }
    self->fThis->setVertices(verts);
    return 0;
}

static int pySpanTemplate_setIndices(pySpanTemplate* self, PyObject* value, void*) {
    std::vector<unsigned short> indices;
    if (value == NULL) {
        self->fThis->setIndices(0, NULL);
        return 0;
    }
    if (!PyList_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "indices should be a list of ints");
        return -1;
    }
    size_t numIndices = PyList_Size(value);
    indices.resize(numIndices);
    for (size_t i=0; i<numIndices; i++) {
        PyObject* itm = PyList_GetItem(value, i);
        if (!PyInt_Check(itm)) {
            PyErr_SetString(PyExc_TypeError, "indices should be a list of ints");
            return -1;
        }
        indices[i] = PyInt_AsLong(itm);
    }
    self->fThis->setIndices(numIndices, indices.data());
    return 0;
}

static PyMethodDef pySpanTemplate_Methods[] = {
    pySpanTemplate_read_method,
    pySpanTemplate_write_method,
    PY_METHOD_TERMINATOR
};

PY_PROPERTY(unsigned short, SpanTemplate, format, getFormat, setFormat)

static PyGetSetDef pySpanTemplate_GetSet[] = {
    { _pycs("vertices"), (getter)pySpanTemplate_getVerts,
        (setter)pySpanTemplate_setVerts, NULL, NULL },
    { _pycs("indices"), (getter)pySpanTemplate_getIndices,
        (setter)pySpanTemplate_setIndices, NULL, NULL },
    pySpanTemplate_format_getset,
    PY_GETSET_TERMINATOR
};

PY_PLASMA_TYPE(SpanTemplate, plSpanTemplate, "plSpanTemplate wrapper")

PY_PLASMA_TYPE_INIT(SpanTemplate) {
    pySpanTemplate_Type.tp_new = pySpanTemplate_new;
    pySpanTemplate_Type.tp_methods = pySpanTemplate_Methods;
    pySpanTemplate_Type.tp_getset = pySpanTemplate_GetSet;
    if (PyType_Ready(&pySpanTemplate_Type) < 0)
        return NULL;

    Py_INCREF(&pySpanTemplate_Type);
    return (PyObject*)&pySpanTemplate_Type;
}

PY_PLASMA_IFC_METHODS(SpanTemplate, plSpanTemplate)

}
