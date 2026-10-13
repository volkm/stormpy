#include "utility.h"

#include <storm/adapters/RationalFunctionAdapter.h>
#include <storm/models/sparse/DeterministicModel.h>
#include <storm/models/sparse/NondeterministicModel.h>
#include <storm/models/sparse/StandardRewardModel.h>
#include <storm/settings/SettingsManager.h>
#include <storm/settings/modules/CounterexampleGeneratorSettings.h>
#include <storm/utility/SignalHandler.h>
#include <storm/utility/graph.h>
#include <storm/utility/initialize.h>

namespace stormpy {

void define_utility(py::module& m) {
    m.def(
        // Thin wrapper for initializing Storm (logging) and its settings.
        // TODO: remove when settings are completely eliminated
        "_set_up",
        [](std::string const& commandLineString) {
            storm::utility::setUp();
            storm::settings::initializeAll("stormpy", "stormpy");
            storm::settings::addModule<storm::settings::modules::CounterexampleGeneratorSettings>();
            storm::settings::mutableManager().setFromString(commandLineString);
        },
        "Initialize Storm", py::arg("command_line_string"));

    py::native_enum<l3pp::LogLevel>(m, "LogLevel", "enum.Enum", "Log level of Storm")
        .value("TRACE", l3pp::LogLevel::TRACE)
        .value("DEBUG", l3pp::LogLevel::DEBUG)
        .value("INFO", l3pp::LogLevel::INFO)
        .value("WARN", l3pp::LogLevel::WARN)
        .value("ERR", l3pp::LogLevel::ERR)
        .value("FATAL", l3pp::LogLevel::FATAL)
        .value("OFF", l3pp::LogLevel::OFF)
        .finalize();
    m.def("set_log_level", &storm::utility::setLogLevel, "Set log level of Storm", py::arg("level"));
    m.def("get_log_level", &storm::utility::getLogLevel, "Get log level of Storm");

    m.def("set_timeout_alarm", &storm::utility::resources::setTimeoutAlarm, "Set timeout in seconds", py::arg("timeout"));
    m.def("reset_timeout_alarm", &storm::utility::resources::resetTimeoutAlarm, "Reset timeout");
    m.def("install_signal_handler", &storm::utility::resources::installSignalHandler, "Install signal handler for timeouts and interrupts",
          py::arg("maximal_wait_time"));
}

template<typename ValueType>
void define_utility_graph_typed(py::module& m) {
    m.def("get_reachable_states", &storm::utility::graph::getReachableStates<ValueType>, "Get states reachable from the initial states",
          py::arg("transition_matrix"), py::arg("initial_states"), py::arg("constraint_states"), py::arg("target_states"), py::arg("use_step_bound") = false,
          py::arg("maximal_steps") = 0, py::arg("choice_filter") = boost::none);
    m.def("perform_prob01",
          py::overload_cast<storm::models::sparse::DeterministicModel<ValueType> const&, storm::storage::BitVector const&, storm::storage::BitVector const&>(
              &storm::utility::graph::performProb01<ValueType>),
          "Compute states with probability 0 and 1 of satisfying phi until psi", py::arg("model"), py::arg("phi_states"), py::arg("psi_states"));
    m.def("perform_prob01_min",
          py::overload_cast<storm::models::sparse::NondeterministicModel<ValueType> const&, storm::storage::BitVector const&, storm::storage::BitVector const&>(
              &storm::utility::graph::performProb01Min<ValueType, storm::models::sparse::StandardRewardModel<ValueType>>),
          "Compute states with minimal probability 0 and 1 of satisfying phi until psi", py::arg("model"), py::arg("phi_states"), py::arg("psi_states"));
    m.def("perform_prob01_max",
          py::overload_cast<storm::models::sparse::NondeterministicModel<ValueType> const&, storm::storage::BitVector const&, storm::storage::BitVector const&>(
              &storm::utility::graph::performProb01Max<ValueType, storm::models::sparse::StandardRewardModel<ValueType>>),
          "Compute states with maximal probability 0 and 1 of satisfying phi until psi", py::arg("model"), py::arg("phi_states"), py::arg("psi_states"));
}

template void define_utility_graph_typed<double>(py::module&);
template void define_utility_graph_typed<storm::RationalNumber>(py::module&);
template void define_utility_graph_typed<storm::RationalFunction>(py::module&);

}  // namespace stormpy
