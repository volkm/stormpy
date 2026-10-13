#pragma once

#include "src/core/common.h"

namespace stormpy {

void define_transformer(py::module& m);
template<typename ValueType>
void define_transformer_typed(py::module& m);
template<typename ValueType>
void define_transformer_add_uncertainty_typed(py::module& m);

}  // namespace stormpy
