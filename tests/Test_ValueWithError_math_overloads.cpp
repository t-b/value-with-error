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

#include <iostream>

using namespace error_propagation;

namespace {

  typedef ValueWithError<double> VD;
  using boost::math::pow;

  const double D_EPS             = std::numeric_limits<double>::epsilon();
  const double D_LARGE_EPS       = 1000.0 * std::numeric_limits<double>::epsilon();

  const double half_pi           = boost::math::constants::half_pi<double>();
  const double pi                = boost::math::constants::pi<double>();
  const double e                 = boost::math::constants::e<double>();
  const double ln_two            = boost::math::constants::ln_two<double>();
  const double ln_ten            = boost::math::constants::ln_ten<double>();
  const double one_div_root_two  = boost::math::constants::one_div_root_two<double>();
  const double root_two          = boost::math::constants::root_two<double>();
  const double three_quarters_pi = boost::math::constants::three_quarters_pi<double>();
  const double quarter_pi        = boost::math::constants::pi<double>() / 4.0;
  const double one_div_root_pi   = boost::math::constants::one_div_root_pi<double>();
  const double two_div_root_pi   = boost::math::constants::one_div_root_pi<double>() * 2.0;
  const double euler             = boost::math::constants::euler<double>();

  struct Fixture
  {
    Fixture()
      :
      pvMinus(-2.0,4.0),
      pvPlus(2.0,4.0)
    {}

    const VD pvMinus, pvPlus, pvZero;
  };

} // anonymous namespace

BOOST_FIXTURE_TEST_SUITE(Test_ValueWithError_MathOverloads,Fixture)

BOOST_AUTO_TEST_SUITE(_abs)

BOOST_AUTO_TEST_CASE(positive)
{
  VD result = abs(pvPlus);
  BOOST_CHECK_EQUAL(result.GetValue(),pvPlus.GetValue());
  BOOST_CHECK_EQUAL(result.GetError(),pvPlus.GetError());
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD result = abs(pvZero);
  BOOST_CHECK_EQUAL(result.GetValue(),pvZero.GetValue());
  BOOST_CHECK_EQUAL(result.GetError(),pvZero.GetError());
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD result = abs(pvMinus);
  BOOST_CHECK_EQUAL(result.GetValue(),std::abs(pvMinus.GetValue()));
  BOOST_CHECK_EQUAL(result.GetError(),pvMinus.GetError());
}

BOOST_AUTO_TEST_SUITE_END() // _abs

BOOST_AUTO_TEST_SUITE(_fabs)

BOOST_AUTO_TEST_CASE(positive)
{
  VD result = fabs(pvPlus);
  BOOST_CHECK_EQUAL(result.GetValue(),pvPlus.GetValue());
  BOOST_CHECK_EQUAL(result.GetError(),pvPlus.GetError());
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD result = fabs(pvZero);
  BOOST_CHECK_EQUAL(result.GetValue(),pvZero.GetValue());
  BOOST_CHECK_EQUAL(result.GetError(),pvZero.GetError());
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD result = fabs(pvMinus);
  BOOST_CHECK_EQUAL(result.GetValue(),std::abs(pvMinus.GetValue()));
  BOOST_CHECK_EQUAL(result.GetError(),pvMinus.GetError());
}
BOOST_AUTO_TEST_SUITE_END() // _fabs

BOOST_AUTO_TEST_SUITE(_sin)

BOOST_AUTO_TEST_CASE(positive)
{
  VD pv(half_pi,1.0);
  VD result = sin(pv);
  BOOST_CHECK_CLOSE(result.GetValue(),1.0,D_EPS);
  BOOST_CHECK_SMALL(result.GetError(),D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD pv(0.0,1.0);
  VD result = sin(pv);
  BOOST_CHECK_SMALL(result.GetValue(),D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),1.0,D_EPS);
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD pv(-1.0 * half_pi,1.0);
  VD result = sin(pv);
  BOOST_CHECK_CLOSE(result.GetValue(),-1.0,D_EPS);
  BOOST_CHECK_SMALL(result.GetError(),D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _sin

BOOST_AUTO_TEST_SUITE(_cos)

BOOST_AUTO_TEST_CASE(positive)
{
// using namespace boost::test_tools;
// fraction_tolerance_t<double> tol(D_EPS);
// close_at_tolerance<double> p(tol,FPC_WEAK);
  VD pv(half_pi,1.0);
  VD result = cos(pv);
  BOOST_CHECK_SMALL(result.GetValue(),D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),pv.GetError(),D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD pv(0.0,1.0);
  VD result = cos(pv);
  BOOST_CHECK_CLOSE(result.GetValue(),1.0,D_EPS);
  BOOST_CHECK_SMALL(result.GetError(),D_EPS);
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD pv(-1.0 * half_pi,1.0);
  VD result = cos(pv);
  BOOST_CHECK_SMALL(result.GetValue(),D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),pv.GetError(),D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _cos

BOOST_AUTO_TEST_SUITE(_exp)

BOOST_AUTO_TEST_CASE(positive)
{
  VD pv(1.0,2.0);
  VD result = exp(pv);
  BOOST_CHECK_CLOSE(result.GetValue(),e,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),2.0 * e,D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD pv(0.0,2.0);
  VD result = exp(pv);
  BOOST_CHECK_CLOSE(result.GetValue(),1.0,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),2.0,D_EPS);
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD pv(-1.0,2.0);
  VD result = exp(pv);
  BOOST_CHECK_CLOSE(result.GetValue(),1.0 / e,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),2.0 / e,D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _exp

BOOST_AUTO_TEST_SUITE(_pow_basePV_expDOUBLE)

BOOST_AUTO_TEST_CASE(positive)
{
  VD base(2.0,2.0);
  double exponent = 3.0;
  VD result = pow(base, exponent);
  BOOST_CHECK_CLOSE(result.GetValue(),8.0,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),3.0 * 4.0 * 2.0,D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD base(0.0,2.0);
  double exponent = 3.0;
  VD result = pow(base, exponent);
  BOOST_CHECK_CLOSE(result.GetValue(),0.0,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),0.0,D_EPS);
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD base(-2.0,2.0);
  double exponent = 3.0;
  VD result = pow(base, exponent);
  BOOST_CHECK_CLOSE(result.GetValue(),-8.0,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),3.0 * 4.0 * 2.0,D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _pow_basePV_expDOUBLE

BOOST_AUTO_TEST_SUITE(_pow_baseDOUBLE_expPV)

BOOST_AUTO_TEST_CASE(positive)
{
  double base = 2.0;
  VD exponent(3.0,2.0);
  VD result = pow(base, exponent);
  BOOST_CHECK_CLOSE(result.GetValue(),8.0,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), ln_two * 8.0 * 2.0, D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  double base = 0.0;
  VD exponent(3.0,2.0);
  VD result = pow(base, exponent);
  BOOST_CHECK_CLOSE(result.GetValue(),0.0,D_EPS);
  BOOST_CHECK(boost::math::isnan(result.GetError()));
}

BOOST_AUTO_TEST_CASE(negative)
{
  double base = -2.0;
  VD exponent(3.0,2.0);
  VD result = pow(base, exponent);
  BOOST_CHECK_CLOSE(result.GetValue(), -8.0, D_EPS);
  BOOST_CHECK(boost::math::isnan(result.GetError()));
}

BOOST_AUTO_TEST_SUITE_END() // _pow_baseDOUBLE_expPV

BOOST_AUTO_TEST_SUITE(_pow_basePV_expPV)

BOOST_AUTO_TEST_CASE(both_positive)
{
  VD base(2.0,1.5);
  VD exponent(3.0,2.0);
  VD result = pow(base, exponent);
  BOOST_CHECK_CLOSE(result.GetValue(),8.0,D_EPS);
  double error = detail::hypot(3.0 * 4.0 * 1.5, ln_two * 8.0 * 2.0);
  BOOST_CHECK_CLOSE(result.GetError(),error, D_EPS);
}

BOOST_AUTO_TEST_CASE(both_negative)
{
  VD base(-2.0,1.5);
  VD exponent(-3.0,2.0);
  VD result = pow(base, exponent);
  BOOST_CHECK_CLOSE(result.GetValue(),-1.0/8.0,D_EPS);
  BOOST_CHECK(boost::math::isnan(result.GetError()));
}

BOOST_AUTO_TEST_CASE(base_zero_exponent_positive)
{
  VD base(0.0,1.5);
  VD exponent(3.0,2.0);
  VD result = pow(base, exponent);
  BOOST_CHECK_CLOSE(result.GetValue(),0.0,D_EPS);
  BOOST_CHECK(boost::math::isnan(result.GetError()));
}

BOOST_AUTO_TEST_CASE(base_zero_exponent_zero)
{
  VD base(0.0,1.5);
  VD exponent(0.0,2.0);
  VD result = pow(base, exponent);
  BOOST_CHECK_CLOSE(result.GetValue(),1.0,D_EPS);
  BOOST_CHECK(boost::math::isinf(result.GetError()));
}

BOOST_AUTO_TEST_CASE(base_zero_exponent_negative)
{
  VD base(0.0,1.5);
  VD exponent(-2.5,2.0);
  VD result = pow(base, exponent);
  BOOST_CHECK(boost::math::isinf(result.GetValue()));
  BOOST_CHECK(boost::math::isinf(result.GetError()));
}

BOOST_AUTO_TEST_CASE(base_positive_exponent_zero)
{
  VD base(2.0,1.5);
  VD exponent(0.0,3.0);
  VD result = pow(base, exponent);
  BOOST_CHECK_CLOSE(result.GetValue(),1.0,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), ln_two * 1.0 * 3.0, D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _pow_baseDOUBLE_expPV

BOOST_AUTO_TEST_SUITE(_sqrt)

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(4.0,2.0);
  VD result = sqrt(val);
  BOOST_CHECK_CLOSE(result.GetValue(),2.0,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 0.5 / 2.0 * 2.0, D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val(0.0,2.0);
  VD result = sqrt(val);
  BOOST_CHECK_CLOSE(result.GetValue(),0.0,D_EPS);
  BOOST_CHECK(boost::math::isinf(result.GetError()));
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val(-1.0,1.0);
  VD result = sqrt(val);
  BOOST_CHECK(boost::math::isnan(result.GetValue()));
  BOOST_CHECK(boost::math::isnan(result.GetError()));
}

BOOST_AUTO_TEST_SUITE_END() // _sqrt

BOOST_AUTO_TEST_SUITE(_log)

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(2.0,2.0);
  VD result = log(val);
  BOOST_CHECK_CLOSE(result.GetValue(),ln_two,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 0.5 * 2.0, D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val(0.0,2.0);
  VD result = log(val);
  BOOST_CHECK(boost::math::isinf(result.GetValue()));
  BOOST_CHECK(boost::math::isinf(result.GetError()));
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val(-2.0,2.0);
  VD result = log(val);
  BOOST_CHECK(boost::math::isnan(result.GetValue()));
  BOOST_CHECK_CLOSE(result.GetError(), 0.5 * 2.0, D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _log

BOOST_AUTO_TEST_SUITE(_log10)

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(10.0,2.0);
  VD result = log10(val);
  BOOST_CHECK_CLOSE(result.GetValue(),1.0,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 0.2 / ln_ten, D_LARGE_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val(0.0,2.0);
  VD result = log10(val);
  BOOST_CHECK(boost::math::isinf(result.GetValue()));
  BOOST_CHECK(boost::math::isinf(result.GetError()));
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val(-2.0,2.0);
  VD result = log10(val);
  BOOST_CHECK(boost::math::isnan(result.GetValue()));
  BOOST_CHECK_CLOSE(result.GetError(), 0.5 / ln_ten * 2.0, D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _log10

BOOST_AUTO_TEST_SUITE(_acos)

BOOST_AUTO_TEST_CASE(positive_special)
{
  VD val(1.0,2.0);
  VD result = acos(val);
  BOOST_CHECK_SMALL(result.GetValue(),D_EPS);
  BOOST_CHECK(boost::math::isinf(result.GetError()));
}

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(one_div_root_two,2.0);
  VD result = acos(val);
  BOOST_CHECK_CLOSE(result.GetValue(),pi / 4.0,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),root_two * 2.0,D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val(0.0,2.0);
  VD result = acos(val);
  BOOST_CHECK_CLOSE(result.GetValue(),half_pi,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),2.0,D_EPS);
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val(-one_div_root_two,2.0);
  VD result = acos(val);
  BOOST_CHECK_CLOSE(result.GetValue(),three_quarters_pi,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),root_two * 2.0,D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _acos

BOOST_AUTO_TEST_SUITE(_asin)

BOOST_AUTO_TEST_CASE(positive_special)
{
  VD val(1.0,2.0);
  VD result = asin(val);
  BOOST_CHECK_CLOSE(result.GetValue(),half_pi,D_EPS);
  BOOST_CHECK(boost::math::isinf(result.GetError()));
}

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(one_div_root_two,2.0);
  VD result = asin(val);
  BOOST_CHECK_CLOSE(result.GetValue(),quarter_pi ,D_LARGE_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),root_two * 2.0,D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val(0.0,2.0);
  VD result = asin(val);
  BOOST_CHECK_SMALL(result.GetValue(),D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),2.0,D_EPS);
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val(-one_div_root_two,2.0);
  VD result = asin(val);
  BOOST_CHECK_CLOSE(result.GetValue(),-quarter_pi,D_LARGE_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),root_two * 2.0,D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _asin

BOOST_AUTO_TEST_SUITE(_atan)

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(1.0,2.0);
  VD result = atan(val);
  BOOST_CHECK_CLOSE(result.GetValue(),quarter_pi,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),1.0,D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val(0.0,2.0);
  VD result = atan(val);
  BOOST_CHECK_SMALL(result.GetValue(),D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),2.0,D_EPS);
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val(-1.0,2.0);
  VD result = atan(val);
  BOOST_CHECK_CLOSE(result.GetValue(),-quarter_pi,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),1.0,D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _atan

BOOST_AUTO_TEST_SUITE(_atan2_pv_pv)

// error is symmetric in x and y, so we need to test only three cases instead of nine
BOOST_AUTO_TEST_CASE(positive)
{
  VD val1(1.0,2.0);
  VD val2(1.0,4.0);
  VD result = atan2(val1,val2);
  BOOST_CHECK_CLOSE(result.GetValue(),quarter_pi,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),std::sqrt(5.0) ,D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val1(0.0,1.0);
  VD val2(0.0,2.0);
  VD result = atan2(val1,val2);
  BOOST_CHECK_SMALL(result.GetValue(),D_EPS);
  BOOST_CHECK(boost::math::isnan(result.GetError()));
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val1(-1.0,2.0);
  VD val2(1.0,4.0);
  VD result = atan2(val1,val2);
  BOOST_CHECK_CLOSE(result.GetValue(),-quarter_pi,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),std::sqrt(5.0) ,D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _atan2_pv_pv

BOOST_AUTO_TEST_SUITE(_atan2_pv_double)

// error is symmetric in x and y, so we need to test only three cases instead of nine
BOOST_AUTO_TEST_CASE(positive)
{
  VD val1(1.0,2.0);
  double val2 = 1.0;
  VD result = atan2(val1,val2);
  BOOST_CHECK_CLOSE(result.GetValue(),quarter_pi,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),1.0 ,D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val1(0.0,1.0);
  double val2 = 0.0;
  VD result = atan2(val1,val2);
  BOOST_CHECK_SMALL(result.GetValue(),D_EPS);
  BOOST_CHECK(boost::math::isnan(result.GetError()));
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val1(-1.0,2.0);
  double val2 = 1.0;
  VD result = atan2(val1,val2);
  BOOST_CHECK_CLOSE(result.GetValue(),-quarter_pi,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),1.0 ,D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _atan2_pv_double

BOOST_AUTO_TEST_SUITE(_atan2_double_pv)

// error is symmetric in x and y, so we need to test only three cases instead of nine
BOOST_AUTO_TEST_CASE(positive)
{
  double val1 = 1.0;
  VD val2(1.0,2.0);
  VD result = atan2(val1,val2);
  BOOST_CHECK_CLOSE(result.GetValue(),quarter_pi,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),1.0 ,D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  double val1 = 0.0;
  VD val2(0.0,2.0);
  VD result = atan2(val1,val2);
  BOOST_CHECK_SMALL(result.GetValue(),D_EPS);
  BOOST_CHECK(boost::math::isnan(result.GetError()));
}

BOOST_AUTO_TEST_CASE(negative)
{
  double val1 = -1.0;
  VD val2(1.0,2.0);
  VD result = atan2(val1,val2);
  BOOST_CHECK_CLOSE(result.GetValue(),-quarter_pi,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),1.0 ,D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _atan2_double_pv

BOOST_AUTO_TEST_SUITE(_sinh)

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(1.0,2.0);
  VD result = sinh(val);
  BOOST_CHECK_CLOSE(result.GetValue(),( e - 1.0 / e )/ 2.0 ,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),( e + 1.0 / e )/ 2.0 * 2.0 ,D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val(0.0,2.0);
  VD result = sinh(val);
  BOOST_CHECK_SMALL(result.GetValue(),D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),1.0 * 2.0,D_EPS);
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val(-1.0,2.0);
  VD result = sinh(val);
  BOOST_CHECK_CLOSE(result.GetValue(),-( e - 1.0 / e )/ 2.0 ,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),( e + 1.0 / e )/ 2.0 * 2.0 ,D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _sinh

BOOST_AUTO_TEST_SUITE(_cosh)

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(1.0,2.0);
  VD result = cosh(val);
  BOOST_CHECK_CLOSE(result.GetValue(),( e + 1.0 / e )/ 2.0 ,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),( e - 1.0 / e )/ 2.0 * 2.0 ,D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val(0.0,2.0);
  VD result = cosh(val);
  BOOST_CHECK_CLOSE(result.GetValue(),1.0, D_EPS);
  BOOST_CHECK_SMALL(result.GetError(), D_EPS);
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val(-1.0,2.0);
  VD result = cosh(val);
  BOOST_CHECK_CLOSE(result.GetValue(),( e + 1.0 / e )/ 2.0 ,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),( e - 1.0 / e )/ 2.0 * 2.0 ,D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _cosh

BOOST_AUTO_TEST_SUITE(_tanh)

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(1.0,2.0);
  VD result = tanh(val);
  BOOST_CHECK_CLOSE(result.GetValue(),( e - 1.0 / e )/ ( e + 1.0 / e ) ,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 1.0 / pow<2>( ( e + 1.0 / e )/ 2.0) * 2.0 ,D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val(0.0,2.0);
  VD result = tanh(val);
  BOOST_CHECK_SMALL(result.GetValue(),D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),1.0 * 2.0,D_EPS);
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val(-1.0,2.0);
  VD result = tanh(val);
  BOOST_CHECK_CLOSE(result.GetValue(),-( e - 1.0 / e )/ ( e + 1.0 / e ) ,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 1.0 / pow<2>( ( e + 1.0 / e )/ 2.0) * 2.0 ,D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _tanh

BOOST_AUTO_TEST_SUITE(_tan)

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(quarter_pi,2.0);
  VD result = tan(val);
  BOOST_CHECK_CLOSE(result.GetValue(), 1.0 ,D_LARGE_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 2.0 * 2.0 ,D_LARGE_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val(0.0,2.0);
  VD result = tan(val);
  BOOST_CHECK_SMALL(result.GetValue(),D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 1.0 * 2.0,D_EPS);
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val(-quarter_pi,2.0);
  VD result = tan(val);
  BOOST_CHECK_CLOSE(result.GetValue(), -1.0 ,D_LARGE_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 2.0 * 2.0 ,D_LARGE_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _tan


BOOST_AUTO_TEST_SUITE_END() // Test_MathOverloads

#ifdef __clang__
#pragma clang diagnostic pop
#endif // __clang__

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic pop
#endif

