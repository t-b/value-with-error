#!/bin/bash

MAKE_FLAGS=-j6

# $1:    build folder
# $2:    C-compiler
# $3:    C++-compiler
# $4:    CMake options
function run_test
{
  rm -rf $1
  mkdir -p $1

  export CC=$2
  export CXX=$3

  FOLDER=$PWD
  cd $1 && cmake $4 .. && make $MAKE_FLAGS && make check 2>&1 | tee output.log
  cd $FOLDER

  unset CC CXX
}

run_test build-g++-4.6 /usr/bin/gcc-4.6 /usr/bin/g++-4.6 ""

run_test build-g++-4.7 /usr/bin/gcc-4.7 /usr/bin/g++-4.7 ""
run_test build-g++-4.7-c++11 /usr/bin/gcc-4.7 /usr/bin/g++-4.7 "-DCPP11:STRING=YES"

run_test build-g++-4.8 $HOME/gcc-4.8.2/bin/gcc $HOME/gcc-4.8.2/bin/g++ ""
run_test build-g++-4.8-c++11 $HOME/gcc-4.8.2/bin/gcc $HOME/gcc-4.8.2/bin/g++ "-DCPP11:STRING=YES"

run_test build-g++-trunk $HOME/gcc-trunk/bin/gcc $HOME/gcc-trunk/bin/g++ ""
run_test build-g++-trunk-c++11 $HOME/gcc-trunk/bin/gcc $HOME/gcc-trunk/bin/g++ "-DCPP11:STRING=YES"

run_test build-clang-3.3 /usr/local/bin/clang /usr/local/bin/clang++ ""
run_test build-clang-3.3-c++11 /usr/local/bin/clang /usr/local/bin/clang++ "-DCPP11:STRING=YES"

run_test build-clang-3.4 /usr/bin/clang /usr/bin/clang++ ""
run_test build-clang-3.4-c++11 /usr/bin/clang /usr/bin/clang++ "-DCPP11:STRING=YES"

# Display test run results very briefly
tail -n2 build-*/output.log
