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
#endif

#include <boost/test/unit_test.hpp>

using namespace error_propagation;

namespace std
{
  template<typename charT, typename traits, typename T>
  basic_ostream<charT,traits>&
  operator<<(basic_ostream<charT,traits>& out, const valarray<T>& arr)
  {
    for(unsigned int i = 0; i != arr.size(); i++)
      out << arr[i] << " ";

    out << endl;

    return out;
  }
}

namespace {

#define BOOST_CHECK_EQUAL_PV_VAL(A, B, C) \
  BOOST_CHECK_PREDICATE( valarray_comp, (A.GetValue())(B) ); \
  BOOST_CHECK_PREDICATE( valarray_comp, (A.GetError())(C) );

  /// Compares two valarray objects and determines if they are equal or not
  ///
  /// Required as operator== of std::valarray returns std::valarray<bool>
  bool valarray_comp( std::valarray<double> const& v1, std::valarray<double> const& v2)
  {
    std::valarray<bool> cmp = ( v1 == v2 );
    bool equal = true;
    for(unsigned int i = 0; i < cmp.size() && equal; i++)
      equal &= cmp[i];

    return equal;
  }

  typedef std::valarray<double> VAD;
  typedef ValueWithError<VAD> VD;

  struct Fixture
  {
   VD pvd;
  };

} // anonymous namespace

namespace boost { namespace math { namespace tools {
  template <> struct promote_args_2<std::valarray<double>, double> {  typedef std::valarray<double> type; };
  template <> struct promote_args_2<double, std::valarray<double> > {  typedef std::valarray<double> type; };
}}}

BOOST_FIXTURE_TEST_SUITE(Test_ValueWithError_Valarray,Fixture)

  BOOST_AUTO_TEST_CASE(default_ctor)
  {
    const VAD val;
    const VAD vd1(1.0,1);
    const VAD vd2(2.0,1);
    BOOST_CHECK_EQUAL_PV_VAL(VD(),val,val);
    BOOST_CHECK_EQUAL_PV_VAL(VD(vd1),vd1,val);
    BOOST_CHECK_EQUAL_PV_VAL(VD(vd1,vd2),vd1,vd2);
  }

  BOOST_AUTO_TEST_CASE(assign)
  {
    VD a, b;
    a = b;
    BOOST_CHECK_EQUAL_PV_VAL(a,b.GetValue(),b.GetError());
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
TEST_MATH_ONE_ARG(atan)
TEST_MATH_ONE_ARG(cos)
TEST_MATH_ONE_ARG(cosh)
TEST_MATH_ONE_ARG(exp)
TEST_MATH_ONE_ARG(log)
TEST_MATH_TWO_ARGS(pow)
TEST_MATH_ONE_ARG(sin)
TEST_MATH_ONE_ARG(sinh)
TEST_MATH_ONE_ARG(sqrt)
TEST_MATH_ONE_ARG(tan)
TEST_MATH_ONE_ARG(tanh)

BOOST_AUTO_TEST_SUITE_END() // Test_ValueWithError_Valarray

#ifdef __clang__
#pragma clang diagnostic pop
#endif // __clang__

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic pop
#endif

