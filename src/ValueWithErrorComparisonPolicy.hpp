#ifndef VALUE_WITH_ERROR_COMPARISONPOLICY_HPP
#define VALUE_WITH_ERROR_COMPARISONPOLICY_HPP

#include "DetailValueWithError.hpp"
#include "fwd.hpp"

namespace error_propagation {

  /// Simple comparison based on the values only and ignoring errors
  class ExactValueAndIgnoreErrorPolicy
  {
  public:
    template<typename U, typename V, typename P>
    CONST static bool Equal(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
    {
      return lhs.GetValue() == rhs.GetValue();
    }
    template<typename U, typename V, typename P>
    CONST static bool Equal(const ValueWithError<U, P>& lhs, const V& rhs)
    {
      return lhs.GetValue() == rhs;
    }
    template<typename U, typename V, typename P>
    CONST static bool Equal(const U& lhs, const ValueWithError<V, P>& rhs)
    {
      return lhs == rhs.GetValue();
    }
    template<typename U, typename V, typename P>
    CONST static bool GreaterThan(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
    {
      return lhs.GetValue() > rhs.GetValue();
    }
    template<typename U, typename V, typename P>
    CONST static bool GreaterThan(const ValueWithError<U, P>& lhs, const V& rhs)
    {
      return lhs.GetValue() > rhs;
    }
    template<typename U, typename V, typename P>
    CONST static bool GreaterThan(const U& lhs, const ValueWithError<V, P>& rhs)
    {
      return lhs > rhs.GetValue();
    }
    template<typename U, typename V, typename P>
    CONST static bool GreaterOrEqual(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
    {
      return lhs.GetValue() >= rhs.GetValue();
    }
    template<typename U, typename V, typename P>
    CONST static bool GreaterOrEqual(const ValueWithError<U, P>& lhs, const V& rhs)
    {
      return lhs.GetValue() >= rhs;
    }
    template<typename U, typename V, typename P>
    CONST static bool GreaterOrEqual(const U& lhs, const ValueWithError<V, P>& rhs)
    {
      return lhs >= rhs.GetValue();
    }

  private:
    ExactValueAndIgnoreErrorPolicy();
    ~ExactValueAndIgnoreErrorPolicy();
  };

  /// Compare two values with respect to their errors
  class CompareWithinErrorIntervalsPolicy
  {
  public:
    template<typename U, typename V, typename P>
    CONST static bool Equal(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
    {
      using std::abs;
      using std::max;
      typedef typename detail::promote_args<U, V>::type R;
      return (abs(lhs.GetValue() - rhs.GetValue()) <= max<R>(lhs.GetError(), rhs.GetError()));
    }
    template<typename U, typename V, typename P>
    CONST static bool Equal(const ValueWithError<U, P>& lhs, const V& rhs)
    {
      using std::abs;
      return (abs(lhs.GetValue() - rhs) <= lhs.GetError());
    }
    template<typename U, typename V, typename P>
    CONST static bool Equal(const U& lhs, const ValueWithError<V, P>& rhs)
    {
      using std::abs;
      return (abs(lhs - rhs.GetValue()) <= rhs.GetError());
    }
    template<typename U, typename V, typename P>
    CONST static bool GreaterThan(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
    {
      using std::max;
      typedef typename detail::promote_args<U, V>::type R;
      return ((lhs.GetValue() - rhs.GetValue()) > max<R>(lhs.GetError(), rhs.GetError()));
    }
    template<typename U, typename V, typename P>
    CONST static bool GreaterThan(const ValueWithError<U, P>& lhs, const V& rhs)
    {
      return ((lhs.GetValue() - rhs) > lhs.GetError());
    }
    template<typename U, typename V, typename P>
    CONST static bool GreaterThan(const U& lhs, const ValueWithError<V, P>& rhs)
    {
      return ((lhs - rhs.GetValue()) > rhs.GetError());
    }
    template<typename U, typename V, typename P>
    CONST static bool GreaterOrEqual(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
    {
      using std::max;
      typedef typename detail::promote_args<U, V>::type R;
      return ((lhs.GetValue() - rhs.GetValue()) >= -max<R>(lhs.GetError(), rhs.GetError()));
    }
    template<typename U, typename V, typename P>
    CONST static bool GreaterOrEqual(const ValueWithError<U, P>& lhs, const V& rhs)
    {
      return ((lhs.GetValue() - rhs) >= -lhs.GetError());
    }
    template<typename U, typename V, typename P>
    CONST static bool GreaterOrEqual(const U& lhs, const ValueWithError<V, P>& rhs)
    {
      return ((lhs - rhs.GetValue()) >= -rhs.GetError());
    }

  private:
    CompareWithinErrorIntervalsPolicy();
    ~CompareWithinErrorIntervalsPolicy();
  };

} // namespace error_propagation

#endif // VALUE_WITH_ERROR_COMPARISONPOLICY_HPP
