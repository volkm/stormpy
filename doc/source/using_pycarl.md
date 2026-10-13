---
jupytext:
  formats: ipynb,md:myst
  text_representation:
    extension: .md
    format_name: myst
    format_version: 0.13
kernelspec:
  display_name: Python 3
  language: python
  name: python3
---

# Getting Started with Pycarl
Pycarl provides the bindings of the carl library, which Storm uses for its number types, polynomials and rational functions.
Pycarl is shipped with the stormpy library as `stormpy.numbers.developer` and can be installed following the [general installation instructions](installation).
The number types matching Storm's value types (e.g. `stormpy.numbers.Rational`), the arbitrary-precision `stormpy.numbers.Integer` and `stormpy.numbers.Variable` are available in the public module `stormpy.numbers`.
For convenience, the value types are also available directly in `stormpy`, e.g. `stormpy.Rational` is the same as `stormpy.numbers.Rational`.
The complete bindings in `stormpy.numbers.developer` are mainly meant for developers.

## A Quick Tour through pycarl
First we import pycarl:

```{code-cell} python3
import stormpy.numbers
```

Pycarl can use two different number types: `gmp` and `cln`.
The number types in `stormpy.numbers` (e.g. `stormpy.numbers.Integer`) use the same backend as Storm.
The backends can also be used explicitly via `stormpy.numbers.developer.gmp` and `stormpy.numbers.developer.cln`.

### Simple arithmetic operations
[01-getting-started.py](https://github.com/stormchecker/stormpy/blob/master/examples/pycarl/01-pycarl-getting-started.py)

We start by doing some simple arithmetic operations.
First we create two variables `x` and `y`:

```{code-cell} python3
stormpy.numbers.clear_pools()
x = stormpy.numbers.Variable("x")
y = stormpy.numbers.Variable("y")
```

We perform some operations on polynomials by using the common arithmetic operations of Python:

```{code-cell} python3
pol1 = x * x + stormpy.numbers.Integer(2)
pol2 = y + stormpy.numbers.Integer(1)
result = pol1 * pol2
print("({}) * ({}) = {}".format(pol1, pol2, result))
```
