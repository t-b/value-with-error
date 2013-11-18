#include "precompiled.hpp"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif // __clang__

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#endif

#include <boost/test/unit_test.hpp>

namespace error_propagation { class PolicyMock; }
#define DEFAULT_POLICY_CLASS error_propagation::PolicyMock
#include "ValueWithError.hpp"
#include "PolicyMock.hpp"

using namespace error_propagation;

namespace {

  typedef ValueWithError<double,PolicyMock> VD;
  typedef ValueWithError<float,PolicyMock>  VF;

} // anonymous namespace

BOOST_AUTO_TEST_SUITE(Test_ValueWithError_Policy)

BOOST_AUTO_TEST_CASE(make_val)
{
 VD a;
 a = make_value(1.0, 1.0);
 BOOST_CHECK(true);
}

BOOST_AUTO_TEST_SUITE_END() // Test_ValueWithError_Policy

#ifdef __clang__
#pragma clang diagnostic pop
#endif // __clang__

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic pop
#endif

