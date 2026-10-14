"""Raw bindings of the carl library (number types, polynomials, constraints and formulas).

This module is only meant for developers. Use :mod:`stormpy.numbers` for the public API.

The naming of functions and classes of the bindings mirrors the corresponding ``carl`` C++ functions and classes.
"""

from . import _pycarl_core

from . import _numbers
from ._pycarl_core import *
from . import infinity
from stormpy.numbers.developer import _config

inf = infinity.Infinity()


def carl_version():
    """
    Get Carl version.
    :return: Version of Carl.
    """
    return _config.CARL_VERSION


def has_cln():
    """
    Check if pycarl has support for CLN.
    :return: True iff CLN is supported.
    """
    return _config.CARL_WITH_CLN


def clear_pools():
    """
    Clear all pools.
    """
    # clear_monomial_pool()
    clear_variable_pool()
