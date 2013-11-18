#include "ValueWithError.hpp"
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
#endif // __GNUC__

#include <boost/test/unit_test.hpp>

using namespace error_propagation;

namespace {

  typedef ValueWithError<double> VD;

  struct Fixture
  {
    Fixture()
      :
       val1(1.0),
       val2(2.0),
       error(2.0),
       pv1(val1,error),
       pv2(val2,error)
    {}

    const double val1, val2, error;
    const VD pv1, pv2;
  };

} // anonymous namespace

BOOST_FIXTURE_TEST_SUITE(Test_ExactValueAndIgnoreErrorPolicy,Fixture)

BOOST_AUTO_TEST_CASE(operator_equal_pv_pv)
{
  BOOST_CHECK(ExactValueAndIgnoreErrorPolicy::Equal(pv1,pv1));
  BOOST_CHECK(!ExactValueAndIgnoreErrorPolicy::Equal(pv1,pv2));
}

BOOST_AUTO_TEST_CASE(operator_equal_pv_val)
{
  BOOST_CHECK(ExactValueAndIgnoreErrorPolicy::Equal(pv1,pv1.GetValue()));
  BOOST_CHECK(!ExactValueAndIgnoreErrorPolicy::Equal(pv1,pv2.GetValue()));
}

BOOST_AUTO_TEST_CASE(operator_equal_val_pv)
{
  BOOST_CHECK(ExactValueAndIgnoreErrorPolicy::Equal(pv1.GetValue(),pv1));
  BOOST_CHECK(!ExactValueAndIgnoreErrorPolicy::Equal(pv1.GetValue(),pv2));
}

BOOST_AUTO_TEST_CASE(operator_greaterThan_pv_pv)
{
  BOOST_CHECK(ExactValueAndIgnoreErrorPolicy::GreaterThan(pv2,pv1));
  BOOST_CHECK(!ExactValueAndIgnoreErrorPolicy::GreaterThan(pv1,pv2));
}

BOOST_AUTO_TEST_CASE(operator_greaterThan_pv_val)
{
  BOOST_CHECK(ExactValueAndIgnoreErrorPolicy::GreaterThan(pv2,pv1.GetValue()));
  BOOST_CHECK(!ExactValueAndIgnoreErrorPolicy::GreaterThan(pv1,pv2.GetValue()));
}

BOOST_AUTO_TEST_CASE(operator_greaterThan_val_pv)
{
  BOOST_CHECK(ExactValueAndIgnoreErrorPolicy::GreaterThan(pv2.GetValue(),pv1));
  BOOST_CHECK(!ExactValueAndIgnoreErrorPolicy::GreaterThan(pv1.GetValue(),pv2));
}

BOOST_AUTO_TEST_CASE(operator_greaterOrEqual_pv_pv)
{
  BOOST_CHECK(ExactValueAndIgnoreErrorPolicy::GreaterOrEqual(pv1,pv1));
  BOOST_CHECK(ExactValueAndIgnoreErrorPolicy::GreaterOrEqual(pv2,pv1));
  BOOST_CHECK(!ExactValueAndIgnoreErrorPolicy::GreaterOrEqual(pv1,pv2));
}

BOOST_AUTO_TEST_CASE(operator_greaterOrEqual_pv_val)
{
  BOOST_CHECK(ExactValueAndIgnoreErrorPolicy::GreaterOrEqual(pv1,pv1.GetValue()));
  BOOST_CHECK(ExactValueAndIgnoreErrorPolicy::GreaterOrEqual(pv2,pv1.GetValue()));
  BOOST_CHECK(!ExactValueAndIgnoreErrorPolicy::GreaterOrEqual(pv1,pv2.GetValue()));
}

BOOST_AUTO_TEST_CASE(operator_greaterOrEqual_val_pv)
{
  BOOST_CHECK(ExactValueAndIgnoreErrorPolicy::GreaterOrEqual(pv1,pv1.GetValue()));
  BOOST_CHECK(ExactValueAndIgnoreErrorPolicy::GreaterOrEqual(pv2.GetValue(),pv1));
  BOOST_CHECK(!ExactValueAndIgnoreErrorPolicy::GreaterOrEqual(pv1.GetValue(),pv2));
}

BOOST_AUTO_TEST_SUITE_END() // Test_ExactValueAndIgnoreErrorPolicy

#ifdef __clang__
#pragma clang diagnostic pop
#endif // __clang__

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic pop
#endif

