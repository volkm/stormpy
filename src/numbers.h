#pragma once

#include <cmath>
#include <limits>
#include <storm/adapters/IntervalAdapter.h>
#include <storm/adapters/RationalFunctionAdapter.h>
#include <storm/utility/ExtendedNumber.h>

// Convert storm::utility::ExtendedNumber<T> (value type extended with +/- infinity) from/to Python.
// Finite values are converted as T, infinite values as float('inf') / float('-inf').
namespace pybind11 {
namespace detail {

template<typename T>
struct type_caster<storm::utility::ExtendedNumber<T>> {
    using ExtendedNumber = storm::utility::ExtendedNumber<T>;

   public:
    PYBIND11_TYPE_CASTER(ExtendedNumber, make_caster<T>::name);

    bool load(handle src, bool convert) {
        if (PyFloat_Check(src.ptr())) {
            double const value = PyFloat_AsDouble(src.ptr());
            if (std::isinf(value)) {
                this->value = value > 0 ? ExtendedNumber::infinity() : ExtendedNumber::negativeInfinity();
                return true;
            }
        }
        make_caster<T> finiteCaster;
        if (!finiteCaster.load(src, convert)) {
            return false;
        }
        this->value = ExtendedNumber(cast_op<T&&>(std::move(finiteCaster)));
        return true;
    }

    static handle cast(ExtendedNumber const& src, return_value_policy /* policy */, handle parent) {
        if (src.isPositiveInfinity()) {
            return PyFloat_FromDouble(std::numeric_limits<double>::infinity());
        }
        if (src.isNegativeInfinity()) {
            return PyFloat_FromDouble(-std::numeric_limits<double>::infinity());
        }
        return make_caster<T>::cast(src.getFinite(), return_value_policy::copy, parent);
    }
};

}  // namespace detail
}  // namespace pybind11
