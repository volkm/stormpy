#pragma once

#include "src/core/common.h"

namespace stormpy {

void define_builder(py::module& m);
template<typename ValueType>
void define_builder_typed(py::module& m);

}  // namespace stormpy
