#include "solver.h"

#include <storm/solver/OptimizationDirection.h>
#include <storm/solver/SolverSelectionOptions.h>
#include <storm/solver/UncertaintyResolutionMode.h>

namespace stormpy {

void define_solver(py::module& m) {
    py::native_enum<storm::solver::OptimizationDirection>(m, "OptimizationDirection", "enum.Enum", "Direction of optimization")
        .value("MINIMIZE", storm::solver::OptimizationDirection::Minimize)
        .value("MAXIMIZE", storm::solver::OptimizationDirection::Maximize)
        .finalize();

    py::native_enum<storm::solver::UncertaintyResolutionMode>(m, "UncertaintyResolutionMode", "enum.Enum", "How uncertainty in interval models is resolved")
        .value("MINIMIZE", storm::solver::UncertaintyResolutionMode::Minimize)
        .value("MAXIMIZE", storm::solver::UncertaintyResolutionMode::Maximize)
        .value("ROBUST", storm::solver::UncertaintyResolutionMode::Robust)
        .value("COOPERATIVE", storm::solver::UncertaintyResolutionMode::Cooperative)
        .value("UNSET", storm::solver::UncertaintyResolutionMode::Unset)
        .finalize();

    py::native_enum<storm::solver::EquationSolverType>(m, "EquationSolverType", "enum.Enum", "Solver type for equation systems")
        .value("NATIVE", storm::solver::EquationSolverType::Native)
        .value("GMMXX", storm::solver::EquationSolverType::Gmmxx)
        .value("EIGEN", storm::solver::EquationSolverType::Eigen)
        .value("ELIMINATION", storm::solver::EquationSolverType::Elimination)
        .value("TOPOLOGICAL", storm::solver::EquationSolverType::Topological)
        .value("ACYCLIC", storm::solver::EquationSolverType::Acyclic)
        .finalize();

    py::native_enum<storm::solver::NativeLinearEquationSolverMethod>(m, "NativeLinearEquationSolverMethod", "enum.Enum",
                                                                     "Method for linear equation systems with the native solver")
        .value("JACOBI", storm::solver::NativeLinearEquationSolverMethod::Jacobi)
        .value("GAUSS_SEIDEL", storm::solver::NativeLinearEquationSolverMethod::GaussSeidel)
        .value("SOR", storm::solver::NativeLinearEquationSolverMethod::SOR)
        .value("WALKER_CHAE", storm::solver::NativeLinearEquationSolverMethod::WalkerChae)
        .value("POWER", storm::solver::NativeLinearEquationSolverMethod::Power)
        .value("SOUND_VALUE_ITERATION", storm::solver::NativeLinearEquationSolverMethod::SoundValueIteration)
        .value("OPTIMISTIC_VALUE_ITERATION", storm::solver::NativeLinearEquationSolverMethod::OptimisticValueIteration)
        .value("INTERVAL_ITERATION", storm::solver::NativeLinearEquationSolverMethod::IntervalIteration)
        .value("RATIONAL_SEARCH", storm::solver::NativeLinearEquationSolverMethod::RationalSearch)
        .finalize();

    py::native_enum<storm::solver::MinMaxMethod>(m, "MinMaxMethod", "enum.Enum", "Method for min-max equation systems")
        .value("VALUE_ITERATION", storm::solver::MinMaxMethod::ValueIteration)
        .value("POLICY_ITERATION", storm::solver::MinMaxMethod::PolicyIteration)
        .value("LINEAR_PROGRAMMING", storm::solver::MinMaxMethod::LinearProgramming)
        .value("TOPOLOGICAL", storm::solver::MinMaxMethod::Topological)
        .value("RATIONAL_SEARCH", storm::solver::MinMaxMethod::RationalSearch)
        .value("INTERVAL_ITERATION", storm::solver::MinMaxMethod::IntervalIteration)
        .value("SOUND_VALUE_ITERATION", storm::solver::MinMaxMethod::SoundValueIteration)
        .value("OPTIMISTIC_VALUE_ITERATION", storm::solver::MinMaxMethod::OptimisticValueIteration)
        .finalize();
}

}  // namespace stormpy
