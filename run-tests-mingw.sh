#!/bin/bash

MAKE_FLAGS=-j1
CMAKE=../../libs/cmake-2.8.11.2-win32-x86/bin/cmake.exe

# $1:    build folder
# $2:    C-compiler
# $3:    C++-compiler
# $4:    CMake options
function run_test
{
  mkdir -p $1

  export CC=$2
  export CXX=$3

  FOLDER=$PWD
  cd $1 && $CMAKE -G "MSYS Makefiles" $4 .. && make $MAKE_FLAGS && make check 2>&1 | tee output.log

  cd $FOLDER

  unset CC CXX
}

run_test build-g++-4.8 /mingw/bin/gcc /mingw/bin/g++ ""
run_test build-g++-4.8-c++11 /mingw/bin/gcc /mingw/bin/g++ "-DCPP11:STRING=YES"

run_test build-clang-3.3 /mingw/bin/clang /mingw/bin/clang++ ""
run_test build-clang-3.3-c++11 /mingw/bin/clang /mingw/bin/clang++ "-DCPP11:STRING=YES"

# Display test run results very briefly
tail -n2 build-*/output.log
