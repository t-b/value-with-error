#pragma once

namespace error_propagation
{
  class PolicyMock
  {
  public:
    template<typename U, typename V, typename P>
    CONST static bool Equal(const ValueWithError<U,P> &lhs, const ValueWithError<V,P> &rhs) { return false; };
    template<typename U, typename V, typename P>
    CONST static bool Equal(const ValueWithError<U,P> &lhs, const V &rhs) { return false; };
    template<typename U, typename V, typename P>
    CONST static bool Equal(const U &lhs, const ValueWithError<V,P> &rhs) { return false; };
    template<typename U, typename V, typename P>
    CONST static bool GreaterThan(const ValueWithError<U,P> &lhs, const ValueWithError<V,P> &rhs) { return false; };
    template<typename U, typename V, typename P>
    CONST static bool GreaterThan(const ValueWithError<U,P> &lhs, const V &rhs) { return false; };
    template<typename U, typename V, typename P>
    CONST static bool GreaterThan(const U &lhs, const ValueWithError<V,P> &rhs) { return false; };
    template<typename U, typename V, typename P>
    CONST static bool GreaterOrEqual(const ValueWithError<U,P> &lhs, const ValueWithError<V,P> &rhs) { return false; };
    template<typename U, typename V, typename P>
    CONST static bool GreaterOrEqual(const ValueWithError<U,P> &lhs, const V &rhs) { return false; };
    template<typename U, typename V, typename P>
    CONST static bool GreaterOrEqual(const U &lhs, const ValueWithError<V,P> &rhs) { return false; };
  };
} // namespace error_propagation
