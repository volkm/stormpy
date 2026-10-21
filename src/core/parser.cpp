#include "parser.h"

#include <storm-parsers/api/storm-parsers.h>
#include <storm-parsers/parser/DirectEncodingParser.h>
#include <storm-parsers/parser/ExplicitModelParserOptions.h>

#include "src/binding_type_index.h"
#include "src/numbers.h"

namespace stormpy {

void define_parser(py::module& m) {
    py::classh<storm::parser::DirectEncodingParserOptions>(m, "DirectEncodingParserOptions", "Options for the DRN parser")
        .def(py::init<>(), "Create default options")
        .def_readwrite("build_choice_labeling", &storm::parser::DirectEncodingParserOptions::buildChoiceLabeling, "Build choice labeling");

    py::classh<storm::parser::ExplicitModelParserOptions>(m, "ExplicitModelParserOptions", "Options for the explicit model parser")
        .def(py::init<>(), "Create default options")
        .def_readwrite("fix_deadlocks", &storm::parser::ExplicitModelParserOptions::fixDeadlocks,
                       "If set, deadlocks states will be fixed by adding a self-loop with probability 1.")
        .def_readwrite("build_choice_labels", &storm::parser::ExplicitModelParserOptions::buildChoiceLabels, "Whether to build choice labels.");
}

void define_parsers_api_model_descriptions(py::module& m) {
    m.def("parse_program", &storm::api::parseProgram, "Parse Prism program", py::arg("filename"), py::arg("prism_compatibility") = false,
          py::arg("simplify") = true);
    m.def("parse_jani_model", py::overload_cast<std::string const&, boost::optional<std::vector<std::string>> const&>(&storm::api::parseJaniModel),
          "Parse Jani model and its properties", py::arg("filename"), py::arg("property_filter") = boost::none);
    m.def("parse_jani_model_from_string",
          py::overload_cast<std::string const&, boost::optional<std::vector<std::string>> const&>(&storm::api::parseJaniModelFromString),
          "Parse Jani model and its properties from string", py::arg("jsonstring"), py::arg("property_filter") = boost::none);
}

void define_parsers_api_properties(py::module& m) {
    m.def("parse_properties", py::overload_cast<std::string const&, boost::optional<std::set<std::string>> const&>(&storm::api::parseProperties),
          "Parse properties (without model context)", py::arg("input_string"), py::arg("property_filter") = boost::none);
    m.def("parse_properties_for_prism_program", &storm::api::parsePropertiesForPrismProgram,
          "Parse properties, allowing references to variables of the Prism program", py::arg("input_string"), py::arg("program"),
          py::arg("property_filter") = boost::none);
    m.def("parse_properties_for_jani_model", &storm::api::parsePropertiesForJaniModel, "Parse properties, allowing references to variables of the Jani model",
          py::arg("input_string"), py::arg("model"), py::arg("property_filter") = boost::none);
    m.def("parse_properties_for_symbolic_model_description", &storm::api::parsePropertiesForSymbolicModelDescription,
          "Parse properties, allowing references to variables of the symbolic model description", py::arg("input_string"), py::arg("model_description"),
          py::arg("property_filter") = boost::none);
}

template<typename ValueType>
void define_parsers_api_explicit_models_typed(py::module& m) {
    // Value type cannot be deduced from the arguments -> bind as template function family
    stormpy::bindings::bindTemplateFunction(m, "build_explicit_drn_model", stormpy::bindings::typeIndex<ValueType>(),
                                            &storm::api::buildExplicitDRNModel<ValueType>, "Build sparse model from DRN file", py::arg("drn_file"),
                                            py::arg("options") = storm::parser::DirectEncodingParserOptions());

    if constexpr (std::is_same_v<ValueType, double>) {
        m.def("build_explicit_model", &storm::api::buildExplicitModel<double>, "Build sparse model from explicit input files", py::arg("transitions_file"),
              py::arg("labeling_file"), py::arg("state_rewards_file"), py::arg("transition_rewards_file"), py::arg("choice_labeling_file"),
              py::arg("options") = storm::parser::ExplicitModelParserOptions());
    }
}

template void define_parsers_api_explicit_models_typed<double>(py::module&);
template void define_parsers_api_explicit_models_typed<storm::RationalNumber>(py::module&);
template void define_parsers_api_explicit_models_typed<storm::RationalFunction>(py::module&);
template void define_parsers_api_explicit_models_typed<storm::Interval>(py::module&);
template void define_parsers_api_explicit_models_typed<storm::RationalInterval>(py::module&);

}  // namespace stormpy
