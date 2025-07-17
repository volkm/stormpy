#!/bin/bash

# Inspired by build process of spead2

set -e -u

brew install ccache automake boost cln ginac glpk hwloc z3 xerces-c

# Install Storm
git clone https://github.com/moves-rwth/storm.git -b ${STORM_VERSION}
cd storm
mkdir build
cd build
cmake .. -DSTORM_BUILD_TESTS=OFF -DSTORM_BUILD_EXECUTABLES=OFF -DSTORM_PORTABLE=ON
make -j ${NR_JOBS}
sudo make install # Needed for macOS 14
cd ..
rm -rf build
