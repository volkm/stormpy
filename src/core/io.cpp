#include "io.h"

#include <storm/io/DirectEncodingExporterOptions.h>

namespace stormpy {

void define_io(py::module& m) {
    py::classh<storm::io::DirectEncodingExporterOptions>(m, "DirectEncodingExporterOptions", "Options for the DRN exporter")
        .def(py::init<>(), "Create default options")
        .def_readwrite("allow_placeholders", &storm::io::DirectEncodingExporterOptions::allowPlaceholders, "Allow placeholders for repeated values")
        .def_readwrite("output_precision", &storm::io::DirectEncodingExporterOptions::outputPrecision, "Precision of exported values");
}

}  // namespace stormpy
