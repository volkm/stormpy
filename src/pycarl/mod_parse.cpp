#include <carl-parser/Parser.h>
#include <carl/formula/Formula.h>

#include "src/common.h"

/**
 * The actual module definition
 */
PYBIND11_MODULE(_parse, m) {
    m.attr("__name__") = "stormpy.pycarl.parse";
    m.doc() = "pycarl parsing functions";

    // Constraint relies on Rational
    m.import("stormpy.pycarl");

    py::native_enum<carlparser::ParserReturnType>(m, "_ParserReturnType", "enum.Enum")
        .value("RATIONAL", carlparser::ParserReturnType::Rational)
        .value("VARIABLE", carlparser::ParserReturnType::Variable)
        .value("MONOMIAL", carlparser::ParserReturnType::Monomial)
        .value("TERM", carlparser::ParserReturnType::Term)
        .value("POLYNOMIAL", carlparser::ParserReturnType::Polynomial)
        .value("RATIONAL_FUNCTION", carlparser::ParserReturnType::RationalFunction)
        .value("CONSTRAINT", carlparser::ParserReturnType::Constraint)
        .value("FORMULA", carlparser::ParserReturnType::Formula)
        .finalize();
}
