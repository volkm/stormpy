#include "settings.h"

#include <storm/environment/Environment.h>
#include <storm/settings/EnvironmentBuilder.h>
#include <storm/settings/SettingsManager.h>

namespace stormpy {

void define_settings(py::module& m) {
    m.def(
        // Thin wrapper as the SettingsManager itself is not bound
        "set_from_exploded_string",
        [](std::vector<std::string> const& commandLineArguments) { storm::settings::mutableManager().setFromExplodedString(commandLineArguments); },
        "Set Storm settings from command line arguments", py::arg("command_line_arguments"));

    py::classh<storm::settings::EnvironmentBuilder>(m, "EnvironmentBuilder", "Builds an environment from the current Storm settings")
        .def_static("build_environment", &storm::settings::EnvironmentBuilder::buildEnvironment,
                    "Build an environment whose solver and model checker configuration is taken from the current Storm settings");
}

}  // namespace stormpy
