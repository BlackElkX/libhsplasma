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

#include "pyDynamicEnvMap.h"

#include <PRP/Surface/plDynamicEnvMap.h>
#include "pyRenderTarget.h"
#include "PRP/pyCreatable.h"
#include "PRP/Surface/pyBitmap.h"
#include "PRP/KeyedObject/pyKey.h"
#include "PRP/Object/pySynchedObject.h"
#include "Math/pyGeometry3.h"
#include "Sys/pyColor.h"

extern "C" {

// DynamicEnvMap

PY_PLASMA_NEW(DynamicEnvMap, plDynamicEnvMap)

static PyObject* pyDynamicEnvMap_getVisRegions(pyDynamicEnvMap* self, void*) {
    const std::vector<plKey>& keys = self->fThis->getVisRegions();
    PyObject* regionList = PyList_New(keys.size());
    for (size_t i=0; i<keys.size(); i++)
        PyList_SET_ITEM(regionList, i, pyKey_FromKey(keys[i]));
    return regionList;
}

static PyObject* pyDynamicEnvMap_getVisRegionNames(pyDynamicEnvMap* self, void*) {
    const std::vector<plString>& names = self->fThis->getVisRegionNames();
    PyObject* regionNameList = PyList_New(names.size());
    for (size_t i=0; i<names.size(); i++)
        PyList_SET_ITEM(regionNameList, i, PlasmaString_To_PyString(names[i]));
    return regionNameList;
}

static int pyDynamicEnvMap_setVisRegions(pyDynamicEnvMap* self, PyObject* value, void*) {
    if (value == NULL || !PySequence_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "visRegions should be a sequence of plKeys");
        return -1;
    }
    std::vector<plKey> regions;
    regions.resize(PySequence_Size(value));
    for (Py_ssize_t i=0; i<PySequence_Size(value); i++) {
        PyObject* region = PySequence_GetItem(value, i);
        if (pyKey_Check(region)){
            regions[i] = *(reinterpret_cast<pyKey *>(region)->fThis);
        } else {
            PyErr_SetString(PyExc_TypeError, "visRegions should be a sequence of plKeys");
            return -1;
        }
    }
    self->fThis->setVisRegions(regions);
    return 0;
}

static int pyDynamicEnvMap_setVisRegionNames(pyDynamicEnvMap* self, PyObject* value, void*) {
    if (value == NULL || !PySequence_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "visRegionNames should be a sequence of strings");
        return -1;
    }
    std::vector<plString> names;
    names.resize(PySequence_Size(value));
    for (Py_ssize_t i=0; i<PySequence_Size(value); i++) {
        PyObject* name = PySequence_GetItem(value, i);
        if (PyAnyStr_Check(name)) {
            names[i] = PyString_To_PlasmaString(name);
        } else {
            PyErr_SetString(PyExc_TypeError, "visRegionNames should be a sequence of strings");
            return -1;
        }
    }
    self->fThis->setVisRegionNames(names);
    return 0;
}

PY_PROPERTY(plKey, DynamicEnvMap, rootNode, getRootNode, setRootNode)
PY_PROPERTY(hsVector3, DynamicEnvMap, position, getPosition, setPosition)
PY_PROPERTY(float, DynamicEnvMap, hither, getHither, setHither)
PY_PROPERTY(float, DynamicEnvMap, yon, getYon, setYon)
PY_PROPERTY(float, DynamicEnvMap, fogStart, getFogStart, setFogStart)
PY_PROPERTY(hsColorRGBA, DynamicEnvMap, color, getColor, setColor)
PY_PROPERTY(float, DynamicEnvMap, refreshRate, getRefreshRate, setRefreshRate)
PY_PROPERTY(bool, DynamicEnvMap, incCharacters, getIncludeCharacters,
            setIncludeCharacters)

static PyGetSetDef pyDynamicEnvMap_GetSet[] = {
    pyDynamicEnvMap_rootNode_getset,
    pyDynamicEnvMap_position_getset,
    pyDynamicEnvMap_hither_getset,
    pyDynamicEnvMap_yon_getset,
    pyDynamicEnvMap_fogStart_getset,
    pyDynamicEnvMap_color_getset,
    pyDynamicEnvMap_refreshRate_getset,
    { _pycs("visRegions"), (getter)pyDynamicEnvMap_getVisRegions, (setter)pyDynamicEnvMap_setVisRegions, NULL, NULL },
    { _pycs("visRegionNames"), (getter)pyDynamicEnvMap_getVisRegionNames, (setter)pyDynamicEnvMap_setVisRegionNames, NULL, NULL },
    pyDynamicEnvMap_incCharacters_getset,
    PY_GETSET_TERMINATOR
};

PY_PLASMA_TYPE(DynamicEnvMap, plDynamicEnvMap, "plDynamicEnvMap wrapper")

PY_PLASMA_TYPE_INIT(DynamicEnvMap) {
    pyDynamicEnvMap_Type.tp_new = pyDynamicEnvMap_new;
    pyDynamicEnvMap_Type.tp_getset = pyDynamicEnvMap_GetSet;
    pyDynamicEnvMap_Type.tp_base = &pyCubicRenderTarget_Type;
    if (PyType_Ready(&pyDynamicEnvMap_Type) < 0)
        return NULL;

    Py_INCREF(&pyDynamicEnvMap_Type);
    return (PyObject*)&pyDynamicEnvMap_Type;
}

PY_PLASMA_IFC_METHODS(DynamicEnvMap, plDynamicEnvMap)

}
