#pragma once

#include "src/core/common.h"

namespace stormpy {

template<typename ValueType>
void define_simulator_sparse_typed(py::module& m);
template<typename ValueType>
void define_simulator_prism_typed(py::module& m);

}  // namespace stormpy
