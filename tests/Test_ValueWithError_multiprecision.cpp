#include "ValueWithError.hpp"
#include "precompiled.hpp"

#define TEST_COMPATIBLE_TYPE
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
#endif

#include <boost/test/unit_test.hpp>

using namespace error_propagation;

namespace {

  typedef boost::multiprecision::cpp_dec_float_100 MPH;
  typedef boost::multiprecision::cpp_dec_float_50  MPF;
  typedef ValueWithError<MPH> VD;
  typedef ValueWithError<MPF> VF;

  struct Fixture
  {
   VD pvd;
   VD pvf;
  };

} // anonymous namespace


BOOST_FIXTURE_TEST_SUITE(Test_ValueWithError_multiprecision,Fixture)

BOOST_AUTO_TEST_CASE(default_ctor)
{
  BOOST_CHECK_PV(VD(),MPH(),MPH());
  BOOST_CHECK_PV(VD(MPH(1.0)),MPH(1.0),MPH());
  BOOST_CHECK_PV(VD(MPH(1.0),MPH(2.0)),MPH(1.0),MPH(2.0));
}

BOOST_AUTO_TEST_CASE(assign)
{
  VD a, b;
  a = b;
  BOOST_CHECK_PV(a,b.GetValue(),b.GetError());
}

TEST_OP(times,*)
TEST_OP(plus,+)
TEST_OP(minus,-)
TEST_OP(div,/)

TEST_OP_COMPARE(equal,==)
TEST_OP_COMPARE(not_equal,!=)
TEST_OP_COMPARE(greater,>)
TEST_OP_COMPARE(greater_than,>=)
TEST_OP_COMPARE(smaller,<)
TEST_OP_COMPARE(smaller_than,<=)

TEST_MATH_ONE_ARG(abs)
TEST_MATH_ONE_ARG(acos)
TEST_MATH_ONE_ARG(asin)
TEST_MATH_ONE_ARG(atan)
TEST_MATH_ONE_ARG(cos)
TEST_MATH_ONE_ARG(cosh)
  // TEST_MATH_ONE_ARG(cbrt)
  // TEST_MATH_ONE_ARG(asinh)
  // TEST_MATH_ONE_ARG(acosh)
  // TEST_MATH_ONE_ARG(atanh)
  // TEST_MATH_ONE_ARG(erf)
  // TEST_MATH_ONE_ARG(erfc)
  // TEST_MATH_ONE_ARG(lgamma)
  // TEST_MATH_ONE_ARG(tgamma)
  // TEST_MATH_TWO_ARGS(hypot)
TEST_MATH_ONE_ARG(exp)
TEST_MATH_ONE_ARG(fabs)
TEST_MATH_ONE_ARG(log)
TEST_MATH_TWO_ARGS(pow)
TEST_MATH_ONE_ARG(sin)
TEST_MATH_ONE_ARG(sinh)
TEST_MATH_ONE_ARG(sqrt)
TEST_MATH_ONE_ARG(tan)
TEST_MATH_ONE_ARG(tanh)

BOOST_AUTO_TEST_SUITE_END() // Test_ValueWithError_multiprecision

#ifdef __clang__
#pragma clang diagnostic pop
#endif // __clang__

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic pop
#endif

