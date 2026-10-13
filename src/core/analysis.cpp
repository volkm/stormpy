#include "analysis.h"

#include <storm/analysis/GraphConditions.h>

#include "src/binding_type_index.h"

namespace stormpy {

void define_analysis(py::module& m) {
    using ConstraintCollector = storm::analysis::ConstraintCollector<storm::RationalFunction>;
    stormpy::bindings::bindTemplateClass<ConstraintCollector>(m, "ConstraintCollector", stormpy::bindings::typeIndex<storm::RationalFunction>(),
                                                              "Collector for constraints on parametric Markov chains")
        .def(py::init<storm::models::sparse::Model<storm::RationalFunction> const&>(), "Collect constraints of the model", py::arg("model"))
        .def("get_wellformed_constraints", &ConstraintCollector::getWellformedConstraints, "Get the constraints ensuring a well-formed model")
        .def("get_graph_preserving_constraints", &ConstraintCollector::getGraphPreservingConstraints, "Get the constraints ensuring the graph is preserved");
}

}  // namespace stormpy
