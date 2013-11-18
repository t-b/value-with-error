#pragma once

extern const double D_NAN;
extern const float  F_NAN;

#define BOOST_CHECK_PV(A, B, C) \
  BOOST_CHECK_EQUAL(A.GetValue(),B); \
  BOOST_CHECK_EQUAL(A.GetError(),C);

#define BOOST_CHECK_PVS(A, B) \
  BOOST_CHECK_EQUAL(A.GetValue(),B.GetValue()); \
  BOOST_CHECK_EQUAL(A.GetError(),B.GetError());

#define BOOST_CHECK_PV_TYPE(A, B) \
  const bool is_same_type = is_same()(A, B); \
  BOOST_CHECK(is_same_type);

/// Allows to check if a ValueWithError<T,P> object and a value of type U have  U == T or not
struct is_same
{
    template<template <typename T, typename P> class ValueWithError, typename T, typename P>
    bool operator()(ValueWithError<T, P> pv, T t)
    {
      return true;
    }

    template<template <typename T, typename P> class ValueWithError, typename U, typename T, typename P>
    bool operator()(ValueWithError<T, P> pv, U t)
    {
      return false;
    }
};

#ifdef TEST_COMPATIBLE_TYPE
#define TEST_OP_COMPATIBLE_TYPE(A, B)   \
  BOOST_AUTO_TEST_CASE(A##_compat_type) \
  {                                     \
    VD a;                               \
    a B##= pvf;                         \
    a = pvd  B pvf;                     \
    a = pvd  B 1.0f;                    \
    a = 1.0f B pvd;                     \
    a = 1.0  B pvf;                     \
    a = pvf  B 1.0;                     \
    BOOST_CHECK(true);                  \
  }
#else
#define TEST_OP_COMPATIBLE_TYPE(A, B)
#endif // TEST_COMPATIBLE_TYPE

#define TEST_OP(A, B)         \
BOOST_AUTO_TEST_CASE(A)       \
{                             \
  VD a;                       \
  a B##= pvd;                 \
  a B##= 1.0;                 \
  a = pvd  B pvd;             \
  a = pvd  B 1.0;             \
  a = 1.0  B pvd;             \
  BOOST_CHECK(true);          \
}                             \
TEST_OP_COMPATIBLE_TYPE(A, B)


#ifdef TEST_COMPATIBLE_TYPE
#define TEST_OP_COMPARE_COMPATIBLE_TYPE(A, B) \
BOOST_AUTO_TEST_CASE(A##_compat_type)         \
{                                             \
  bool res;                                   \
  res = pvd B pvf;                            \
  res = pvd B 1.0f;                           \
  res = 1.0f B pvd;                           \
  res = pvf B 1.0;                            \
  res = 1.0 B pvf;                            \
  BOOST_CHECK(res || true);                   \
}
#else
#define TEST_OP_COMPARE_COMPATIBLE_TYPE(A, B)
#endif // TEST_COMPATIBLE_TYPE

#define TEST_OP_COMPARE(A, B)  \
BOOST_AUTO_TEST_CASE(A)        \
{                              \
  bool res;                    \
  res = pvd B pvd;             \
  res = pvd B 1.0;             \
  res = 1.0 B pvd;             \
  res = pvf B 1.0;             \
  BOOST_CHECK(res || true);    \
} \
TEST_OP_COMPARE_COMPATIBLE_TYPE(A,B)

#ifdef TEST_COMPATIBLE_TYPE
#define TEST_MATH_ONE_ARG_COMPATIBLE_TYPE(A) \
BOOST_AUTO_TEST_CASE(_##A##_compat_type)     \
{                                            \
  VD a;                                      \
  a = A(pvf);                                \
  BOOST_CHECK(true);                         \
}
#else
#define TEST_MATH_ONE_ARG_COMPATIBLE_TYPE(A)
#endif // TEST_COMPATIBLE_TYPE

#define TEST_MATH_ONE_ARG(A)           \
BOOST_AUTO_TEST_CASE(_##A)             \
{                                      \
  VD a;                                \
  a = A(pvd);                          \
  BOOST_CHECK(true);                   \
}                                      \
TEST_MATH_ONE_ARG_COMPATIBLE_TYPE(A)


#ifdef TEST_COMPATIBLE_TYPE
#define TEST_MATH_TWO_ARGS_COMPATIBLE_TYPE(A) \
BOOST_AUTO_TEST_CASE(_##A##_compat_type)      \
{                                             \
  VD a;                                       \
  a = A(pvd,pvf);                             \
  a = A(pvf,pvd);                             \
  a = A(1.0f,pvd);                            \
  a = A(pvd,1.0f);                            \
  a = A(1.0,pvf);                             \
  a = A(pvf,1.0);                             \
  BOOST_CHECK(true);                          \
}
#else
#define TEST_MATH_TWO_ARGS_COMPATIBLE_TYPE(A)
#endif // TEST_COMPATIBLE_TYPE

#define TEST_MATH_TWO_ARGS(A) \
BOOST_AUTO_TEST_CASE(_##A)    \
{                             \
  VD a;                       \
  a = A(pvd,pvd);             \
  a = A(pvd,1.0);             \
  a = A(1.0,pvd);             \
  BOOST_CHECK(true);          \
} \
TEST_MATH_TWO_ARGS_COMPATIBLE_TYPE(A)

