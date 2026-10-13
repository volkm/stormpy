#pragma once

#include "src/dft/common.h"

namespace stormpy::dft {

void define_builder(py::module& m);

template<typename ValueType>
void define_builder_typed(py::module& m);

}  // namespace stormpy::dft
