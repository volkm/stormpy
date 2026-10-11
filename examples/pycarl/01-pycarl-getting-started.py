import stormpy.numbers
from stormpy.numbers.developer import gmp


def pycarl_example_getting_started_01():
    stormpy.numbers.clear_pools()
    x = stormpy.numbers.Variable("x")
    y = stormpy.numbers.Variable("y")
    pol1 = x * x + gmp.Integer(2)
    pol2 = y + gmp.Integer(1)
    result = pol1 * pol2
    print("({}) * ({}) = {}".format(pol1, pol2, result))


if __name__ == "__main__":
    pycarl_example_getting_started_01()
