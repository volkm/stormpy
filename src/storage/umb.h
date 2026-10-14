#pragma once

#include "src/common.h"

void define_umb(py::module& m);
template<typename ValueType>
void define_umb_typed(py::module& m);
