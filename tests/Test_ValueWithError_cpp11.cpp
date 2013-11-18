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
    double GetError() { return error; }

    const double val, error;
    const VD physVal1, physVal2, physValDefault;
    const PVF physValFloat;
  };

} // anonymous namespace

BOOST_FIXTURE_TEST_SUITE(Test_ValueWithError_Basics,Fixture)

BOOST_AUTO_TEST_SUITE(ctor)

BOOST_AUTO_TEST_CASE(move_pv)
{
  VD pv(std::move(GetValueWithError()));
  BOOST_CHECK_PV(pv,val,error);
}

BOOST_AUTO_TEST_CASE(move_val_move_error)
{
  VD pv(std::move(GetValue()),std::move(GetError()*-1.0));
  BOOST_CHECK_PV(pv,val,error);
}

BOOST_AUTO_TEST_CASE(move_val_copy_error)
{
  VD pv(std::move(GetValue()),error);
  BOOST_CHECK_PV(pv,val,error);
}

BOOST_AUTO_TEST_CASE(copy_val_move_error)
{
  VD pv(val,std::move(GetError()));
  BOOST_CHECK_PV(pv,val,error);
}

BOOST_AUTO_TEST_CASE(move_val)
{
  VD pv(std::move(GetValue()));
  BOOST_CHECK_PV(pv,val,0.0);
}

BOOST_AUTO_TEST_CASE(move_value)
{
  VD pv(std::move(GetValue()));
  BOOST_CHECK_PV(pv,val,0.0);
}

BOOST_AUTO_TEST_SUITE_END() // ctor

BOOST_AUTO_TEST_CASE(initializer_list_zero_args)
{
  VD result;
  VD pv {};
  BOOST_CHECK_PVS(pv,result);
}

BOOST_AUTO_TEST_CASE(initializer_list_one_arg)
{
  VD result(1.0);
  VD pv { 1.0 };
  BOOST_CHECK_PVS(pv,result);
}

BOOST_AUTO_TEST_CASE(initializer_list_two_args)
{
  VD result(1.0, 2.0);
  VD pv { 1.0, 2.0 };
  BOOST_CHECK_PVS(pv,result);
}

BOOST_AUTO_TEST_CASE(initializer_list_more_than_two_args)
{
  VD result(1.0, 2.0);
  VD pv { 1.0, 2.0, 3.0 };
  BOOST_CHECK_PVS(pv,result);
}

// Results in narrowing warning
// BOOST_AUTO_TEST_CASE(initializer_list_narrowing)
// {
//   PVF pv { val , error };
//   BOOST_CHECK_EQUAL(pv.GetValue(),val);
//   BOOST_CHECK_EQUAL(pv.GetError(),error);
// }

BOOST_AUTO_TEST_CASE(initializer_list_vector)
{
  typedef std::vector<ValueWithError<double> > PVV;
  PVV pv { { 1.0, 2.0 }, { 3.0, 4.0 } , { 5.0 } };
  BOOST_CHECK( pv.size() == 3 );
  BOOST_CHECK_PV(pv[0],1.0,2.0);
  BOOST_CHECK_PV(pv[1],3.0,4.0);
  BOOST_CHECK_PV(pv[2],5.0,0.0);
}

BOOST_AUTO_TEST_CASE(initializer_list_c_array)
{
  VD pv[] { { 1.0, 2.0 }, { 3.0, 4.0 } };
  BOOST_CHECK_PV(pv[0],1.0,2.0);
  BOOST_CHECK_PV(pv[1],3.0,4.0);
}

BOOST_AUTO_TEST_CASE(operator_assign_initializer_empty)
{
  VD pv(1.0, 2.0);
  pv = {};
  BOOST_CHECK_PV(pv,0.0,0.0);
}

BOOST_AUTO_TEST_CASE(operator_assign_initializer_one)
{
  VD pv(1.0, 2.0);
  pv = { 3.0 };
  BOOST_CHECK_PV(pv,3.0,0.0);
}

BOOST_AUTO_TEST_CASE(operator_assign_initializer_two)
{
  VD pv(1.0, 2.0);
  pv = { 3.0, 4.0 };
  BOOST_CHECK_PV(pv,3.0,4.0);
}

BOOST_AUTO_TEST_CASE(operator_assign_initializer_two_convertible)
{
  VD pv(1.0, 2.0);
  pv = { 3.0f, 4.0f };
  BOOST_CHECK_PV(pv,3.0,4.0);
}

BOOST_AUTO_TEST_SUITE(_make_value)
// policy tested in Test_ValueWithError_Policy_make_value.cpp

BOOST_AUTO_TEST_CASE(type_decay)
{
  bool sucess = std::is_same<decltype(val), const double>::value;
  BOOST_CHECK(sucess);
  sucess = std::is_same<decltype(make_value(val, error))::value_type, double>::value;
  BOOST_CHECK(sucess);
}

BOOST_AUTO_TEST_CASE(type_promotion)
{
  bool success_float_double = std::is_same<decltype(make_value((float)val, (double)error))::value_type, double>::value;
  BOOST_CHECK(success_float_double);
  bool success_double_float = std::is_same<decltype(make_value((double)val, (float)error))::value_type, double>::value;
  BOOST_CHECK(success_double_float);
}

BOOST_AUTO_TEST_CASE(type_deduction)
{
  bool success = std::is_same<decltype(make_value((float)val, (float)error))::value_type, float>::value;
  BOOST_CHECK(success);
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

