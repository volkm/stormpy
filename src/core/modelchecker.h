#pragma once

#include "src/core/common.h"

namespace stormpy {

void define_modelchecker(py::module& m);
template<typename ValueType>
void define_modelchecker_typed(py::module& m);

}  // namespace stormpy
