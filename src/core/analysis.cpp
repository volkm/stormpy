#include "analysis.h"

#include <storm/analysis/GraphConditions.h>

namespace stormpy {

void define_analysis(py::module& m) {
    py::classh<storm::analysis::ConstraintCollector>(m, "ConstraintCollector", "Collector for constraints on parametric Markov chains")
        .def(py::init<storm::models::sparse::Model<storm::RationalFunction> const&>(), "Collect constraints of the model", py::arg("model"))
        .def("get_wellformed_constraints", &storm::analysis::ConstraintCollector::getWellformedConstraints, "Get the constraints ensuring a well-formed model")
        .def("get_graph_preserving_constraints", &storm::analysis::ConstraintCollector::getGraphPreservingConstraints,
             "Get the constraints ensuring the graph is preserved");
}

}  // namespace stormpy
