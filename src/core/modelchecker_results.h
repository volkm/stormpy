#pragma once

#include "src/core/common.h"

namespace stormpy {

void define_modelchecker_results(py::module& m);
template<typename ValueType>
void define_modelchecker_results_typed(py::module& m);

}  // namespace stormpy
