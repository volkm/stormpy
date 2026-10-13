import stormpy


def test_counterexample_options_default():
    options = stormpy.developer.SMTMinimalLabelSetGeneratorOptions()
    assert type(options) is stormpy.developer.SMTMinimalLabelSetGeneratorOptions[float]
