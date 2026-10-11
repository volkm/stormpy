#include "src/common.h"
#include "src/helpers.h"
#include "src/pycarl/typed_formula/constraint.h"
#include "src/pycarl/typed_formula/formula.h"

PYBIND11_MODULE(_formula, m) {
    m.attr("__name__") = "stormpy.numbers.developer.formula";
    m.doc() = "pycarl formula typed functions";

    // Constraint relies on Rational
    m.import("stormpy.numbers.developer");
    m.import("stormpy.numbers.developer.formula");

    define_constraint(m);
    define_simple_constraint(m);
    define_formula(m);
}
