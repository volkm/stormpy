#pragma once

#include "src/dft/common.h"

namespace stormpy::dft {

void define_storage_elements(py::module& m);

template<typename ValueType>
void define_storage_elements_typed(py::module& m);

}  // namespace stormpy::dft
