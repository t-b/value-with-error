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

BOOST_AUTO_TEST_SUITE(_expm1)

BOOST_AUTO_TEST_CASE(positive)
{
  VD pv(1.0,2.0);
  VD result = expm1(pv);
  BOOST_CHECK_CLOSE(result.GetValue(),e - 1.0,D_LARGE_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),2.0 * e,D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD pv(0.0,2.0);
  VD result = expm1(pv);
  BOOST_CHECK_SMALL(result.GetValue(),D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),2.0,D_EPS);
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD pv(-1.0,2.0);
  VD result = expm1(pv);
  BOOST_CHECK_CLOSE(result.GetValue(),1.0 / e - 1.0,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),2.0 / e,D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _expm1

BOOST_AUTO_TEST_SUITE(_exp2)

BOOST_AUTO_TEST_CASE(positive)
{
  VD pv(1.0,2.0);
  VD result = exp2(pv);
  BOOST_CHECK_CLOSE(result.GetValue(),2.0,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),ln_two * 2.0 * 2.0,D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD pv(0.0,2.0);
  VD result = exp2(pv);
  BOOST_CHECK_CLOSE(result.GetValue(), 1.0, D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), ln_two*2.0, D_EPS);
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD pv(-1.0,2.0);
  VD result = exp2(pv);
  BOOST_CHECK_CLOSE(result.GetValue(),0.5,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), ln_two * 0.5 * 2.0, D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _exp2

BOOST_AUTO_TEST_SUITE(_log2)

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(2.0,2.0);
  VD result = log2(val);
  BOOST_CHECK_CLOSE(result.GetValue(),1.0,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 1.0 / ln_two, D_LARGE_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val(0.0,2.0);
  VD result = log2(val);
  BOOST_CHECK(boost::math::isinf(result.GetValue()));
  BOOST_CHECK(boost::math::isinf(result.GetError()));
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val(-2.0,2.0);
  VD result = log2(val);
  BOOST_CHECK(boost::math::isnan(result.GetValue()));
  BOOST_CHECK_CLOSE(result.GetError(), 0.5 / ln_two * 2.0, D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _log2

BOOST_AUTO_TEST_SUITE(_log1p)

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(1.0,2.0);
  VD result = log1p(val);
  BOOST_CHECK_CLOSE(result.GetValue(),ln_two,D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 1.0, D_LARGE_EPS);
}

BOOST_AUTO_TEST_CASE(negative_special_case)
{
  // -1.0 is special
  VD val(-1.0,2.0);
  VD result = log1p(val);
  BOOST_CHECK(boost::math::isinf(result.GetValue()));
  BOOST_CHECK(boost::math::isinf(result.GetError()));
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val(-3.0,2.0);
  VD result = log1p(val);
  BOOST_CHECK(boost::math::isnan(result.GetValue()));
  BOOST_CHECK_CLOSE(result.GetError(), 1.0 , D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _log1p

BOOST_AUTO_TEST_SUITE(_asinh)

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(1.0,2.0);
  VD result = asinh(val);
  BOOST_CHECK_CLOSE(result.GetValue(), std::log(1.0 + root_two), D_LARGE_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 1.0 / root_two * 2.0, D_LARGE_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val(0.0,2.0);
  VD result = asinh(val);
  BOOST_CHECK_SMALL(result.GetValue(),D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(),2.0,D_EPS);
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val(-1.0,2.0);
  VD result = asinh(val);
  BOOST_CHECK_CLOSE(result.GetValue(),std::log(-1.0 + root_two),D_LARGE_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 1.0 / root_two * 2.0 ,D_LARGE_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _asinh

BOOST_AUTO_TEST_SUITE(_acosh)

BOOST_AUTO_TEST_CASE(positive_special_plus_one)
{
  VD val(1.0,2.0);
  VD result = acosh(val);
  BOOST_CHECK_SMALL(result.GetValue(), D_EPS);
  BOOST_CHECK(boost::math::isinf(result.GetError()));
}

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(2.0,3.0);
  VD result = acosh(val);
  BOOST_CHECK_CLOSE(result.GetValue(), std::log( 2.0 + std::sqrt(3.0) ), D_LARGE_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 1.0 / std::sqrt(3.0) * 3.0, D_LARGE_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _acosh

BOOST_AUTO_TEST_SUITE(_atanh)

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(0.5,2.0);
  VD result = atanh(val);
  BOOST_CHECK_CLOSE(result.GetValue(), 0.5 * std::log(3.0), D_LARGE_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 4.0/3.0 * 2.0, D_EPS);
}

BOOST_AUTO_TEST_CASE(positive_one_special)
{
  VD val(1.0,2.0);
  VD result = atanh(val);
  BOOST_CHECK(boost::math::isinf(result.GetValue()));
  BOOST_CHECK(boost::math::isinf(result.GetError()));
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val(0.0,2.0);
  VD result = atanh(val);
  BOOST_CHECK_SMALL(result.GetValue(), D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 2.0, D_EPS);
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val(-0.5,2.0);
  VD result = atanh(val);
  BOOST_CHECK_CLOSE(result.GetValue(), -0.5 * std::log(3.0), D_LARGE_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 4.0/3.0 * 2.0, D_EPS);
}

BOOST_AUTO_TEST_CASE(negative_one_special)
{
  VD val(-1.0,2.0);
  VD result = atanh(val);
  BOOST_CHECK(boost::math::isinf(result.GetValue()));
  BOOST_CHECK(boost::math::isinf(result.GetError()));
}

BOOST_AUTO_TEST_SUITE_END() // _atanh

BOOST_AUTO_TEST_SUITE(_cbrt)

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(1.0,2.0);
  VD result = cbrt(val);
  BOOST_CHECK_CLOSE(result.GetValue(), 1.0, D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 1.0 / 3.0 * 2.0, D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val(0.0,2.0);
  VD result = cbrt(val);
  BOOST_CHECK_SMALL(result.GetValue(),D_EPS);
  BOOST_CHECK(boost::math::isinf(result.GetError()));
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val(-1.0,2.0);
  VD result = cbrt(val);
  BOOST_CHECK_CLOSE(result.GetValue(), -1.0, D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), 1.0 / 3.0 * 2.0 , D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _cbrt

BOOST_AUTO_TEST_SUITE(_erf)

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(0.2,2.0);
  VD result = erf(val);
  // Taken from Abramowitz and Stegun, 9th Dover printing, Table 7.1 page 310
  BOOST_CHECK_CLOSE(result.GetValue(), 0.2227025892, 1e-8);
  BOOST_CHECK_CLOSE(result.GetError(), two_div_root_pi * std::exp(-0.04) * 2.0 , D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val(0.0,2.0);
  VD result = erf(val);
  BOOST_CHECK_SMALL(result.GetValue(), D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), two_div_root_pi * 2.0, D_EPS);
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val(-0.2,2.0);
  VD result = erf(val);
  // Taken from Abramowitz and Stegun, 9th Dover printing, Table 7.1 page 310
  // and changed sign
  BOOST_CHECK_CLOSE(result.GetValue(), -0.2227025892, 1e-8);
  BOOST_CHECK_CLOSE(result.GetError(),  two_div_root_pi * std::exp(-0.04) * 2.0 , D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _erf

BOOST_AUTO_TEST_SUITE(_erfc)

BOOST_AUTO_TEST_CASE(positive)
{
  VD val(0.2,2.0);
  VD result = erfc(val);
  // Taken from Abramowitz and Stegun, 9th Dover printing, Table 7.1 page 310
  BOOST_CHECK_CLOSE(result.GetValue(), 1.0 - 0.2227025892, 1e-8);
  BOOST_CHECK_CLOSE(result.GetError(), two_div_root_pi * std::exp(-0.04) * 2.0 , D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD val(0.0,2.0);
  VD result = erfc(val);
  BOOST_CHECK_CLOSE(result.GetValue(), 1.0 , D_EPS);
  BOOST_CHECK_CLOSE(result.GetError(), two_div_root_pi * 2.0, D_EPS);
}

BOOST_AUTO_TEST_CASE(negative)
{
  VD val(-0.2,2.0);
  VD result = erfc(val);
  // Taken from Abramowitz and Stegun, 9th Dover printing, Table 7.1 page 310
  // and changed sign
  BOOST_CHECK_CLOSE(result.GetValue(), 1.0 + 0.2227025892, 1e-8);
  BOOST_CHECK_CLOSE(result.GetError(), two_div_root_pi * std::exp(-0.04) * 2.0 , D_EPS);
}

BOOST_AUTO_TEST_SUITE_END() // _erfc

BOOST_AUTO_TEST_SUITE(_hypot)

BOOST_AUTO_TEST_SUITE(x_y_pv)

BOOST_AUTO_TEST_CASE(positive)
{
  VD x(3.0,1.0);
  VD y(4.0,2.0);
  VD result = hypot(x,y);
  BOOST_CHECK_CLOSE(result.GetValue(), 5.0, D_LARGE_EPS );
  BOOST_CHECK_CLOSE(result.GetError(), 1.0 / 25.0 * std::hypot(3.0 * 1.0, 4.0 * 2.0), D_LARGE_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD x(0.0,1.0);
  VD y(0.0,2.0);
  VD result = hypot(x,y);
  BOOST_CHECK_CLOSE(result.GetValue(), 0.0 , D_EPS);
  BOOST_CHECK(boost::math::isnan(result.GetError()));
}

BOOST_AUTO_TEST_SUITE_END() // x_y_positive

BOOST_AUTO_TEST_SUITE(x_pv_y_double)

BOOST_AUTO_TEST_CASE(positive)
{
  VD x(3.0,1.0);
  double y = 4.0;
  VD result = hypot(x,y);
  BOOST_CHECK_CLOSE(result.GetValue(), 5.0, D_EPS );
  BOOST_CHECK_CLOSE(result.GetError(), 1.0 / 25.0 * 3.0 * 1.0, D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD x(0.0,1.0);
  double y = 0.0;
  VD result = hypot(x,y);
  BOOST_CHECK_CLOSE(result.GetValue(), 0.0, D_EPS);
  BOOST_CHECK(boost::math::isnan(result.GetError()));
}

BOOST_AUTO_TEST_SUITE_END() // x_pv_y_double

BOOST_AUTO_TEST_SUITE(x_double_y_pv)

BOOST_AUTO_TEST_CASE(positive)
{
  double x = 4.0;
  VD y(3.0,1.0);
  VD result = hypot(x,y);
  BOOST_CHECK_CLOSE(result.GetValue(), 5.0, D_EPS );
  BOOST_CHECK_CLOSE(result.GetError(), 1.0 / 25.0 * 3.0 * 1.0, D_EPS);
}

BOOST_AUTO_TEST_CASE(zero)
{
  double x = 0.0;
  VD y(0.0,1.0);
  VD result = hypot(x,y);
  BOOST_CHECK_CLOSE(result.GetValue(), 0.0, D_EPS);
  BOOST_CHECK(boost::math::isnan(result.GetError()));
}

BOOST_AUTO_TEST_SUITE_END() // x_double_y_pv

BOOST_AUTO_TEST_SUITE_END() // _hypot

BOOST_AUTO_TEST_SUITE(_tgamma)

BOOST_AUTO_TEST_CASE(positive_integer)
{
  VD x(4.0,1.0);
  VD result = tgamma(x);
  BOOST_CHECK_CLOSE(result.GetValue(), 6.0, D_EPS ); // gamma(n) = factorial(n-1) = 4*3*2*1: see Abramowitz & Stegun p. 255
  BOOST_CHECK_CLOSE(result.GetError(), (-euler + 1.0 + 0.5 + 1.0/3.0) * 6.0 * x.GetError(), D_LARGE_EPS); // psi(n) * gamma(n), psi(n): see Abramowitz & Stegun p. 258
}

BOOST_AUTO_TEST_CASE(positive_fractional)
{
  VD x(0.5,1.0);
  VD result = tgamma(x);
  BOOST_CHECK_CLOSE(result.GetValue(), 1.772453850905516, D_EPS ); // tabulated gamma(1/2): see Abramowitz & Stegun p. 3
  BOOST_CHECK_CLOSE(result.GetError(), std::abs((-euler - 2.0*ln_two) * 1.772453850905516 * x.GetError()), D_LARGE_EPS); // psi(1/2): see Abramowitz & Stegun p. 258
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD x(0.0,1.0);
  VD result = tgamma(x);
  BOOST_CHECK(boost::math::isinf(result.GetValue()));
  BOOST_CHECK(boost::math::isinf(result.GetError()));
}

BOOST_AUTO_TEST_CASE(negative_integer)
{
  VD x(-1.0,1.0);
  VD result = tgamma(x);
  BOOST_CHECK(boost::math::isnan(result.GetValue()));
  BOOST_CHECK(boost::math::isnan(result.GetError()));
}

BOOST_AUTO_TEST_CASE(negative_fractional)
{
  VD x(-0.5,1.0);
  VD result = tgamma(x);
  // An error threshold of 1e-8 used here as tabulated value for Gamma(3/2) is available from Abramowitz to 10 digits precision only. This test should pass with the given precision on a 32bit system.
  BOOST_CHECK_CLOSE(result.GetValue(), (pi/std::sin(-half_pi)) / 0.8862269254 , 1e-8 ); // tabulated Gamma(3/2), see Abramowitz & Stegun p. 255, reflection formula: Gamma(z)*Gamma(1-z) = pi*csc(pi*z)
  BOOST_CHECK_CLOSE(result.GetError(), std::abs(((-euler - 2.0*ln_two)-1/(-0.5)) * ((pi/std::sin(-half_pi))/0.8862269254) * x.GetError()), 1e-8); // psi(1/2), recurrence formula psi(1+z) = psi(z) + 1/z, see Abramowitz & Stegun p. 258
}

BOOST_AUTO_TEST_SUITE_END() // _tgamma

BOOST_AUTO_TEST_SUITE(_lgamma)

BOOST_AUTO_TEST_CASE(positive_integer)
{
  VD x(4.0,1.0);
  VD result = lgamma(x);
  BOOST_CHECK_CLOSE(result.GetValue(), std::log(6.0), D_EPS ); // gamma(n) = factorial(n-1) = 4*3*2*1, see Abramowitz & Stegun p. 255
  BOOST_CHECK_CLOSE(result.GetError(), (-euler + 1.0 + 0.5 + 1.0/3.0) * x.GetError(), D_LARGE_EPS); // psi(n), see Abramowitz & Stegun p. 258
}

BOOST_AUTO_TEST_CASE(positive_fractional)
{
  VD x(0.5,1.0);
  VD result = lgamma(x);
  BOOST_CHECK_CLOSE(result.GetValue(), std::log(1.772453850905516), D_EPS ); // tabulated gamma(1/2), see Abramowitz & Stegun p. 3
  BOOST_CHECK_CLOSE(result.GetError(), std::abs((-euler - 2.0*ln_two) * x.GetError()), D_EPS); // psi(1/2), see Abramowitz & Stegun p. 258
}

BOOST_AUTO_TEST_CASE(zero)
{
  VD x(0.0,1.0);
  VD result = lgamma(x);
  BOOST_CHECK(boost::math::isinf(result.GetValue()));
  BOOST_CHECK(boost::math::isinf(result.GetError()));
}

BOOST_AUTO_TEST_CASE(negative_integer)
{
  VD x(-1.0,1.0);
  VD result = lgamma(x);
  BOOST_CHECK(boost::math::isinf(result.GetValue())); // TODO: lgamma and tgamma specifications do not match in linux c++ standard library libstdc++.so.6.0.17 from gcc-4.7, check this with 4.8.
  BOOST_CHECK(boost::math::isnan(result.GetError()));
}

BOOST_AUTO_TEST_CASE(negative_fractional)
{
  VD x(-0.5,1.0);
  VD result = lgamma(x);
  // An error threshold of 1e-8 used here as tabulated value for Gamma(3/2) is available from Abramowitz to 10 digits precision only. This test should pass with the given precision on a 32bit system.
  BOOST_CHECK_CLOSE(result.GetValue(), std::log(std::abs((pi/std::sin(-half_pi)) / 0.8862269254)) , 1e-8 ); // tabulated Gamma(3/2), see Abramowitz & Stegun p. 255, reflection formula: Gamma(z)*Gamma(1-z) = pi*csc(pi*z)
  BOOST_CHECK_CLOSE(result.GetError(), std::abs(((-euler - 2.0*ln_two)-1/(-0.5)) * x.GetError()), 1e-8); // psi(1/2), recurrence formula psi(1+z) = psi(z) + 1/z, see Abramowitz & Stegun p. 258
}

BOOST_AUTO_TEST_SUITE_END() // _lgamma

BOOST_AUTO_TEST_SUITE_END() // Test_MathOverloads

#ifdef __clang__
#pragma clang diagnostic pop
#endif // __clang__

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic pop
#endif

