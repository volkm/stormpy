#pragma once

#include "src/core/common.h"

namespace stormpy {

void define_parser(py::module& m);

void define_parsers_api_model_descriptions(py::module& m);
void define_parsers_api_properties(py::module& m);
template<typename ValueType>
void define_parsers_api_explicit_models_typed(py::module& m);

}  // namespace stormpy
