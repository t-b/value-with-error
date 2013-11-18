#ifndef DETAIL_VALUE_WITH_ERROR_HPP
#define DETAIL_VALUE_WITH_ERROR_HPP

#include <cmath>
#include <iostream>
#include <iomanip>
#include <boost/math/special_functions/digamma.hpp>
#include <boost/math/special_functions/hypot.hpp>
#include <boost/math/tools/promotion.hpp>
#include <boost/type_traits/has_less.hpp>

#include "fwd.hpp" // for DEFAULT_POLICY_CLASS define

#if __cplusplus >= 201103L

  #include <type_traits>
  #include <initializer_list>

  // CONST is being mapped to constexpr (C++11) or const (C++98)
  #ifdef BOOST_NO_CXX11_CONSTEXPR
    #define CONSTEXPR
    #define CONST const
  #else
    #define CONST constexpr
    #define CONSTEXPR constexpr
  #endif
#else
  #include <boost/type_traits.hpp>
  #define CONST const
  #define CONSTEXPR
#endif // __cplusplus >= 201103L

namespace error_propagation {
  namespace detail {

    using boost::math::tools::promote_args;

    typedef boost::math::policies::policy<
                                          boost::math::policies::domain_error<boost::math::policies::errno_on_error>,
                                          boost::math::policies::pole_error<boost::math::policies::errno_on_error>,
                                          boost::math::policies::overflow_error<boost::math::policies::errno_on_error>,
                                          boost::math::policies::evaluation_error<boost::math::policies::errno_on_error>
                                         > policy_errno_on_error;

    // Only use hypot for types which support std::less
    template<typename T, typename U, bool has_less_op>
    struct hypot_impl;

    template<typename T, typename U>
    struct hypot_impl<T, U, false>
    {
      CONSTEXPR typename promote_args<T, U>::type
      operator()(const T& a, const U& b) const
      {
        using std::sqrt;
        return sqrt(a*a + b*b);
      }
    };

    template<typename T, typename U>
    struct hypot_impl<T, U, true>
    {
      CONSTEXPR typename promote_args<T, U>::type
      operator()(const T& a, const U& b) const
      {
        // we don't use std::hypot here as that does not know how to work with boost::multiprecision
        return boost::math::hypot(a,b,policy_errno_on_error());
      }
    };

    template<typename T, typename U>
    CONSTEXPR typename promote_args<T,U>::type
    hypot(const T& a, const U& b)
    {
      return hypot_impl<T, U, boost::has_less<T,U,bool>::value>()(a,b);
    }

    template<typename T>
    CONSTEXPR T digamma(const T& a)
    {
      return boost::math::digamma(a,policy_errno_on_error());
    }

  } // namespace detail
} // namespace error_propagation

#endif // DETAIL_VALUE_WITH_ERROR_HPP
