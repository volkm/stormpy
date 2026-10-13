#include "generator.h"

#include <pybind11/functional.h>
#include <storm/generator/Choice.h>
#include <storm/generator/NextStateGenerator.h>

#include "src/binding_type_index.h"

namespace stormpy {

void define_generator(py::module& m) {
    using Choice = storm::generator::Choice<double, uint32_t>;
    stormpy::bindings::bindTemplateClass<Choice>(m, "Choice", stormpy::bindings::typeIndex<double>(), "Choice of a state during state space exploration")
        .def("get_action_index", &Choice::getActionIndex, "Get the index of the action of this choice")
        .def("has_labels", &Choice::hasLabels, "Whether this choice is labeled")
        .def(
            // Thin wrapper as Storm does not check whether labels are present
            "get_labels", [](Choice const& choice) { return choice.hasLabels() ? choice.getLabels() : std::set<std::string>(); },
            "Get the labels of this choice (empty if the choice is not labeled)")
        .def("is_markovian", &Choice::isMarkovian, "Whether this choice is Markovian")
        .def("get_total_mass", &Choice::getTotalMass, "Get the total probability mass of this choice")
        .def("get_rewards", py::overload_cast<>(&Choice::getRewards, py::const_), "Get the rewards of this choice")
        .def("size", &Choice::size, "Get the number of successor states")
        .def("__len__", &Choice::size);

    auto actionMask = stormpy::bindings::bindTemplateClass<storm::generator::ActionMask<double>>(m, "ActionMask", stormpy::bindings::typeIndex<double>(),
                                                                                                 "Mask for actions during state space exploration");
    stormpy::bindings::bindTemplateClass<storm::generator::StateValuationFunctionMask<double>>(
        m, "StateValuationFunctionMask", stormpy::bindings::typeIndex<double>(), "Action mask based on state valuations", actionMask)
        .def(py::init<std::function<bool(storm::expressions::SimpleValuation const&, uint64_t)>>(), py::arg("f"));
}

}  // namespace stormpy
