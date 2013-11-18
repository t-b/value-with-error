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

BOOST_AUTO_TEST_SUITE(ctor)

BOOST_AUTO_TEST_CASE(zero_args)
{
  VD pv;
  BOOST_CHECK_PV(pv,0.0,0.0);
}

BOOST_AUTO_TEST_CASE(one_arg)
{
  VD pv(val);
  BOOST_CHECK_PV(pv,val,0.0);
}

BOOST_AUTO_TEST_CASE(two_args)
{
  BOOST_CHECK_PV(physVal1,val,error);
}

BOOST_AUTO_TEST_CASE(narrowing)
{
  PVF pv(val, error);
  BOOST_CHECK_PV(pv,val,error);
}

BOOST_AUTO_TEST_SUITE_END() // ctor

BOOST_AUTO_TEST_CASE(getter)
{
  VD pv;
  VD::value_type val = pv.GetValue();
  VD::value_type err = pv.GetError();
  BOOST_CHECK_PV(pv,val,err);
}

BOOST_AUTO_TEST_SUITE(operator_assign)

BOOST_AUTO_TEST_CASE(pv)
{
  VD physValTo;
  physValTo = physVal1;

  BOOST_CHECK_PV(physValTo,val,error);
}

BOOST_AUTO_TEST_CASE(compatible_pv)
{
  VD physValTo;
  physValTo = physValFloat;

  BOOST_CHECK_PV(physValTo,val,error);
}

BOOST_AUTO_TEST_CASE(value)
{
  VD physValTo = VD(3.0,4.0);
  physValTo = val;

  BOOST_CHECK_PV(physValTo,val,0.0);
}

BOOST_AUTO_TEST_CASE(compatible_value)
{
  VD physValTo = VD(3.0,4.0);
  physValTo = (float) val;

  BOOST_CHECK_PV(physValTo,val,0.0);
}

BOOST_AUTO_TEST_SUITE_END() // operator_assign

BOOST_AUTO_TEST_CASE(operator_times)
{
  double resultVal = 1.0;
  double resultError = std::sqrt(8.0);

  VD result = physVal1 * physVal2;
  BOOST_CHECK_PV(result,resultVal,resultError);

  double scalar = 3.0;
  VD resultSM1 = physVal1 * scalar;
  VD resultSM2 = scalar * physVal1;

  BOOST_CHECK_PV(resultSM1, 3.0,6.0);
  BOOST_CHECK_PV(resultSM2, 3.0,6.0);
}

BOOST_AUTO_TEST_CASE(operator_timesEqual)
{
  double resultVal = 1.0;
  double resultError = std::sqrt(8.0);
  VD result = physVal1;
  result *= physVal2;

  BOOST_CHECK_PV(result,resultVal,resultError);

  double scalar = 3.0;
  result = physVal1;
  result *= scalar;

  BOOST_CHECK_PV(result, 3.0,6.0);
}

BOOST_AUTO_TEST_CASE(operator_divides)
{
  double resultVal = 1.0;
  double resultError = std::sqrt(8.0);

  VD result = physVal1 / physVal2;
  BOOST_CHECK_PV(result,resultVal,resultError);

  double scalar = 0.5;
  VD resultSM1 = physVal1 / scalar;
  VD resultSM2 = scalar / physVal1;

  BOOST_CHECK_PV(resultSM1, 2.0,4.0);
  BOOST_CHECK_PV(resultSM2, 0.5,1.0);
}

BOOST_AUTO_TEST_CASE(operator_divideEqual)
{
  double resultVal = 1.0;
  double resultError = std::sqrt(8.0);
  VD result = physVal1;
  result /= physVal2;

  BOOST_CHECK_PV(result,resultVal,resultError);

  double scalar = 0.5;
  result = physVal1;
  result /= scalar;
  BOOST_CHECK_PV(result, 2.0,4.0);
}

BOOST_AUTO_TEST_CASE(operator_plus)
{
  double resultVal = 2.0;
  double resultError = std::sqrt(8.0);

  VD result = physVal1 + physVal2;

  BOOST_CHECK_PV(result,resultVal,resultError);

  double scalar = 3.0;
  VD resultSM1 = physVal1 + scalar;
  VD resultSM2 = scalar + physVal1;

  BOOST_CHECK_PV(resultSM1, 4.0, physVal1.GetError());
  BOOST_CHECK_PV(resultSM2, 4.0, physVal1.GetError());
}

BOOST_AUTO_TEST_CASE(operator_plusEqual)
{
  double resultVal = 2.0;
  double resultError = std::sqrt(8.0);
  VD result = physVal1;
  result += physVal2;

  BOOST_CHECK_PV(result,resultVal,resultError);

  double scalar = 3.0;
  result = physVal1;
  result += scalar;

  BOOST_CHECK_PV(result, 4.0, physVal1.GetError());
}

BOOST_AUTO_TEST_CASE(operator_minus)
{
  double resultVal = 0.0;
  double resultError = std::sqrt(8.0);

  VD result = physVal1 - physVal2;

  BOOST_CHECK_PV(result,resultVal,resultError);

  double scalar = 3.0;
  VD resultSM1 = physVal1 - scalar;
  VD resultSM2 = scalar - physVal1;

  BOOST_CHECK_PV(resultSM1, -2.0,physVal1.GetError());
  BOOST_CHECK_PV(resultSM2, 2.0,physVal1.GetError());
}

BOOST_AUTO_TEST_CASE(operator_minusEqual)
{
  double resultVal = 0.0;
  double resultError = std::sqrt(8.0);
  VD result = physVal1;
  result -= physVal2;

  BOOST_CHECK_PV(result,resultVal,resultError);

  double scalar = 3.0;
  result = physVal1;
  result -= scalar;
  BOOST_CHECK_PV(result, -2.0,physVal1.GetError());
}

BOOST_AUTO_TEST_CASE(operator_times_promotion)
{
  double valHigh = 1.0;
  double errorHigh = 2.0;
  float valLow = 1.0;
  float errorLow = 2.0;
  double resultVal = 1.0;
  double resultError = std::sqrt(8.0);
  PVF physValLow(valLow,errorLow);
  VD physValHigh(valHigh,errorHigh);

  VD result = physValLow * physValHigh;
  BOOST_CHECK_PV_TYPE(physValLow * physValHigh, valHigh);
  BOOST_CHECK_PV(result,resultVal,resultError);
}

// See http://llvm.org/bugs/show_bug.cgi?id=14989
#ifndef __clang__
BOOST_AUTO_TEST_CASE(operator_outputOperator)
{
  std::stringstream sstr;
  sstr << physVal1;

  BOOST_CHECK(sstr);
  BOOST_CHECK_EQUAL(sstr.str(), "(1+-2)");
}
#endif

BOOST_AUTO_TEST_CASE(operator_inputOperator)
{
  std::stringstream sstr;
  sstr << '(' << physVal1.GetValue()<< "+-" << physVal1.GetError() << ')';
  VD physVal;
  sstr >> physVal;

  BOOST_CHECK(sstr);
  BOOST_CHECK_EQUAL(physVal,physVal1);
}

BOOST_AUTO_TEST_CASE(operator_inputOperator_with_ws)
{
  std::stringstream sstr;
  sstr << " ( " << physVal1.GetValue()<< " + - " << physVal1.GetError() << "  )  ";
  VD physVal;
  sstr >> physVal;

  BOOST_CHECK(sstr);
  BOOST_CHECK_EQUAL(physVal,physVal1);
}

BOOST_AUTO_TEST_CASE(operator_inputOperator_wrong_format_1)
{
  std::stringstream sstr;
  sstr << "x";
  VD physVal;
  sstr >> physVal;

  BOOST_CHECK(!sstr);
  BOOST_CHECK_EQUAL(physVal,VD());
}

BOOST_AUTO_TEST_CASE(operator_inputOperator_wrong_format_2)
{
  std::stringstream sstr;
  sstr << "(x";
  VD physVal;
  sstr >> physVal;

  BOOST_CHECK(!sstr);
  BOOST_CHECK_EQUAL(physVal,VD());
}

BOOST_AUTO_TEST_CASE(operator_inputOperator_wrong_format_3)
{
  std::stringstream sstr;
  sstr << "( 0-";
  VD physVal;
  sstr >> physVal;

  BOOST_CHECK(!sstr);
  BOOST_CHECK_EQUAL(physVal,VD());
}

BOOST_AUTO_TEST_CASE(operator_inputOperator_wrong_format_4)
{
  std::stringstream sstr;
  sstr << "( 0++";
  VD physVal;
  sstr >> physVal;

  BOOST_CHECK(!sstr);
  BOOST_CHECK_EQUAL(physVal,VD());
}

BOOST_AUTO_TEST_CASE(operator_inputOperator_wrong_format_5)
{
  std::stringstream sstr;
  sstr << "( 0+-x";
  VD physVal;
  sstr >> physVal;

  BOOST_CHECK(!sstr);
  BOOST_CHECK_EQUAL(physVal,VD());
}

BOOST_AUTO_TEST_CASE(operator_inputOperator_wrong_format_6)
{
  std::stringstream sstr;
  sstr << "( 0+-0x";
  VD physVal;
  sstr >> physVal;

  BOOST_CHECK(!sstr);
  BOOST_CHECK_EQUAL(physVal,VD());
}

BOOST_AUTO_TEST_CASE(swap_in_namespace)
{
  VD val1(1.0,2.0);
  VD val2(3.0,4.0);

  VD t = val1;
  VD u = val2;

  BOOST_CHECK_EQUAL(t,val1);
  BOOST_CHECK_EQUAL(u,val2);
  swap(t,u); // found via ADL
  BOOST_CHECK_EQUAL(u,val1);
  BOOST_CHECK_EQUAL(t,val2);
}

BOOST_AUTO_TEST_CASE(member_swap)
{
  VD val1(1.0,2.0);
  VD val2(3.0,4.0);

  VD t = val1;
  VD u = val2;

  BOOST_CHECK_EQUAL(t,val1);
  BOOST_CHECK_EQUAL(u,val2);
  t.swap(u);
  BOOST_CHECK_EQUAL(u,val1);
  BOOST_CHECK_EQUAL(t,val2);
}

BOOST_AUTO_TEST_SUITE_END() // Test_ValueWithError

#ifdef __clang__
#pragma clang diagnostic pop
#endif // __clang__

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic pop
#endif

