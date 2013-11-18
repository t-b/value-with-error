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
#endif

#include <boost/test/unit_test.hpp>
#include "common.hpp"

using namespace error_propagation;

namespace {

  class PolicyMock
  {
  public:
    static unsigned int calledFunction;

    enum Functions
    {
      Invalid     = 0,
      Equal_pv_pv,
      Equal_pv_val,
      Equal_val_pv,
      GreaterThan_pv_pv,
      GreaterThan_pv_val,
      GreaterThan_val_pv,
      GreaterOrEqual_pv_pv,
      GreaterOrEqual_pv_val,
      GreaterOrEqual_val_pv
    };

    template<typename U, typename V, typename P>
    static bool Equal(const ValueWithError<U,P> &lhs, const ValueWithError<V,P> &rhs) { calledFunction = Equal_pv_pv; return true; }
    template<typename U, typename V, typename P>
    static bool Equal(const ValueWithError<U,P> &lhs, const V &rhs){ calledFunction = Equal_pv_val; return true; }
    template<typename U, typename V, typename P>
    static bool Equal(const U &lhs, const ValueWithError<V,P> &rhs){ calledFunction = Equal_val_pv; return true; }
    //
    template<typename U, typename V, typename P>
    static bool GreaterThan(const ValueWithError<U,P> &lhs, const ValueWithError<V,P> &rhs){ calledFunction = GreaterThan_pv_pv; return true; }
    template<typename U, typename V, typename P>
    static bool GreaterThan(const ValueWithError<U,P> &lhs, const V &rhs){ calledFunction = GreaterThan_pv_val; return true; }
    template<typename U, typename V, typename P>
    static bool GreaterThan(const U &lhs, const ValueWithError<V,P> &rhs){ calledFunction = GreaterThan_val_pv; return true; }
    //
    template<typename U, typename V, typename P>
    static bool GreaterOrEqual(const ValueWithError<U,P> &lhs, const ValueWithError<V,P> &rhs){ calledFunction = GreaterOrEqual_pv_pv; return true; }
    template<typename U, typename V, typename P>
    static bool GreaterOrEqual(const ValueWithError<U,P> &lhs, const V &rhs){ calledFunction = GreaterOrEqual_pv_val; return true; }
    template<typename U, typename V, typename P>
    static bool GreaterOrEqual(const U &lhs, const ValueWithError<V,P> &rhs){ calledFunction = GreaterOrEqual_val_pv; return true; }
    //
  private:
    PolicyMock();
    ~PolicyMock();
  };

  unsigned int PolicyMock::calledFunction = PolicyMock::Invalid;

  class TestPolicyFixture
  {
  public:

    TestPolicyFixture()
    :
    pv1(D_NAN,D_NAN),
    pv2(D_NAN,D_NAN)
    {}

   ValueWithError<double,PolicyMock> pv1, pv2;
  };

// execute predicate and do nothing
#define BOOST_EXECUTE(A) { if(A){} }

} // anonymous namespace

BOOST_AUTO_TEST_SUITE(TestComparisonOperatorsUsingPolicy)

BOOST_FIXTURE_TEST_CASE(operator_equal_pv_pv,TestPolicyFixture)
{
  BOOST_EXECUTE(pv1 == pv2);
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::Equal_pv_pv);
}

BOOST_FIXTURE_TEST_CASE(operator_equal_pv_val,TestPolicyFixture)
{
  BOOST_EXECUTE(pv1 == pv2.GetValue());
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::Equal_pv_val);
}

BOOST_FIXTURE_TEST_CASE(operator_equal_val_pv,TestPolicyFixture)
{
  BOOST_EXECUTE(pv2.GetValue() == pv1);
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::Equal_val_pv);
}

// operator!= is implemented in terms of ==
BOOST_FIXTURE_TEST_CASE(operator_notequal_pv_pv,TestPolicyFixture)
{
  BOOST_EXECUTE(pv1 != pv2);
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::Equal_pv_pv);
}

BOOST_FIXTURE_TEST_CASE(operator_notequal_pv_val,TestPolicyFixture)
{
  BOOST_EXECUTE(pv1 != pv2.GetValue());
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::Equal_pv_val);
}

BOOST_FIXTURE_TEST_CASE(operator_notequal_val_pv,TestPolicyFixture)
{
  BOOST_EXECUTE(pv2.GetValue() != pv1);
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::Equal_val_pv);
}

// operator< is implemented in terms of >, so we have to switch the parameters order
BOOST_FIXTURE_TEST_CASE(operator_lowerThan_pv_pv,TestPolicyFixture)
{
  BOOST_EXECUTE(pv2 < pv1);
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::GreaterThan_pv_pv);
}

BOOST_FIXTURE_TEST_CASE(operator_lowerThan_pv_val,TestPolicyFixture)
{
  BOOST_EXECUTE(pv2 < pv1.GetValue());
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::GreaterThan_val_pv);
}

BOOST_FIXTURE_TEST_CASE(operator_lowerThan_val_pv,TestPolicyFixture)
{
  BOOST_EXECUTE(pv2.GetValue() < pv1);
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::GreaterThan_pv_val);
}

// operator>
BOOST_FIXTURE_TEST_CASE(operator_greaterThan_pv_pv,TestPolicyFixture)
{
  BOOST_EXECUTE(pv2 > pv1);
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::GreaterThan_pv_pv);
}

BOOST_FIXTURE_TEST_CASE(operator_greaterThan_pv_val,TestPolicyFixture)
{
  BOOST_EXECUTE(pv2 > pv1.GetValue());
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::GreaterThan_pv_val);
}

BOOST_FIXTURE_TEST_CASE(operator_greaterThan_val_pv,TestPolicyFixture)
{
  BOOST_EXECUTE(pv2.GetValue() > pv1);
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::GreaterThan_val_pv);
}

// operator>=
BOOST_FIXTURE_TEST_CASE(operator_greaterOrEqual_pv_pv,TestPolicyFixture)
{
  BOOST_EXECUTE(pv2 >= pv1);
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::GreaterOrEqual_pv_pv);
}

BOOST_FIXTURE_TEST_CASE(operator_greaterOrEqual_pv_val,TestPolicyFixture)
{
  BOOST_EXECUTE(pv2 >= pv1.GetValue());
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::GreaterOrEqual_pv_val);
}

BOOST_FIXTURE_TEST_CASE(operator_greaterOrEqual_val_pv,TestPolicyFixture)
{
  BOOST_EXECUTE(pv2.GetValue() >= pv1);
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::GreaterOrEqual_val_pv);
}

// operator<= is implemented in terms of >=, so we have to switch the parameters order
BOOST_FIXTURE_TEST_CASE(operator_lowerOrEqual_pv_pv,TestPolicyFixture)
{
  BOOST_EXECUTE(pv2 <= pv1);
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::GreaterOrEqual_pv_pv);
}

BOOST_FIXTURE_TEST_CASE(operator_lowerOrEqual_pv_val,TestPolicyFixture)
{
  BOOST_EXECUTE(pv2 <= pv1.GetValue());
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::GreaterOrEqual_val_pv);
}

BOOST_FIXTURE_TEST_CASE(operator_lowerOrEqual_val_pv,TestPolicyFixture)
{
  BOOST_EXECUTE(pv2.GetValue() <= pv1);
  BOOST_CHECK_EQUAL(PolicyMock::calledFunction,PolicyMock::GreaterOrEqual_pv_val);
}

BOOST_AUTO_TEST_SUITE_END() // TestComparisonOperatorsUsingPolicy

#ifdef __clang__
#pragma clang diagnostic pop
#endif // __clang__

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic pop
#endif

