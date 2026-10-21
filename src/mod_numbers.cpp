#include "src/common.h"
#include "src/numbers/valuetype.h"

PYBIND11_MODULE(_numbers, m) {
    m.doc() = "Number types of Storm";

#ifdef STORMPY_DISABLE_SIGNATURE_DOC
    py::options options;
    options.disable_function_signatures();
#endif

    using namespace stormpy::numbers;

    define_value_types(m);
}
