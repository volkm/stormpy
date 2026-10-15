"""Python conveniences for model checking results."""

from __future__ import annotations

from typing import Any

from stormpy import Rational
from stormpy import developer


class BoundedValue:
    """
    Value of a single state in a model checking result, together with sound lower and upper bounds on the actual value.

    The bounds are given if the solution method could establish them, e.g., for sound or exact methods.
    An unknown bound is ``None``. An infinite bound means the method tried but could not establish a finite bound.

    The object behaves like its value in comparisons with numbers and when converting to ``float``.
    """

    __slots__ = ("_value", "_lower", "_upper")

    def __init__(self, value: Any, lower: Any = None, upper: Any = None):
        """
        Create a value with bounds.

        :param value: The (approximate) value.
        :param lower: Sound lower bound on the actual value (None if not known).
        :param upper: Sound upper bound on the actual value (None if not known).
        """
        self._value = value
        self._lower = lower
        self._upper = upper

    @property
    def value(self) -> Any:
        """The (approximate) value."""
        return self._value

    @property
    def lower(self) -> Any:
        """Sound lower bound on the actual value (None if not known)."""
        return self._lower

    @property
    def upper(self) -> Any:
        """Sound upper bound on the actual value (None if not known)."""
        return self._upper

    @property
    def has_lower(self) -> bool:
        """Whether a lower bound is known."""
        return self._lower is not None

    @property
    def has_upper(self) -> bool:
        """Whether an upper bound is known."""
        return self._upper is not None

    @property
    def is_exact(self) -> bool:
        """Whether both bounds are known and coincide, i.e., the actual value is known exactly."""
        return self.has_lower and self.has_upper and self._lower == self._upper

    def __float__(self) -> float:
        return float(self._value)

    def __round__(self, ndigits: int | None = None) -> Any:
        return round(self._value, ndigits)

    def __format__(self, format_spec: str) -> str:
        # Without format specification (e.g. "{}".format(x)) behave like str(), otherwise format the value
        return str(self) if not format_spec else format(self._value, format_spec)

    def __eq__(self, other: object) -> bool:
        if isinstance(other, BoundedValue):
            return self._value == other._value and self._lower == other._lower and self._upper == other._upper
        return self._value == other

    def __hash__(self) -> int:
        return hash(self._value)

    @staticmethod
    def _plain(other: object) -> object:
        return other._value if isinstance(other, BoundedValue) else other

    def __lt__(self, other: object) -> bool:
        return self._value < self._plain(other)

    def __le__(self, other: object) -> bool:
        return self._value <= self._plain(other)

    def __gt__(self, other: object) -> bool:
        return self._value > self._plain(other)

    def __ge__(self, other: object) -> bool:
        return self._value >= self._plain(other)

    def __repr__(self) -> str:
        return f"BoundedValue({self._value!r}, lower={self._lower!r}, upper={self._upper!r})"

    def __str__(self) -> str:
        if not self.has_lower and not self.has_upper:
            return str(self._value)
        lower = "-" if self._lower is None else str(self._lower)
        upper = "-" if self._upper is None else str(self._upper)
        return f"{self._value} [{lower}, {upper}]"


def _get_item_with_bounds(self, state: int) -> BoundedValue:
    """
    Get the value for the given state together with its sound bounds.

    Use :meth:`get_value` to get only the value.

    :param state: State.
    :return: Value with bounds.
    """
    return BoundedValue(self.get_value(state), self.get_lower_bound(state), self.get_upper_bound(state))


# Parametric results keep returning the plain value (defined in the bindings)
for _value_type in (float, Rational):
    developer.ExplicitQuantitativeCheckResult[_value_type].__getitem__ = _get_item_with_bounds
del _value_type
