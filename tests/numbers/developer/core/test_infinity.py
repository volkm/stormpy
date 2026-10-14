from stormpy.numbers import developer as pycarl


def test_infinity_comparison():
    assert not pycarl.inf < pycarl.inf
    assert -pycarl.inf < pycarl.inf
    assert not pycarl.inf < -pycarl.inf
    assert not -pycarl.inf < -pycarl.inf
    assert not pycarl.inf > pycarl.inf
    assert not -pycarl.inf > pycarl.inf
    assert pycarl.inf > -pycarl.inf
    assert not -pycarl.inf > -pycarl.inf
    assert pycarl.inf <= pycarl.inf
    assert -pycarl.inf <= pycarl.inf
    assert not pycarl.inf <= -pycarl.inf
    assert -pycarl.inf <= -pycarl.inf
    assert pycarl.inf >= pycarl.inf
    assert not -pycarl.inf >= pycarl.inf
    assert pycarl.inf >= -pycarl.inf
    assert -pycarl.inf >= -pycarl.inf
