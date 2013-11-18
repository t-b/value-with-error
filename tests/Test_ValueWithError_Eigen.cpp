#include "ValueWithError.hpp"
#include "precompiled.hpp"
#include "common.hpp"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif // __clang__

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif

#include <Eigen/Eigen>
#include <boost/test/unit_test.hpp>

using namespace error_propagation;

namespace {

  /// Compares two Eigen objects and determines if they are equal or not
  struct eigen_comp
  {
    template<typename Derived1, typename Derived2>
    bool operator()( Derived1 const& v1, Derived2 const& v2) const
    {
      if( v1.size() != v2.size())
        return false;

      bool equal = true;
      for(typename Derived1::Index i = 0; i < v1.size() && equal; i++)
          equal &= ( v1(i) == v2(i) );

      return equal;
    }
  };

#define BOOST_CHECK_EQUAL_PV_EIGEN(A, B, C) \
  BOOST_CHECK_PREDICATE( eigen_comp(), (A.GetValue())(B) ); \
  BOOST_CHECK_PREDICATE( eigen_comp(), (A.GetError())(C) );

  struct ArrayFixture
  {
    typedef Eigen::Array3d     EA;
    typedef ValueWithError<EA> VD;

    VD pvd;
  };

}// anonymous namespace

BOOST_FIXTURE_TEST_SUITE(Test_ValueWithError_Eigen_Array,ArrayFixture)

BOOST_AUTO_TEST_CASE(ctor)
{
  EA val(1,2,3), error(-4,-5,-6);
  VD b(val);
  VD c(val, error);
  BOOST_CHECK_EQUAL_PV_EIGEN(c, val, error.abs());
}

TEST_OP(times,*)
TEST_OP(plus,+)
TEST_OP(minus,-)
TEST_OP(div,/)

// Comparison operators don't return bool so they are not usable for our purpose
// TEST_OP_COMPARE(equal,==)
// TEST_OP_COMPARE(not_equal,!=)
// TEST_OP_COMPARE(greater,>)
// TEST_OP_COMPARE(greater_than,>=)
// TEST_OP_COMPARE(smaller,<)
// TEST_OP_COMPARE(smaller_than,<=)

TEST_MATH_ONE_ARG(abs)
TEST_MATH_ONE_ARG(acos)
TEST_MATH_ONE_ARG(asin)
// TEST_MATH_ONE_ARG(atan)
TEST_MATH_ONE_ARG(cos)
// TEST_MATH_ONE_ARG(cosh)
//   TEST_MATH_ONE_ARG(cbrt)
//   TEST_MATH_ONE_ARG(asinh)
//   TEST_MATH_ONE_ARG(acosh)
//   TEST_MATH_ONE_ARG(atanh)
//   TEST_MATH_ONE_ARG(erf)
//   TEST_MATH_ONE_ARG(erfc)
//   TEST_MATH_ONE_ARG(lgamma)
//   TEST_MATH_ONE_ARG(tgamma)
//   TEST_MATH_TWO_ARGS(hypot)
TEST_MATH_ONE_ARG(exp)
// TEST_MATH_ONE_ARG(fabs)
TEST_MATH_ONE_ARG(log)

// base^array is not defined so we can't use TEST_MATH_TWO_ARGS(pow)

BOOST_AUTO_TEST_CASE(_pow)
{
  VD a;
  a = pow(pvd,pvd);
  a = pow(pvd,1.0);
  BOOST_CHECK(true);
}

TEST_MATH_ONE_ARG(sin)
// TEST_MATH_ONE_ARG(sinh)
TEST_MATH_ONE_ARG(sqrt)
TEST_MATH_ONE_ARG(tan)
// TEST_MATH_ONE_ARG(tanh)

BOOST_AUTO_TEST_SUITE_END() // Test_ValueWithError_Eigen_Array

#ifdef __clang__
#pragma clang diagnostic pop
#endif // __clang__

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic pop
#endif

