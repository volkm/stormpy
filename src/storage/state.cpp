#include "state.h"

#include <storm/adapters/IntervalAdapter.h>

#include "src/binding_type_index.h"

template<typename ValueType>
void define_state(py::module& m) {
    auto const index = stormpy::bindings::typeIndex<ValueType>();
    // SparseModelStates
    stormpy::bindings::bindTemplateClass<SparseModelStates<ValueType>>(m, "SparseModelStates", index, "States in sparse model")
        .def("__getitem__", &SparseModelStates<ValueType>::getState)
        .def("__len__", &SparseModelStates<ValueType>::getSize);

    // SparseModelState
    stormpy::bindings::bindTemplateClass<SparseModelState<ValueType>>(m, "SparseModelState", index, "State in sparse model")
        .def("__str__", &SparseModelState<ValueType>::toString)
        .def_property_readonly("id", &SparseModelState<ValueType>::getIndex, "Id")
        .def_property_readonly("labels", &SparseModelState<ValueType>::getLabels, "Get state labels")
        .def_property_readonly("valuations", &SparseModelState<ValueType>::getValuations, "Get state valuations")
        .def_property_readonly("actions", &SparseModelState<ValueType>::getActions, "Get actions")
        .def("__int__", &SparseModelState<ValueType>::getIndex);

    // SparseModelActions
    stormpy::bindings::bindTemplateClass<SparseModelActions<ValueType>>(m, "SparseModelActions", index, "Actions for state in sparse model")
        .def("__getitem__", &SparseModelActions<ValueType>::getAction)
        .def("__len__", &SparseModelActions<ValueType>::getSize);

    // SparseModelAction
    stormpy::bindings::bindTemplateClass<SparseModelAction<ValueType>>(m, "SparseModelAction", index, "Action for state in sparse model")
        .def("__str__", &SparseModelAction<ValueType>::toString)
        .def_property_readonly("id", &SparseModelAction<ValueType>::getIndex, "Id")
        .def_property_readonly("transitions", &SparseModelAction<ValueType>::getTransitions, "Get transitions")
        .def_property_readonly("labels", &SparseModelAction<ValueType>::getLabels, "Get choice labels")
        .def_property_readonly("origins", &SparseModelAction<ValueType>::getOrigins, "Get choice origins");
}

template void define_state<double>(py::module& m);
template void define_state<storm::RationalNumber>(py::module& m);
template void define_state<storm::Interval>(py::module& m);
template void define_state<storm::RationalInterval>(py::module& m);
template void define_state<storm::RationalFunction>(py::module& m);
