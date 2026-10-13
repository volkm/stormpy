#pragma once

#include "src/dft/common.h"

namespace stormpy::dft {

void define_simulator(py::module& m);

template<typename ValueType>
void define_simulator_typed(py::module& m);

}  // namespace stormpy::dft
