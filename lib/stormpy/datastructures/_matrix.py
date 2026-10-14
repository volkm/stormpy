"""Construction of sparse matrices from NumPy arrays."""

from __future__ import annotations

from collections.abc import Sequence
from typing import TYPE_CHECKING

from stormpy import Rational, RationalFunction, Interval, RationalInterval
from stormpy._template import _deduce_default, _template_function
from . import SparseMatrix, SparseMatrixBuilder

if TYPE_CHECKING:
    import numpy as np


@_template_function(
    "stormpy.datastructures.build_sparse_matrix",
    parameters=("ValueType",),
    instantiations=(float, Rational, RationalFunction, Interval, RationalInterval),
    deduce=_deduce_default(float),
)
def build_sparse_matrix(value_type: type, array: np.ndarray, row_group_indices: Sequence[int] | None = None) -> SparseMatrix:
    """Build a sparse matrix from a two-dimensional NumPy array.

    Zero entries are skipped. The value type is selected by subscription, e.g.
    ``build_sparse_matrix[stormpy.Rational](array)``. It defaults to ``float``.

    :param array: Two-dimensional array containing matrix values.
    :param row_group_indices: Optional ascending starting row indices of row groups.
    :return: Sparse matrix with the shape of the input array.
    """
    num_row = array.shape[0]
    num_col = array.shape[1]
    row_group_indices = [] if row_group_indices is None else row_group_indices

    len_group_indices = len(row_group_indices)
    nz_rows, nz_cols = array.nonzero()
    nz_vals = array[nz_rows, nz_cols]
    builder_class = SparseMatrixBuilder[value_type]
    if len_group_indices > 0:
        builder = builder_class(rows=num_row, columns=num_col, entries=len(nz_rows), has_custom_row_grouping=True, row_groups=len_group_indices)
    else:
        builder = builder_class(rows=num_row, columns=num_col, entries=len(nz_rows))
    builder.add_next_values(nz_rows.tolist(), nz_cols.tolist(), nz_vals.tolist(), row_group_indices)
    return builder.build()
