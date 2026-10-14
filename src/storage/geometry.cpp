#include "geometry.h"

#include <storm/storage/geometry/Polytope.h>

#include "src/binding_type_index.h"
#include "src/helpers.h"

template<typename ValueType>
void define_geometry(py::module& m) {
    typedef storm::storage::geometry::Polytope<ValueType> Polytope;
    auto polytope = stormpy::bindings::bindTemplateClass<Polytope>(m, "Polytope", stormpy::bindings::typeIndex<ValueType>(), "Polytope");
    polytope.def_property_readonly("vertices", &Polytope::getVertices);
    polytope.def("downward_closure", &Polytope::downwardClosure);
    polytope.def("get_vertices_in_clockwise_order", &Polytope::getVerticesInClockwiseOrder);
}

template void define_geometry<double>(py::module&);
template void define_geometry<storm::RationalNumber>(py::module&);
