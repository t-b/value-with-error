#include "ValueWithError.hpp"
#include "precompiled.hpp"
#include "common.hpp"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif // __clang__

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#endif // __GNUC__

#include <boost/test/unit_test.hpp>

using namespace error_propagation;

namespace {

  typedef ValueWithError<double,CompareWithinErrorIntervalsPolicy> VD;
  typedef ValueWithError<float,CompareWithinErrorIntervalsPolicy> PVF;

  struct CompareWithinErrorFixture{ };

} // anonymous namespace

BOOST_FIXTURE_TEST_SUITE(Test_CompareWithinErrorIntervalsPolicy,CompareWithinErrorFixture)

BOOST_AUTO_TEST_CASE(operator_equal)
{
  // no overlap
  BOOST_CHECK(!CompareWithinErrorIntervalsPolicy::Equal(VD(1.0,0.4),VD(2.0,0.4)));
  // point overlap
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::Equal(VD(1.0,1.0), VD(2.0,0.4)));
  // range overlap
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::Equal(VD(1.0,1.3), VD(2.0,0.4)));

  // test overloads
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::Equal(VD(1.0), 1.0));
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::Equal(VD(1.0,0.4), 1.0));
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::Equal(1.0, VD(1.0,0.4)));
}
// not equal defined in terms of equal therefore skipping for now

BOOST_AUTO_TEST_CASE(operator_greaterOrEqualThan)
{
  // no overlap
  BOOST_CHECK(!CompareWithinErrorIntervalsPolicy::GreaterOrEqual(VD(1.0,0.4), VD(2.0,0.4)));
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::GreaterOrEqual(VD(2.0,0.4), VD(1.0,0.4)));
  // point overlap
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::GreaterOrEqual(VD(1.0,1.0), VD(2.0,0.4)));
  // range overlap
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::GreaterOrEqual(VD(1.0,1.3), VD(2.0,0.4)));

  // test overloads
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::GreaterOrEqual(VD(1.0), 1.0));
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::GreaterOrEqual(VD(2.0), 1.0));
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::GreaterOrEqual(VD(1.0,0.4), 1.0));
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::GreaterOrEqual(VD(2.0,0.4), 1.0));
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::GreaterOrEqual(1.0, VD(1.0,0.4)));
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::GreaterOrEqual(2.0, VD(1.0,0.4)));
}

BOOST_AUTO_TEST_CASE(operator_greaterThan)
{
  // no overlap
  BOOST_CHECK(!CompareWithinErrorIntervalsPolicy::GreaterThan(VD(1.0,0.4), VD(2.0,0.4)));
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::GreaterThan(VD(2.0,0.4), VD(1.0,0.4)));
  // point overlap
  BOOST_CHECK(!CompareWithinErrorIntervalsPolicy::GreaterThan(VD(1.0,1.0), VD(2.0,0.4)));
  // range overlap
  BOOST_CHECK(!CompareWithinErrorIntervalsPolicy::GreaterThan(VD(1.0,1.3), VD(2.0,0.4)));

  // test overloads
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::GreaterThan(VD(2.0), 1.0));
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::GreaterThan(VD(2.0,0.4), 1.0));
  BOOST_CHECK(CompareWithinErrorIntervalsPolicy::GreaterThan(2.0, VD(1.0,0.4)));
}

BOOST_AUTO_TEST_SUITE_END() // Test_CompareWithinErrorIntervalsPolicy

#ifdef __clang__
#pragma clang diagnostic pop
#endif // __clang__

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic pop
#endif // __GNUC__

