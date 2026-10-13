import pathlib
import pytest
import os

import stormpy
from stormpy.exceptions import StormError
from helpers.helper import get_example_path
from configurations import dft


@dft
class TestDftLoad:
    def test_load_dft_galileo_file(self):
        dft = stormpy.dft.load_dft_galileo_file(get_example_path("dft", "hecs.dft"))
        assert dft.nr_elements() == 23
        assert dft.nr_basic_elements() == 13
        assert dft.nr_dynamic_elements() == 2
        assert not dft.can_have_nondeterminism()

    def test_load_dft_json_file(self):
        dft = stormpy.dft.load_dft_json_file(get_example_path("dft", "and.json"))
        assert dft.nr_elements() == 3
        assert dft.nr_basic_elements() == 2
        assert dft.nr_dynamic_elements() == 0
        assert not dft.can_have_nondeterminism()

    def test_load_dft_json_string(self):
        # Build json string
        json_node_a = '{"data": {"id":"0", "name":"A", "type":"be", "rate":"1", "dorm":"1", "label":"A (1)"}, "group":"nodes", "classes":"be"}'
        json_node_b = '{"data": {"id":"1", "name":"B", "type":"be", "rate":"1", "dorm":"1", "label":"B (1)"}, "group":"nodes", "classes":"be"}'
        json_node_c = '{"data": {"id":"6", "name":"Z", "type":"pand", "children":["0", "1"], "label":"Z"}, "group":"nodes", "classes":"pand"}'
        json_string = '{"toplevel": "6", "parameters": {}, "nodes": [' + json_node_a + "," + json_node_b + "," + json_node_c + "]}"
        # Load
        dft = stormpy.dft.load_dft_json_string(json_string)
        assert dft.nr_elements() == 3
        assert dft.nr_basic_elements() == 2
        assert dft.nr_dynamic_elements() == 1
        assert not dft.can_have_nondeterminism()

    def test_load_parametric_dft_json_string(self):
        json_node_a = '{"data": {"id":"0", "name":"A", "type":"be", "rate":"x", "dorm":"1", "label":"A (x)"}, "group":"nodes", "classes":"be"}'
        json_node_b = '{"data": {"id":"1", "name":"B", "type":"be", "rate":"1", "dorm":"1", "label":"B (1)"}, "group":"nodes", "classes":"be"}'
        json_node_c = '{"data": {"id":"6", "name":"Z", "type":"and", "children":["0", "1"], "label":"Z"}, "group":"nodes", "classes":"and"}'
        json_string = '{"toplevel": "6", "parameters": ["x"], "nodes": [' + json_node_a + "," + json_node_b + "," + json_node_c + "]}"
        # Load
        dft = stormpy.dft.load_dft_json_string[stormpy.RationalFunction](json_string)
        assert type(dft) is stormpy.dft.DFT[stormpy.RationalFunction]
        assert dft.nr_elements() == 3
        assert dft.nr_basic_elements() == 2
        assert dft.nr_dynamic_elements() == 0
        assert not dft.can_have_nondeterminism()
        parameter_names = {parameter.name for parameter in dft.get_parameters()}
        assert parameter_names == {"x"}

    def test_load_from_path(self):
        dft = stormpy.dft.load_dft_json_file(pathlib.Path(get_example_path("dft", "and.json")))
        assert dft.nr_elements() == 3
        dft = stormpy.dft.load_dft_galileo_file[stormpy.RationalFunction](pathlib.Path(get_example_path("dft", "symmetry_param.dft")))
        assert type(dft) is stormpy.dft.DFT[stormpy.RationalFunction]
        with pytest.raises(TypeError):
            stormpy.dft.load_dft_json_file(42)


@dft
class TestDftExport:
    def test_export_dft_json_string(self):
        dft = stormpy.dft.load_dft_galileo_file(get_example_path("dft", "hecs.dft"))
        assert dft.nr_elements() == 23
        assert dft.nr_basic_elements() == 13
        assert dft.nr_dynamic_elements() == 2
        json_string = stormpy.dft.export_dft_json_string(dft)
        dft2 = stormpy.dft.load_dft_json_string(json_string)
        assert dft2.nr_elements() == 23
        assert dft2.nr_basic_elements() == 13
        assert dft2.nr_dynamic_elements() == 2

    def test_export_dft_json_file(self, tmp_path):
        dft = stormpy.dft.load_dft_galileo_file(get_example_path("dft", "hecs.dft"))
        assert dft.nr_elements() == 23
        assert dft.nr_basic_elements() == 13
        assert dft.nr_dynamic_elements() == 2
        export_file = tmp_path / "hecs.json"
        stormpy.dft.export_dft_json_file(dft, export_file)
        dft2 = stormpy.dft.load_dft_json_file(export_file)
        assert dft2.nr_elements() == 23
        assert dft2.nr_basic_elements() == 13
        assert dft2.nr_dynamic_elements() == 2

    def test_export_parametric_dft_json_string(self):
        dft = stormpy.dft.load_dft_galileo_file[stormpy.RationalFunction](get_example_path("dft", "symmetry_param.dft"))
        parameter_names = {parameter.name for parameter in dft.get_parameters()}
        assert parameter_names == {"x", "y"}

        json_string = stormpy.dft.export_dft_json_string(dft)
        dft2 = stormpy.dft.load_dft_json_string[stormpy.RationalFunction](json_string)

        assert type(dft2) is stormpy.dft.DFT[stormpy.RationalFunction]
        assert dft2.nr_elements() == dft.nr_elements()
        assert dft2.nr_basic_elements() == dft.nr_basic_elements()
        assert dft2.nr_dynamic_elements() == dft.nr_dynamic_elements()
        assert {parameter.name for parameter in dft2.get_parameters()} == parameter_names

    def test_export_parametric_dft_json_file(self, tmp_path):
        dft = stormpy.dft.load_dft_galileo_file[stormpy.RationalFunction](get_example_path("dft", "symmetry_param.dft"))
        parameter_names = {parameter.name for parameter in dft.get_parameters()}
        assert parameter_names == {"x", "y"}

        export_file = tmp_path / "symmetry_param.json"
        stormpy.dft.export_dft_json_file(dft, export_file)
        dft2 = stormpy.dft.load_dft_json_file[stormpy.RationalFunction](export_file)

        assert type(dft2) is stormpy.dft.DFT[stormpy.RationalFunction]
        assert dft2.nr_elements() == dft.nr_elements()
        assert dft2.nr_basic_elements() == dft.nr_basic_elements()
        assert dft2.nr_dynamic_elements() == dft.nr_dynamic_elements()
        assert {parameter.name for parameter in dft2.get_parameters()} == parameter_names

    def test_load_unsupported_value_type(self):
        with pytest.raises(TypeError, match="Supported are: float, "):
            stormpy.dft.load_dft_json_file[stormpy.Rational](get_example_path("dft", "and.json"))

    def test_load_published_template_function(self):
        assert repr(stormpy.dft.load_dft_json_file) == "<template function stormpy.dft.load_dft_json_file>"
        dft = stormpy.dft.load_dft_json_file[stormpy.RationalFunction](get_example_path("dft", "and.json"))
        assert type(dft) is stormpy.dft.DFT[stormpy.RationalFunction]

    def test_load_developer_template_function(self):
        dft = stormpy.dft.developer.load_dft_json_file[stormpy.RationalFunction](get_example_path("dft", "and.json"))
        assert type(dft) is stormpy.dft.DFT[stormpy.RationalFunction]
        assert type(stormpy.dft.developer.load_dft_json_file(get_example_path("dft", "and.json"))) is stormpy.dft.DFT[float]
