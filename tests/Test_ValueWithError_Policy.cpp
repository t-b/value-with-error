#include "ValueWithError.hpp"
#include "precompiled.hpp"
#include "common.hpp"
#include "PolicyMock.hpp"

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

  typedef ValueWithError<double,PolicyMock> VD;
  typedef ValueWithError<float,PolicyMock>  PVF;

  class Fixture
  {
  public:
    Fixture()
    :
    pvd(D_NAN,D_NAN),
    pvf(F_NAN,F_NAN)
    {}

   VD pvd;
   PVF pvf;
  };

} // anonymous namespace

// Here we check that the same Policy is returned as the input PV had
// The *compilation* is already the verification
// FIXME should better test that different policies are not allowed, but that means we must check for a snippet to not compile
BOOST_FIXTURE_TEST_SUITE(Test_ValueWithError_Policy, Fixture)

BOOST_AUTO_TEST_CASE(ctor)
{
 VD a(1.0,1.0);
 VD b(1.0);
 VD c;
 BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(assign_op)
{
 VD a, b, c;
 a = pvd;
 b = pvf;
 c = 1.0;
 BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(copy_ctor)
{
 VD a(pvd);
 VD b(pvf);
 BOOST_CHECK(true);
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

TEST_MATH_ONE_ARG(exp)
TEST_MATH_ONE_ARG(fabs)
TEST_MATH_ONE_ARG(log)
TEST_MATH_TWO_ARGS(pow)
TEST_MATH_ONE_ARG(sin)
TEST_MATH_ONE_ARG(sinh)
TEST_MATH_ONE_ARG(sqrt)
TEST_MATH_ONE_ARG(tan)
TEST_MATH_ONE_ARG(tanh)

BOOST_AUTO_TEST_SUITE_END() // Test_ValueWithError_Policy

#ifdef __clang__
#pragma clang diagnostic pop
#endif // __clang__

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic pop
#endif

