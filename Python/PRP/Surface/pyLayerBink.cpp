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

#include "pyLayerMovie.h"

#include <PRP/Surface/plLayerMovie.h>
#include "PRP/pyCreatable.h"

extern "C" {

PY_PLASMA_NEW(LayerBink, plLayerBink)

PY_PLASMA_TYPE(LayerBink, plLayerBink, "plLayerBink wrapper")

PY_PLASMA_TYPE_INIT(LayerBink) {
    pyLayerBink_Type.tp_new = pyLayerBink_new;
    pyLayerBink_Type.tp_base = &pyLayerMovie_Type;
    if (PyType_Ready(&pyLayerBink_Type) < 0)
        return NULL;

    Py_INCREF(&pyLayerBink_Type);
    return (PyObject*)&pyLayerBink_Type;
}

PY_PLASMA_IFC_METHODS(LayerBink, plLayerBink)

}
