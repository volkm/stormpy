import stormpy
import stormpy.pars

import stormpy.examples
import stormpy.examples.files


def example_parametric_models_01():
    path = stormpy.examples.files.prism_pdtmc_die
    prism_program = stormpy.parse_prism_program(path)

    formula_str = "P=? [F s=7 & d=2]"
    properties = stormpy.parse_properties(formula_str, prism_program)
    model = stormpy.build_model[stormpy.RationalFunction](prism_program, properties)
    print("Model supports parameters: {}".format(model.supports_parameters))
    parameters = model.collect_all_parameters()
    assert len(parameters) == 2

    instantiator = stormpy.pars.ModelInstantiator[stormpy.models.ModelType.DTMC, float](model)
    point = dict()
    for x in parameters:
        print(x.name)
        point[x] = stormpy.RationalFunctionCoefficient(0.4)
    instantiated_model = instantiator.instantiate(point)
    result = stormpy.model_checking(instantiated_model, properties[0])
    print(result)


if __name__ == "__main__":
    example_parametric_models_01()
