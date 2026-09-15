#!/bin/bash

# Inspired by build process of spead2

set -e -u

ginac_version=1.8.10

dnf install -y boost-devel cln-devel glpk-devel gmp-devel hwloc-devel libarchive-devel xerces-c-devel z3-devel eigen3-devel # missing ginac-devel

cd /tmp

# Install ginac
curl -fsSLO https://www.ginac.de/ginac-${ginac_version}.tar.bz2
tar -jxf ginac-${ginac_version}.tar.bz2
cd ginac-${ginac_version}
./configure CXXFLAGS="-O2"
make -j ${NR_JOBS}
make install
cd ..

# TEMPORARY, for validation only -- see plan/PR discussion. The permanent fix belongs
# in Storm's resources/3rdparty/include_spot.cmake (single source of truth for every
# Storm consumer); this stormpy-side copy exists only to confirm the flag actually
# works using stormpy's own CI before opening that PR, and will be reverted once the
# Storm-side fix is merged.
#
# Spot (a Storm dependency, built below) links against libatomic when the target CPU
# isn't guaranteed to support CMPXCHG16B -- relevant here because Storm is built with
# -DSTORM_PORTABLE=ON, i.e. for a generic baseline CPU (see spot/m4/l_atomic.m4,
# CHECK_ATOMIC: it probes with AC_LINK_IFELSE and only appends "-latomic" to LIBS if a
# plain std::atomic compare-exchange fails to link without it). Pre-seed LIBS here with
# a *statically* linked libatomic so that first link check already succeeds, and Spot's
# ./configure never appends its own dynamic -latomic. Otherwise libspot.so ends up with
# a runtime dependency on libatomic.so.1 that's missing by default on some
# distributions (e.g. plain ubuntu/fedora images), breaking `import stormpy` there.
export LIBS="-Wl,-Bstatic -latomic -Wl,-Bdynamic"

# Install Storm
git clone https://github.com/stormchecker/storm.git -b ${STORM_VERSION}
cd storm
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE:-Release} -DSTORM_BUILD_TESTS=OFF -DSTORM_BUILD_EXECUTABLES=OFF -DSTORM_PORTABLE=ON
make -j ${NR_JOBS}
make install
cd ..
rm -rf build
