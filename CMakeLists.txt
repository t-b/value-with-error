cmake_minimum_required(VERSION 2.8)

Project(unit-test)

SET(SYSTEM_LIBRARIES ../libs)

# SET(Boost_DEBUG 1)
SET(Boost_NO_SYSTEM_PATHS true)
SET(Boost_USE_STATIC_LIBS true)
SET(BOOST_ROOT "${SYSTEM_LIBRARIES}/boost_1_55_0" CACHE STRING "boost directory")

# required for mingw build clang
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND WIN32)
  SET(Boost_COMPILER "-clang33")
endif()

find_package(Boost 1.55 COMPONENTS unit_test_framework timer chrono system REQUIRED)

# add a target to generate API documentation with Doxygen
# find_package for Doxygen looks only in the installation folder and not in a custom folder
# find_package(Doxygen REQUIRED)

if(WIN32)
  SET(DOXYGEN_EXECUTABLE "${CMAKE_SOURCE_DIR}/${SYSTEM_LIBRARIES}/doxygen-1.8.5-win32/doxygen.exe")
elseif(UNIX)
  SET(DOXYGEN_EXECUTABLE "${CMAKE_SOURCE_DIR}/${SYSTEM_LIBRARIES}/doxygen-1.8.5-linux/doxygen")
else()
  MESSAGE(ERROR "Could not locate doxygen binary")
endif()

configure_file(doc/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
add_custom_target(doc
  ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating API documentation with Doxygen"
  VERBATIM
)

SET(TEST_EXECUTABLE unit-test)

add_custom_target(check
  ./${TEST_EXECUTABLE} --log_level=test_suite
  DEPENDS ${TEST_EXECUTABLE}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  VERBATIM
)

include_directories(src tests ${Boost_INCLUDE_DIR} ${SYSTEM_LIBRARIES}/eigen-ffa86ffb5570)

SET(COMMON_SOURCES src/DetailValueWithError.hpp
                   src/ValueWithErrorComparisonPolicy.hpp
                   tests/common.cpp
                   tests/precompiled.cpp
                   tests/Test_CompareWithinErrorIntervalsPolicy.cpp
                   tests/Test_ComparisonOperatorsUsingPolicy.cpp
                   tests/Test_ExactValueAndIgnoreErrorPolicy.cpp
                   tests/Test_ValueWithError.cpp
                   tests/Test_ValueWithError_Policy.cpp
                   tests/Test_ValueWithError_math_overloads.cpp
                   tests/Test_ValueWithError_valarray.cpp
                   tests/Test_ValueWithError_Eigen.cpp
                   tests/Test_ValueWithError_multiprecision.cpp)

SET(CPP_98_SOURCES ${COMMON_SOURCES}
                   src/cpp98/ValueWithError.hpp
                   tests/Test_ValueWithError_cpp98.cpp
                   tests/Test_ValueWithError_Policy_make_value_cpp98.cpp)

SET(CPP_11_SOURCES ${COMMON_SOURCES}
                   src/cpp11/ValueWithError.hpp
                   tests/Test_ValueWithError_cpp11.cpp
                   tests/Test_ValueWithError_math_overloads_cpp11.cpp
                   tests/Test_ValueWithError_Policy_cpp11.cpp
                   tests/Test_ValueWithError_Policy_make_value_cpp11.cpp)

if("${CPP11}" STREQUAL "YES")
  add_executable(${TEST_EXECUTABLE} ${CPP_11_SOURCES})
  include_directories(src/cpp11)
else()
  add_executable(${TEST_EXECUTABLE} ${CPP_98_SOURCES})
  include_directories(src/cpp98)
endif()

# LLVM support
set(LLVM_ROOT "" CACHE PATH "Root of LLVM install.")

if( EXISTS ${LLVM_ROOT}/include/llvm )
  # We incorporate the CMake features provided by LLVM:
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/share/llvm-3.3/cmake")
  include(LLVM-Config)

  # Now set the header and library paths:
  include_directories( ${LLVM_INCLUDE_DIRS} )
  link_directories( ${LLVM_LIBRARY_DIRS} )
  add_definitions( ${LLVM_DEFINITIONS} )

  llvm_map_components_to_libraries(REQ_LLVM_LIBRARIES native)
  target_link_libraries(${TEST_EXECUTABLE} ${REQ_LLVM_LIBRARIES})
endif()

if(UNIX)
  add_custom_target(cppcheck
    "${CMAKE_SOURCE_DIR}/${SYSTEM_LIBRARIES}/cppcheck/cppcheck" -j 6 --force --inconclusive --enable=all --std=c++11 --suppress="*:/usr*" --includes-file=include-paths.txt -I src/cpp11 -I ${BOOST_ROOT} ${CPP_11_SOURCES} 2> ${CMAKE_CURRENT_BINARY_DIR}/errors.txt
    DEPENDS ${TEST_EXECUTABLE} generateIncludePathList
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )

  add_custom_target(generateIncludePathList
    ${CMAKE_CXX_COMPILER} -print-search-dirs | grep "^libraries" | cut -f 2 -d '=' | tr ':' '\\n' > include-paths.txt
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
endif()

# handle compiler specifics
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

  if("${CPP11}" STREQUAL "YES")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  else()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98")
  endif()

  if("${COVERAGE}" STREQUAL "YES")
    # test code coverage
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -fprofile-arcs -ftest-coverage")
    target_link_libraries(${TEST_EXECUTABLE} "-lgcov")

    add_custom_target(coverage
      make check &&
      lcov --capture --directory . --output-file coverage.info &&
      lcov --remove coverage.info "/usr/*" -o coverage.info &&
      lcov --remove coverage.info "${CMAKE_SOURCE_DIR}/${SYSTEM_LIBRARIES}/*" -o coverage.info &&
      lcov --remove coverage.info "${CMAKE_SOURCE_DIR}/tests/*" -o coverage.info &&
      genhtml coverage.info --output-directory coverage
      DEPENDS check
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      VERBATIM
    )
  else()
    # default flags
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O3")
  endif()

  # precompiled header support
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
  include(pch)

  add_pch(pch tests/precompiled.hpp tests/precompiled.cpp)
  use_pch(${TEST_EXECUTABLE} pch)

  # realtime library
  if(!WIN32)
    target_link_libraries(${TEST_EXECUTABLE} rt)
  endif()

  if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    SET(CMAKE_CXX_FLAGS "-isystem ${Boost_INCLUDE_DIR} -pedantic -Wno-\\#pragma-messages -Werror=c++1y-extensions ${CMAKE_CXX_FLAGS}")
  endif()
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  # do nothing
endif()

target_link_libraries(${TEST_EXECUTABLE} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
                                         ${Boost_TIMER_LIBRARY}
                                         ${Boost_CHRONO_LIBRARY}
                                         ${Boost_SYSTEM_LIBRARY})

