#pragma once

#include "src/core/common.h"

namespace stormpy {

void define_utility(py::module& m);
template<typename ValueType>
void define_utility_graph_typed(py::module& m);

}  // namespace stormpy
