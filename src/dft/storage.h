#pragma once

#include "src/dft/common.h"

namespace stormpy::dft {

void define_symmetries(py::module& m);

void define_dft(py::module& m);

template<typename ValueType>
void define_dft_typed(py::module& m);

template<typename ValueType>
void define_dft_state(py::module& m);

void define_failable_elements(py::module& m);

void define_module(py::module& m);

}  // namespace stormpy::dft
