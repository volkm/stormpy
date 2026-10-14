from stormpy.numbers import developer as pycarl
from stormpy.numbers.developer.formula import Relation


class TestRelation:
    def test_to_string(self):
        r = Relation.LESS
        assert r.friendly_name() == "<"
        assert Relation.GEQ.friendly_name() == ">="

    def test_name(self):
        assert Relation.LESS.name == "LESS"
        assert Relation.GEQ.name == "GEQ"
