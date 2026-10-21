#include "distribution.h"

#include <storm/adapters/IntervalAdapter.h>
#include <storm/adapters/RationalNumberAdapter.h>
#include <storm/storage/Distribution.h>

#include "src/binding_type_index.h"
#include "src/helpers.h"

template<typename ValueType>
void define_distribution(py::module& m) {
    using Distrib = storm::storage::Distribution<ValueType, uint_fast64_t>;

    auto distribution =
        stormpy::bindings::bindTemplateClass<Distrib>(m, "Distribution", stormpy::bindings::typeIndex<ValueType>(), "Finite Support Distribution");
    distribution.def("__str__", &streamToString<Distrib>);
}

template void define_distribution<double>(py::module&);
template void define_distribution<storm::RationalNumber>(py::module&);
template void define_distribution<storm::Interval>(py::module&);
template void define_distribution<storm::RationalInterval>(py::module&);
