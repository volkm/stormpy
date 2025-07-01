from deprecated.sphinx import deprecated


@deprecated(
    version="1.13.0", reason="Stormpy was restructured and this general import will be removed in the future. Please revise your imports to the new structure."
)
def _warn():
    pass


_warn()

from .. import *
from ..logic import *
from ..utility import *
from ..storage import *

from .. import info as info
from .. import exceptions as exceptions
from .. import dft as dft
from .. import gspn as gspn
from .. import pars as pars
from .. import pomdp as pomdp
from .. import examples as examples
from .. import pycarl as pycarl
from .. import _config as _config
from .. import __version__ as __version__

import sys as _sys


def _stormpy_getattr(name):
    for _mod_name in (
        "stormpy.logic",
        "stormpy.utility",
        "stormpy.storage",
    ):
        _mod = _sys.modules.get(_mod_name)
        if _mod and hasattr(_mod, name):
            return getattr(_mod, name)
    raise AttributeError(f"module 'stormpy' has no attribute '{name}'")


_sys.modules["stormpy"].__getattr__ = _stormpy_getattr
