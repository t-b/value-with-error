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

namespace {

  typedef ValueWithError<double> VD;
  typedef ValueWithError<float>  PVF;

  struct Fixture
  {
    Fixture()
      :
       val(1.0),
       error(2.0),
       physVal1(val,error),
       physVal2(val,error),
       physValFloat((float)val,(float)error)
    {}

    VD GetValueWithError() { return physVal1; }
    double GetValue() { return val; }

    const double val, error;
    const VD physVal1, physVal2, physValDefault;
    const PVF physValFloat;
  };

} // anonymous namespace

BOOST_FIXTURE_TEST_SUITE(Test_ValueWithError_Basics,Fixture)

BOOST_AUTO_TEST_SUITE(_make_value)
// policy tested in Test_ValueWithError_Policy_make_value.cpp

BOOST_AUTO_TEST_CASE(values)
{
  VD pv = make_value(val,error);
  BOOST_CHECK_PV(pv,val, error);
}

BOOST_AUTO_TEST_SUITE_END() // _make_value

BOOST_AUTO_TEST_SUITE_END() // Test_ValueWithError

#ifdef __clang__
#pragma clang diagnostic pop
#endif // __clang__

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic pop
#endif

