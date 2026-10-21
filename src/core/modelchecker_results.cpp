#include "modelchecker_results.h"

#include <storm/adapters/RationalFunctionAdapter.h>
#include <storm/modelchecker/results/ExplicitParetoCurveCheckResult.h>
#include <storm/modelchecker/results/ExplicitQualitativeCheckResult.h>
#include <storm/modelchecker/results/ExplicitQuantitativeCheckResult.h>
#include <storm/modelchecker/results/FilterType.h>
#include <storm/modelchecker/results/HybridQuantitativeCheckResult.h>
#include <storm/modelchecker/results/SymbolicQualitativeCheckResult.h>
#include <storm/modelchecker/results/SymbolicQuantitativeCheckResult.h>
#include <storm/models/symbolic/StandardRewardModel.h>
#include <storm/solver/SolutionBounds.h>

#include "src/binding_type_index.h"
#include "src/numbers.h"

namespace stormpy {

using CheckResult = storm::modelchecker::CheckResult;
using QualitativeCheckResult = storm::modelchecker::QualitativeCheckResult;
constexpr auto Sylvan = storm::dd::DdType::Sylvan;

// Storm only asserts (in debug builds) that a state has a value. Raise an IndexError instead of reading out of bounds.
template<typename Result>
void checkHasValueForState(Result const& result, storm::storage::sparse::state_type state) {
    if (!result.hasValueForState(state)) {
        throw py::index_error("State " + std::to_string(state) + " has no value in this result");
    }
}

void define_modelchecker_results(py::module& m) {
    py::native_enum<storm::modelchecker::FilterType>(m, "FilterType", "enum.Enum", "Types of filters applied to model checking results")
        .value("MIN", storm::modelchecker::FilterType::MIN)
        .value("MAX", storm::modelchecker::FilterType::MAX)
        .value("SUM", storm::modelchecker::FilterType::SUM)
        .value("AVG", storm::modelchecker::FilterType::AVG)
        .value("COUNT", storm::modelchecker::FilterType::COUNT)
        .value("FORALL", storm::modelchecker::FilterType::FORALL)
        .value("EXISTS", storm::modelchecker::FilterType::EXISTS)
        .value("ARGMIN", storm::modelchecker::FilterType::ARGMIN)
        .value("ARGMAX", storm::modelchecker::FilterType::ARGMAX)
        .value("VALUES", storm::modelchecker::FilterType::VALUES)
        .finalize();

    py::classh<CheckResult> checkResult(m, "CheckResult", "Base class for all model checking results");
    checkResult.def("is_explicit", &CheckResult::isExplicit, "Whether the result is explicit")
        .def("is_symbolic", &CheckResult::isSymbolic, "Whether the result is symbolic")
        .def("is_hybrid", &CheckResult::isHybrid, "Whether the result is hybrid")
        .def("is_quantitative", &CheckResult::isQuantitative, "Whether the result is quantitative")
        .def("is_qualitative", &CheckResult::isQualitative, "Whether the result is qualitative")
        .def("is_pareto_curve_check_result", &CheckResult::isParetoCurveCheckResult, "Whether the result is a Pareto curve")
        .def("is_explicit_qualitative_check_result", &CheckResult::isExplicitQualitativeCheckResult, "Whether the result is explicit qualitative")
        .def("is_explicit_quantitative_check_result", &CheckResult::isExplicitQuantitativeCheckResult, "Whether the result is explicit quantitative")
        .def("is_explicit_pareto_curve_check_result", &CheckResult::isExplicitParetoCurveCheckResult, "Whether the result is an explicit Pareto curve")
        .def("is_symbolic_qualitative_check_result", &CheckResult::isSymbolicQualitativeCheckResult, "Whether the result is symbolic qualitative")
        .def("is_symbolic_quantitative_check_result", &CheckResult::isSymbolicQuantitativeCheckResult, "Whether the result is symbolic quantitative")
        .def("is_hybrid_quantitative_check_result", &CheckResult::isHybridQuantitativeCheckResult, "Whether the result is hybrid quantitative")
        .def("is_result_for_all_states", &CheckResult::isResultForAllStates, "Whether the result contains values for all states")
        .def("has_scheduler", &CheckResult::hasScheduler, "Whether a scheduler is present")
        .def("filter", &CheckResult::filter, "Restrict the result to the states of the given filter", py::arg("filter"))
        .def("__str__", [](CheckResult const& result) {
            std::stringstream stream;
            result.writeToStream(stream);
            return stream.str();
        });

    py::classh<QualitativeCheckResult>(m, "QualitativeCheckResult", "Base class for qualitative model checking results", checkResult)
        .def("exists_true", &QualitativeCheckResult::existsTrue, "Whether at least one state satisfies the property")
        .def("forall_true", &QualitativeCheckResult::forallTrue, "Whether all states satisfy the property")
        .def("count", &QualitativeCheckResult::count, "Number of states satisfying the property")
        .def("complement", &QualitativeCheckResult::complement, "Complement the result");

    using SymbolicQualitative = storm::modelchecker::SymbolicQualitativeCheckResult<Sylvan>;
    stormpy::bindings::bindTemplateClass<SymbolicQualitative, QualitativeCheckResult>(
        m, "SymbolicQualitativeCheckResult", stormpy::bindings::typeIndex<Sylvan>(), "Symbolic qualitative model checking result")
        .def(py::init<storm::dd::Bdd<Sylvan> const&, storm::dd::Bdd<Sylvan> const&>(), "Create result", py::arg("reachable_states"), py::arg("truth_values"))
        .def("get_truth_values_vector", &SymbolicQualitative::getTruthValuesVector, "Get BDD representing the truth values");
}

template<typename ValueType>
void define_modelchecker_results_typed(py::module& m) {
    auto const index = stormpy::bindings::typeIndex<ValueType>();

    using ExplicitQualitative = storm::modelchecker::ExplicitQualitativeCheckResult<ValueType>;
    stormpy::bindings::bindTemplateClass<ExplicitQualitative, QualitativeCheckResult>(m, "ExplicitQualitativeCheckResult", index,
                                                                                      "Explicit qualitative model checking result")
        .def(py::init<storm::storage::BitVector const&>(), "Create result from truth values", py::arg("truth_values"))
        .def(
            "__getitem__",
            [](ExplicitQualitative const& result, storm::storage::sparse::state_type state) {
                checkHasValueForState(result, state);
                return result[state];
            },
            "Get result for given state", py::arg("state"))
        .def("has_value_for_state", &ExplicitQualitative::hasValueForState, "Check whether the result has a value for the given state", py::arg("state"))
        .def("get_states", &ExplicitQualitative::getStates, "Get BitVector of the states for which the result has values", py::return_value_policy::copy)
        .def("get_truth_values_vector", &ExplicitQualitative::getTruthValuesVector, "Get BitVector representing the truth values")
        .def("get_scheduler", py::overload_cast<>(&ExplicitQualitative::getScheduler, py::const_), "Get scheduler",
             py::return_value_policy::reference_internal);

    using Quantitative = storm::modelchecker::QuantitativeCheckResult<ValueType>;
    using ExtendedValueType = typename Quantitative::ExtendedValueType;
    using Bounds = storm::solver::SolutionBounds<ExtendedValueType>;

    auto solutionBounds = stormpy::bindings::bindTemplateClass<Bounds>(
        m, "SolutionBounds", index, "Sound lower and upper bounds on the values of all states of a result. An unknown bound is None.");
    solutionBounds.def_readonly("lower", &Bounds::lower, "Lower bounds (None if not known)")
        .def_readonly("upper", &Bounds::upper, "Upper bounds (None if not known)")
        .def_property_readonly("has_lower", &Bounds::hasLower, "Whether the lower bounds are known")
        .def_property_readonly("has_upper", &Bounds::hasUpper, "Whether the upper bounds are known")
        .def_property_readonly("has_any", &Bounds::hasAny, "Whether at least one of the bounds is known")
        .def_property_readonly("is_exact", &Bounds::isExact, "Whether both bounds are known and coincide");
    if constexpr (std::is_same_v<ValueType, double> || std::is_same_v<ValueType, storm::RationalNumber>) {
        solutionBounds.def("encloses", &Bounds::encloses, "Whether every given value lies within the known bounds", py::arg("values"))
            .def("is_within_precision", &Bounds::isWithinPrecision,
                 "Whether both bounds are known and neither is further from its value than the given precision allows", py::arg("values"), py::arg("precision"),
                 py::arg("relative"));
    }

    using Aggregated = storm::modelchecker::AggregatedValue<ExtendedValueType>;
    stormpy::bindings::bindTemplateClass<Aggregated>(m, "AggregatedValue", index,
                                                     "Aggregate of the values of a result, together with the aggregates of the known bounds")
        .def_readonly("value", &Aggregated::value, "Aggregated value")
        .def_readonly("lower", &Aggregated::lower, "Aggregated lower bound (None if not known)")
        .def_readonly("upper", &Aggregated::upper, "Aggregated upper bound (None if not known)")
        .def_property_readonly("has_lower", &Aggregated::hasLower, "Whether a lower bound is known")
        .def_property_readonly("has_upper", &Aggregated::hasUpper, "Whether an upper bound is known");

    auto quantitativeCheckResult =
        stormpy::bindings::bindTemplateClass<Quantitative, CheckResult>(m, "QuantitativeCheckResult", index, "Base class for quantitative results");
    quantitativeCheckResult.def("get_min", &Quantitative::getMin, "Get minimal value")
        .def("get_max", &Quantitative::getMax, "Get maximal value")
        .def("average", &Quantitative::average, "Get average of the values")
        .def("sum", &Quantitative::sum, "Get sum of the values")
        .def("aggregate", &Quantitative::aggregate, "Aggregate the values together with the known bounds. Supported filters: MIN, MAX, SUM and AVG.",
             py::arg("filter"));

    using ExplicitQuantitative = storm::modelchecker::ExplicitQuantitativeCheckResult<ValueType>;
    // Bound of the given state, if this bound is known
    auto getBound = [](ExplicitQuantitative const& result, storm::storage::sparse::state_type state, bool lower) -> std::optional<ExtendedValueType> {
        checkHasValueForState(result, state);
        if (lower ? !result.hasLowerBounds() : !result.hasUpperBounds()) {
            return std::nullopt;
        }
        // Bounds are stored compressed like the values
        uint64_t const offset = result.getStates().getNumberOfSetBitsBeforeIndex(state);
        return lower ? result.getLowerBoundVector()[offset] : result.getUpperBoundVector()[offset];
    };
    auto explicitQuantitativeCheckResult = stormpy::bindings::bindTemplateClass<ExplicitQuantitative>(
        m, "ExplicitQuantitativeCheckResult", index, "Explicit quantitative model checking result", quantitativeCheckResult);
    explicitQuantitativeCheckResult.def(py::init<std::vector<ValueType>>(), "Create result from values", py::arg("values"))
        .def(
            "get_value",
            [](ExplicitQuantitative const& result, storm::storage::sparse::state_type state) {
                checkHasValueForState(result, state);
                return result[state];
            },
            "Get value for given state (without bounds)", py::arg("state"))
        .def(
            "get_lower_bound",
            [getBound](ExplicitQuantitative const& result, storm::storage::sparse::state_type state) { return getBound(result, state, true); },
            "Get sound lower bound for given state (None if not known)", py::arg("state"))
        .def(
            "get_upper_bound",
            [getBound](ExplicitQuantitative const& result, storm::storage::sparse::state_type state) { return getBound(result, state, false); },
            "Get sound upper bound for given state (None if not known)", py::arg("state"))
        .def("has_lower_bounds", &ExplicitQuantitative::hasLowerBounds, "Whether sound lower bounds on the values are known")
        .def("has_upper_bounds", &ExplicitQuantitative::hasUpperBounds, "Whether sound upper bounds on the values are known")
        .def("get_solution_bounds", &ExplicitQuantitative::getSolutionBounds, "Get sound lower and upper bounds, indexed like get_value_vector()",
             py::return_value_policy::copy)
        .def("get_lower_bound_vector", &ExplicitQuantitative::getLowerBoundVector, "Get sound lower bounds, indexed like get_value_vector()")
        .def("get_upper_bound_vector", &ExplicitQuantitative::getUpperBoundVector, "Get sound upper bounds, indexed like get_value_vector()")
        .def("set_lower_bounds", &ExplicitQuantitative::setLowerBounds, "Set sound lower bounds, indexed like get_value_vector()", py::arg("lower_bounds"))
        .def("set_upper_bounds", &ExplicitQuantitative::setUpperBounds, "Set sound upper bounds, indexed like get_value_vector()", py::arg("upper_bounds"))
        .def("set_values_exact", &ExplicitQuantitative::setValuesExact, "State that the values are exact, i.e., use them as lower and upper bounds")
        .def("clear_bounds", &ExplicitQuantitative::clearBounds, "Remove all bounds")
        .def("has_value_for_state", &ExplicitQuantitative::hasValueForState, "Check whether the result has a value for the given state", py::arg("state"))
        .def("get_states", &ExplicitQuantitative::getStates, "Get BitVector of the states for which the result has values", py::return_value_policy::copy)
        .def("get_value_vector", py::overload_cast<>(&ExplicitQuantitative::getValueVector, py::const_),
             "Get values (possibly infinite) of the states for which the result has values")
        .def("get_finite_value_vector", &ExplicitQuantitative::getFiniteValueVector,
             "Get values for all states as plain values (fails for infinite values which the value type cannot represent)")
        .def("get_scheduler", py::overload_cast<>(&ExplicitQuantitative::getScheduler, py::const_), "Get scheduler",
             py::return_value_policy::reference_internal);
    if constexpr (std::is_same_v<ValueType, storm::RationalFunction>) {
        // Parametric results have no meaningful bounds. For the other value types, __getitem__ returning the value with its bounds is defined in Python.
        explicitQuantitativeCheckResult.def(
            "__getitem__",
            [](ExplicitQuantitative const& result, storm::storage::sparse::state_type state) {
                checkHasValueForState(result, state);
                return result[state];
            },
            "Get result for given state", py::arg("state"));
    }

    using SymbolicQuantitative = storm::modelchecker::SymbolicQuantitativeCheckResult<Sylvan, ValueType>;
    stormpy::bindings::bindTemplateClass<SymbolicQuantitative>(m, "SymbolicQuantitativeCheckResult", stormpy::bindings::typeIndex<Sylvan, ValueType>(),
                                                               "Symbolic quantitative model checking result", quantitativeCheckResult)
        .def(
            // Thin wrapper returning the cloned result with its concrete type
            "clone", [](SymbolicQuantitative const& result) { return result.clone()->template asSymbolicQuantitativeCheckResult<Sylvan, ValueType>(); },
            "Clone the result")
        .def("get_value_vector", &SymbolicQuantitative::getValueVector, "Get ADD representing the values");

    using HybridQuantitative = storm::modelchecker::HybridQuantitativeCheckResult<Sylvan, ValueType>;
    stormpy::bindings::bindTemplateClass<HybridQuantitative>(m, "HybridQuantitativeCheckResult", stormpy::bindings::typeIndex<Sylvan, ValueType>(),
                                                             "Hybrid quantitative model checking result", quantitativeCheckResult)
        .def("get_explicit_value_vector", &HybridQuantitative::getExplicitValueVector, "Get values for all states");

    if constexpr (std::is_same_v<ValueType, double> || std::is_same_v<ValueType, storm::RationalNumber>) {
        using ParetoCurve = storm::modelchecker::ParetoCurveCheckResult<ValueType>;
        auto paretoCurveCheckResult =
            stormpy::bindings::bindTemplateClass<ParetoCurve, CheckResult>(m, "ParetoCurveCheckResult", index, "Result of multi-objective model checking");
        paretoCurveCheckResult.def("get_under_approximation", &ParetoCurve::getUnderApproximation, "Get under-approximation of the Pareto curve")
            .def("get_over_approximation", &ParetoCurve::getOverApproximation, "Get over-approximation of the Pareto curve");

        stormpy::bindings::bindTemplateClass<storm::modelchecker::ExplicitParetoCurveCheckResult<ValueType>>(
            m, "ExplicitParetoCurveCheckResult", index, "Result of explicit multi-objective model checking", paretoCurveCheckResult);
    }
}

template void define_modelchecker_results_typed<double>(py::module& m);
template void define_modelchecker_results_typed<storm::RationalNumber>(py::module& m);
template void define_modelchecker_results_typed<storm::RationalFunction>(py::module& m);

}  // namespace stormpy
