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

template<typename T>
T GetInstance()
{
  return T();
}

BOOST_AUTO_TEST_CASE(move_ctor)
{
 VD a(GetInstance<VD>());
 VD b(GetInstance<VD>());
 BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(move_assign)
{
 VD a, b;
 a = GetInstance<VD>();
 b = GetInstance<VD>();
 BOOST_CHECK(true);
}

// make_value tests are defined in Test_ValueWithError_Policy_make_value.cpp
// as we need to tweak DEFAULT_POLICY_CLASS for that

TEST_MATH_ONE_ARG(cbrt)
TEST_MATH_ONE_ARG(asinh)
TEST_MATH_ONE_ARG(acosh)
TEST_MATH_ONE_ARG(atanh)
TEST_MATH_ONE_ARG(erf)
TEST_MATH_ONE_ARG(erfc)
TEST_MATH_ONE_ARG(lgamma)
TEST_MATH_ONE_ARG(tgamma)
TEST_MATH_TWO_ARGS(hypot)

BOOST_AUTO_TEST_SUITE_END() // Test_ValueWithError_Policy

#ifdef __clang__
#pragma clang diagnostic pop
#endif // __clang__

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic pop
#endif

