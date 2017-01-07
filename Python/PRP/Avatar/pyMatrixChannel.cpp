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

#include "pyAGChannel.h"

#include <PRP/Avatar/plMatrixChannel.h>
#include "PRP/pyCreatable.h"
#include "Math/pyGeometry3.h"

extern "C" {

PY_PLASMA_NEW(MatrixChannel, plMatrixChannel)

PY_PROPERTY(hsAffineParts, MatrixChannel, affine, getAffine, setAffine)

static PyGetSetDef pyMatrixChannel_GetSet[] = {
    pyMatrixChannel_affine_getset,
    PY_GETSET_TERMINATOR
};

PY_PLASMA_TYPE(MatrixChannel, plMatrixChannel, "plMatrixChannel wrapper")

PY_PLASMA_TYPE_INIT(MatrixChannel) {
    pyMatrixChannel_Type.tp_new = pyMatrixChannel_new;
    pyMatrixChannel_Type.tp_getset = pyMatrixChannel_GetSet;
    pyMatrixChannel_Type.tp_base = &pyAGChannel_Type;
    if (PyType_Ready(&pyMatrixChannel_Type) < 0)
        return NULL;

    Py_INCREF(&pyMatrixChannel_Type);
    return (PyObject*)&pyMatrixChannel_Type;
}

PY_PLASMA_IFC_METHODS(MatrixChannel, plMatrixChannel)

}
