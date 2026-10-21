#pragma once

#include "src/core/common.h"

namespace stormpy {

void define_api_properties(py::module& m);
template<typename ValueType>
void define_api_builder_typed(py::module& m);
template<typename ValueType>
void define_api_bisimulation_typed(py::module& m);
template<typename ValueType>
void define_api_export_typed(py::module& m);
template<typename ValueType>
void define_api_transformation_typed(py::module& m);
template<typename ValueType>
void define_api_verification_typed(py::module& m);

}  // namespace stormpy
