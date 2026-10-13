#include "counterexamples.h"

#include <storm-counterexamples/api/counterexamples.h>
#include <storm/environment/Environment.h>

#include "src/binding_type_index.h"

namespace stormpy {

using namespace storm::counterexamples;

// Define python bindings
void define_counterexamples(py::module& m) {
    using CexGeneratorStats = SMTMinimalLabelSetGenerator<double>::GeneratorStats;

    stormpy::bindings::bindTemplateClass<CexGeneratorStats>(m, "SMTMinimalLabelSetGeneratorStats", stormpy::bindings::typeIndex<double>(),
                                                            "Stats for highlevel counterexample generation")
        .def(py::init<>())
        .def_readonly("analysis_time", &CexGeneratorStats::analysisTime)
        .def_readonly("setup_time", &CexGeneratorStats::setupTime)
        .def_readonly("model_checking_time", &CexGeneratorStats::modelCheckingTime)
        .def_readonly("solver_time", &CexGeneratorStats::solverTime)
        .def_readonly("cut_time", &CexGeneratorStats::cutTime)
        .def_readonly("iterations", &CexGeneratorStats::iterations);

    using CexGeneratorOptions = SMTMinimalLabelSetGenerator<double>::Options;
    stormpy::bindings::bindTemplateClass<CexGeneratorOptions>(m, "SMTMinimalLabelSetGeneratorOptions", stormpy::bindings::typeIndex<double>(),
                                                              "Options for highlevel counterexample generation")
        .def(py::init<bool>(), py::arg("check_threshold_feasible") = false)
        .def_readwrite("check_threshold_feasible", &CexGeneratorOptions::checkThresholdFeasible)
        .def_readwrite("encode_reachability", &CexGeneratorOptions::encodeReachability)
        .def_readwrite("silent", &CexGeneratorOptions::silent)
        .def_readwrite("add_backward_implication_cuts", &CexGeneratorOptions::addBackwardImplicationCuts)
        .def_readwrite("use_dynamic_constraints", &CexGeneratorOptions::useDynamicConstraints)
        .def_readwrite("maximum_counterexamples", &CexGeneratorOptions::maximumCounterexamples)
        .def_readwrite("continue_after_first_counterexample_until", &CexGeneratorOptions::continueAfterFirstCounterexampleUntil)
        .def_readwrite("maximum_extra_iterations", &CexGeneratorOptions::maximumExtraIterations);

    stormpy::bindings::bindTemplateClass<SMTMinimalLabelSetGenerator<double>>(m, "SMTMinimalLabelSetGenerator", stormpy::bindings::typeIndex<double>(),
                                                                              "Highlevel Counterexample Generator with SMT as backend")
        .def_static("precompute", &SMTMinimalLabelSetGenerator<double>::precompute, "Precompute input for counterexample generation", py::arg("env"),
                    py::arg("symbolic_model"), py::arg("model"), py::arg("formula"))
        .def_static("compute_counterexample_label_set", &SMTMinimalLabelSetGenerator<double>::computeCounterexampleLabelSet, "Compute counterexample",
                    py::arg("env"), py::arg("stats"), py::arg("symbolic_model"), py::arg("model"), py::arg("counterex_input"), py::arg("dont_care_labels"),
                    py::arg("options"))

        ;

    using CexInput = SMTMinimalLabelSetGenerator<double>::CexInput;
    stormpy::bindings::bindTemplateClass<CexInput>(m, "SMTMinimalLabelSetGeneratorCexInput", stormpy::bindings::typeIndex<double>(),
                                                   "Precomputed input for counterexample generation")
        .def("add_reward_threshold_combination", &CexInput::addRewardThresholdCombination, "Add another reward structure and threshold", py::arg("reward"),
             py::arg("thresh"));
}

}  // namespace stormpy
