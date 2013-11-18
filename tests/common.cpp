#include "precompiled.hpp"

extern const double D_NAN = std::numeric_limits<double>::quiet_NaN();
extern const float  F_NAN = std::numeric_limits<float>::quiet_NaN();

#define BOOST_TEST_MODULE "C++ Unit Tests for ValueWithError"

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#endif

#include <boost/test/unit_test.hpp>

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic pop
#endif

