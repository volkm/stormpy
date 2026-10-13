#pragma once

#include "src/dft/common.h"

namespace stormpy::dft {

template<typename ValueType>
void define_api_io_input_typed(py::module& m);
void define_api_io_output(py::module& m);

void define_api_analysis(py::module& m);

template<typename ValueType>
void define_api_analysis_typed(py::module& m);

template<typename ValueType>
void define_api_transformation_typed(py::module& m);

}  // namespace stormpy::dft
