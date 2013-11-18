#ifndef VALUE_WITH_ERROR_HPP
#define VALUE_WITH_ERROR_HPP

#include "../DetailValueWithError.hpp"
#include "../ValueWithErrorComparisonPolicy.hpp"

namespace error_propagation {

  using std::abs; // required so that ADL works in the ctor initializer list

  template<typename T, typename P>
  void swap(ValueWithError<T, P>& lhs, ValueWithError<T, P>& rhs)
  {
    lhs.swap(rhs);
  }

  /** @brief Template class supporting gaussian error propagation
   *
   * The underlying type T must fullfill the following properties:
   *  <ul>
   *  <li> DefaultConstructible
   *  <li> CopyConstructible
   *  <li> Arithmetic Operations /+-*
   *  </ul>
   * In case you want to compare ValueWithError objects, the underlying type T must support the comparison operators <,>,<=,>=,==,!=
   * and the comparison must return a type convertible to bool.
   *
   * Examples for T: double, float, boost::multiprecision::cpp_dec50/100, std::valarray, Eigen::Array
   *
   * The underlying type P implements the comparison policy. See ExactValueAndIgnoreErrorPolicy and CompareWithinErrorIntervalsPolicy which are already provided. The default policy is ExactValueAndIgnoreErrorPolicy, and can be changed with
   * @code{cpp}
     #define DEFAULT_POLICY_CLASS error_propagation::MyPolicyClass
     #include "ValueWithError.hpp"
     @endcode
   *
   * @tparam T  Arithmetic type, must support all basic arithmetic operations
   * @tparam P  Policy class for comparing values and errors
   */
  template<typename T, typename P>
  class ValueWithError
  {
    public:
    typedef T value_type;
    typedef P policy_type;

    ///@name Constructor
    ///@{
    CONSTEXPR explicit ValueWithError(const T& value, const T& error)
      :
      m_value{value},
      m_error{abs(error)}
    {}

    CONSTEXPR explicit ValueWithError(T&& value, const T& error)
      :
      m_value{std::move(value)},
      m_error{abs(error)}
    {}

    CONSTEXPR explicit ValueWithError(const T& value, T&& error)
      :
      m_value{value},
      m_error{abs(std::move(error))}
    {}

    CONSTEXPR explicit ValueWithError(T&& value, T&& error)
      :
      m_value{std::move(value)},
      m_error{abs(std::move(error))}
    {}

    CONSTEXPR explicit ValueWithError(T&& value)
      :
      m_value{std::move(value)},
      m_error{}
    {}

    CONSTEXPR explicit ValueWithError(const T& value)
      :
      m_value{value},
      m_error{}
    {}

    CONSTEXPR ValueWithError()
      :
      m_value{},
      m_error{}
    {}

    /** @brief Constructor for bracket initialization
     *
     * <ul>
     * <li>First  list entry -> value
     * <li>Second list entry -> error
     * <li>All later list elements are silently skipped
     * </ul>
     */
    CONSTEXPR ValueWithError(std::initializer_list<T> list)
      :
      m_value{list.size() > 0 ? list.begin()[0] : T()},
      m_error{list.size() > 1 ? list.begin()[1] : T()}
    {
      // static_assert(list.size() <= 2,...) is only available for CPP_14
    }
    ///@}

    ///@name Move assignment
    ///@{
    ValueWithError<T, P>& operator=(ValueWithError<T, P>&& rhs)
    {
      if(this == &rhs)
      {
        return *this;
      }

      this->m_value = std::move(rhs.m_value);
      this->m_error = std::move(rhs.m_error);

      return *this;
    }
    ///@}

    ///@name Copy assignment
    ///@{
    ValueWithError<T, P>& operator=(const ValueWithError<T, P>& rhs)
    {
      this->m_value = rhs.m_value;
      this->m_error = rhs.m_error;

      return *this;
    }

    template<typename U>
    ValueWithError<T, P>& operator=(const ValueWithError<U, P>& rhs)
    {
      this->m_value = rhs.GetValue();
      this->m_error = rhs.GetError();

      return *this;
    }

    template<typename U>
    ValueWithError<T, P>& operator=(const U& other)
    {
      this->m_value = other;
      this->m_error = U();

      return *this;
    }

    /**
     * <ul>
     * <li>First  list entry -> value
     * <li>Second list entry -> error
     * <li>All later list elements are silently skipped
     * </ul>
     */
    template<typename U>
    ValueWithError<T, P>& operator=(std::initializer_list<U> list)
    {
      // add static_assert for CPP14
      m_value = list.size() > 0 ? list.begin()[0] : T();
      m_error = list.size() > 1 ? list.begin()[1] : T();

      return *this;
    }
    ///@}

    CONSTEXPR ValueWithError(const ValueWithError<T, P>& rhs)
      :
      m_value{rhs.m_value},
      m_error{rhs.m_error}
    {}

    CONSTEXPR ValueWithError(ValueWithError<T, P>&& rhs)
      :
      m_value{std::move(rhs.m_value)},
      m_error{std::move(rhs.m_error)}
    {}

    template<typename U>
    CONSTEXPR ValueWithError(const ValueWithError<U, P>& rhs)
      :
      m_value{rhs.GetValue()},
      m_error{rhs.GetError()}
    {}

    ///@name Getters
    ///@{
    CONSTEXPR inline const T& GetValue() const
    {
      return m_value;
    }

    CONSTEXPR inline const T& GetError() const
    {
      return m_error;
    }
    ///@}

    ///@name Arithmetic operators
    ///@{

    ///@see operator*(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
    template<typename U>
    ValueWithError<T, P>&
    operator*=(const ValueWithError<U, P>& rhs)
    {
      *this = (*this) * rhs;
      return *this;
    }

    ///@see operator*(const ValueWithError<U, P>& lhs, const V& rhs)
    template<typename U>
    ValueWithError<T, P>&
    operator*=(const U& rhs)
    {
      *this = (*this) * rhs;
      return *this;
    }

    ///@see operator/(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
    template<typename U>
    ValueWithError<T, P>&
    operator/=(const ValueWithError<U, P>& rhs)
    {
      *this = (*this) / rhs;
      return *this;
    }

    ///@see operator/(const ValueWithError<U, P>& lhs, const V& rhs)
    template<typename U>
    ValueWithError<T, P>&
    operator/=(const U& rhs)
    {
      *this = (*this) / rhs;
      return *this;
    }

    ///@see operator+(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
    template<typename U>
    ValueWithError<T, P>&
    operator+=(const ValueWithError<U, P>& rhs)
    {
      *this = (*this) + rhs;
      return *this;
    }

    ///@see operator+(const ValueWithError<U, P>& lhs, const V& rhs)
    template<typename U>
    ValueWithError<T, P>&
    operator+=(const U& rhs)
    {
      *this = (*this) + rhs;
      return *this;
    }

    ///@see operator-(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
    template<typename U>
    ValueWithError<T, P>&
    operator-=(const ValueWithError<U, P>& rhs)
    {
      *this = (*this) - rhs;
      return *this;
    }

    ///@see operator-(const ValueWithError<U, P>& lhs, const V& rhs)
    template<typename U>
    ValueWithError<T, P>&
    operator-=(const U& rhs)
    {
      *this = (*this) - rhs;
      return *this;
    }
    ///@}

    void swap(ValueWithError<T, P>& obj)
    {
      using std::swap;
      swap(this->m_value, obj.m_value);
      swap(this->m_error, obj.m_error);
    }

  private:
    T m_value, m_error;
  };

  /// Helper function using template argument deduction
  template<typename T, typename U, typename P = DEFAULT_POLICY_CLASS>
  CONSTEXPR ValueWithError< typename detail::promote_args< typename std::decay<T>::type, typename std::decay<U>::type>::type, P>
  make_value(T&& value, U&& error)
  {
    typedef typename std::decay<T>::type TV;
    typedef typename std::decay<U>::type UV;
    typedef typename detail::promote_args<TV, UV>::type R;
    return ValueWithError<R, P>(std::forward<T>(value), std::forward<U>(error));
  }

  /**@name Arithmetic operator definitions
   * The following operators are not defined as they don't have a natural definition:
   *
   * %, bitwise, increment, decrement
   *@{
   */

  /// Product.
  /**
   *  *Computation:* @f$ [v_x\pm e_x] \cdot [v_y\pm e_y] = \left[ v_x \cdot v_y\pm \sqrt{\left( v_y \cdot e_x \right)^2 + \left( v_x \cdot e_y \right)^2} \right] @f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<U, V>::type, P>
  operator*(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                lhs.GetValue() * rhs.GetValue(),
                                detail::hypot<R>(rhs.GetValue() * lhs.GetError(),lhs.GetValue() * rhs.GetError())
                               );
  }

  /// Product.
  /**
   *  *Computation:* @f$ [v_x\pm e_x] \cdot v_y = \left[ v_x \cdot v_y\pm v_y \cdot e_x \right] @f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<U, V>::type, P>
  operator*(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                lhs.GetValue() * rhs,
                                lhs.GetError() * rhs
                               );
  }

  /// Product.
  /**
   *  *Computation:* @f$ v_x \cdot [v_y\pm e_y] = \left[ v_x \cdot v_y\pm v_x \cdot e_y \right] @f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<V, U>::type, P>
  operator*(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    typedef typename detail::promote_args<V, U>::type R;
    return ValueWithError<R, P>(
                                rhs.GetValue() * lhs,
                                rhs.GetError() * lhs
                               );
  }

  /// Division.
  /**
   *  *Computation:* @f$ [v_x\pm e_x] / [v_y\pm e_y] = \left[ v_x / v_y\pm \sqrt{ \left(e_x / v_y\right)^2 + \left( e_y \cdot v_x / v_y^2 \right)^2 } \right]  = \left[ v_x / v_y\pm \textrm{hypot}\left(e_x, e_y \cdot v_x/v_y\right) \left/v_y\right. \right] @f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<U, V>::type, P>
  operator/(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                lhs.GetValue() / rhs.GetValue(),
                                detail::hypot( lhs.GetError(), lhs.GetValue() / rhs.GetValue() * rhs.GetError()) / rhs.GetValue()
                               );
  }

  /// Division.
  /**
   *  *Computation:* @f$ [v_x\pm e_x] / v_y = \left[ v_x / v_y\pm e_x / v_y \right] @f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<U, V>::type, P>
  operator/(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                lhs.GetValue() / rhs,
                                lhs.GetError() / rhs
                               );
  }

  /// Division.
  /**
   *  *Computation:* @f$ v_x / [v_y\pm e_y] = \left[ v_x / v_y\pm v_x \cdot e_y /v_y^2 \right] @f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<V, U>::type, P>
  operator/(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    typedef typename detail::promote_args<V, U>::type R;
    return ValueWithError<R, P>(
                                lhs / rhs.GetValue(),
                                (lhs * rhs.GetError()) / ( rhs.GetValue() * rhs.GetValue())
                               );
  }

  /// Addition.
  /**
   *  *Computation:* @f$ [v_x\pm e_x] + [v_y\pm e_y] = \left[ v_x + v_y\pm \sqrt{\left( e_x^2 + e_y^2\right)} \right] = \left[ v_x - v_y\pm \textrm{hypot}(e_x,e_y)\right] @f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<U, V>::type, P>
  operator+(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                lhs.GetValue() + rhs.GetValue(),
                                detail::hypot(lhs.GetError(), rhs.GetError())
                               );
  }

  /// Addition.
  /**
   *  *Computation:* @f$ [v_x\pm e_x] + v_y = \left[ v_x + v_y\pm e_x \right] @f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<U, V>::type, P>
  operator+(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                lhs.GetValue() + rhs,
                                lhs.GetError()
                               );
  }

  /// Addition.
  /**
   *  *Computation:* @f$ v_x + [v_y\pm e_x] = \left[ v_x + v_y\pm e_y \right] @f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<V, U>::type, P>
  operator+(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    typedef typename detail::promote_args<V, U>::type R;
    return ValueWithError<R, P>(
                                rhs.GetValue() + lhs,
                                rhs.GetError()
                               );
  }

  /// Subtraction.
  /**
   *  *Computation:* @f$ [v_x\pm e_x] - [v_y\pm e_y] = \left[ v_x - v_y\pm \sqrt{\left( e_x^2 + e_y^2\right)} \right] = \left[ v_x - v_y\pm \textrm{hypot}(e_x,e_y)\right] @f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<U, V>::type, P>
  operator-(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                lhs.GetValue() - rhs.GetValue(),
                                detail::hypot(lhs.GetError(), rhs.GetError())
                               );
  }

  /// Subtraction.
  /**
   *  *Computation:* @f$ [v_x\pm e_x] - v_y = \left[ v_x - v_y\pm e_x \right] @f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<U, V>::type, P>
  operator-(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                lhs.GetValue() - rhs,
                                lhs.GetError()
                               );
  }

  /// Subtraction.
  /**
   *  *Computation:* @f$ v_x - [v_y\pm e_x] = \left[ v_x - v_y\pm e_y \right] @f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<V, U>::type, P>
  operator-(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    typedef typename detail::promote_args<V, U>::type R;
    return ValueWithError<R, P>(
                                lhs - rhs.GetValue(),
                                rhs.GetError()
                               );
  }
  ///@}

  /** @name Comparison operator definitions
   *
   * All implementations here call internally AbstractPolicy.
   *
   * The negation operator (!) is not defined.
   *@{
   */
  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator==(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    return P::Equal(lhs, rhs);
  }

  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator==(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    return P::Equal(lhs, rhs);
  }

  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator==(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    return P::Equal(lhs,rhs);
  }


  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator!=(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    return ( ! (lhs==rhs) );
  }

  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator!=(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    return ( ! (lhs==rhs) );
  }

  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator!=(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    return ( ! (lhs==rhs) );
  }


  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator<(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    return (rhs > lhs);
  }

  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator<(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    return (rhs > lhs);
  }

  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator<(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    return (rhs > lhs);
  }


  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator>(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    return P::GreaterThan(lhs, rhs);
  }

  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator>(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    return P::GreaterThan(lhs, rhs);
  }

  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator>(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    return P::GreaterThan(lhs, rhs);
  }


  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator<=(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    return (rhs >= lhs);
  }

  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator<=(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    return (rhs >= lhs);
  }

  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator<=(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    return (rhs >= lhs);
  }


  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator>=(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    return P::GreaterOrEqual(lhs, rhs);
  }

  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator>=(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    return P::GreaterOrEqual(lhs, rhs);
  }

  template<typename U, typename V, typename P>
  CONSTEXPR bool
  operator>=(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    return P::GreaterOrEqual(lhs, rhs);
  }
  ///@}

  /** @name Conversion operators
   *
   * Output format: "(value+-error)"<br>
   * On input additional whitespace is silently skipped.
   *@{
   */
  template<typename charT, typename traits, typename T, typename P>
  std::basic_ostream<charT,traits>&
  operator<<(std::basic_ostream<charT,traits>& out, const ValueWithError<T, P>& v)
  {
    // use a temporary stringstream to honour width
    std::basic_ostringstream<charT,traits> sstr;
    sstr.copyfmt(out);
    sstr.width(0);

    sstr << '(' << v.GetValue() << "+-" << v.GetError() << ')';

    out << sstr.str();

    return out;
  }

  template<typename charT, typename traits, typename T, typename P>
  std::basic_istream<charT,traits>&
  operator>>(std::basic_istream<charT,traits>& in, ValueWithError<T, P>& v)
  {
    in >> std::ws;
    if( in.peek() == '(' )
    {
      in.ignore();
      T value;
      in >> value;

      in >> std::ws;
      if( in.peek() == '+' )
      {
        in.ignore();

        in >> std::ws;
        if( in.peek() == '-' )
        {
          in.ignore();
          T error;
          in >> error;

          in >> std::ws;
          if( in.peek() == ')' )
          {
            in.ignore();

            if(!in.fail())
            {
              v = ValueWithError<T, P>(value, error);
            }

            return in;
          }
        }
      }
    }

    in.setstate(std::ios::failbit);

    return in;
  }
  ///@}

  /**@name Mathematical function overloads
   *
   *not overloaded: ceil, copysign, fma, fmax, fmin, fmod, frexp,
   *                ilogb, ldexp, llrint, llround, logb, lrint, lround,
   *                modf, nan, nanf, nanl, nearbyint, nextafter, nexttoward,
   *                remainder, remquo, rint, round, scalbln, scalbn, trunc,
   *                fpclassify, isgreaterequal islessequal, isnan, isunordered,
   *                isfinite, isinf, islessgreater isnormal, signbit, isgreater,
   *                isless
   *
   *Nomenclature:* In mathematical expressions values with errors will be denoted in the form \f$[v\pm e]\f$
   *with \f$ e>0 \f$, i.e. the absolute value of errors is implicitly taken in mathematical formulas.
   *@{
   */

  /// Absolute value.
  /**
   *  *Computation:* \f$ \textrm{abs}([v\pm e]) = [\|v\| \pm e] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  abs(const ValueWithError<T, P>& v)
  {
    return ValueWithError<T, P>(
                                abs(v.GetValue()),
                                v.GetError()
                               );
  }

  /// Arc cosine.
  /**
   *  *Computation:* \f$ acos([v\pm e]) = [acos(v) \pm 1\left/\sqrt{1-v^2}\right.e] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  acos(const ValueWithError<T, P>& v)
  {
    using std::acos;
    using std::sqrt;
    return ValueWithError<T, P>(
                                acos(v.GetValue()),
                                1.0 / sqrt(1.0 - v.GetValue() * v.GetValue()) * v.GetError()
                               );
  }

  /// Arc sine.
  /**
   *  *Computation:* \f$ asin([v\pm e]) = [asin(v) \pm 1\left/\sqrt{1-v^2}\right.e] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  asin(const ValueWithError<T, P>& v)
  {
    using std::asin;
    using std::sqrt;
    return ValueWithError<T, P>(
                                asin(v.GetValue()),
                                1.0 / sqrt(1.0 - v.GetValue() * v.GetValue()) * v.GetError()
                               );
  }

  /// Arc tangent.
  /**
   *  *Computation:* \f$ atan([v\pm e]) = [atan(v) \pm 1\left/\left(1+v^2\right)\right.e] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  atan(const ValueWithError<T, P>& v)
  {
    using std::atan;
    return ValueWithError<T, P>(
                                atan(v.GetValue()),
                                1.0 / (1.0 + v.GetValue() * v.GetValue()) * v.GetError()
                               );
  }

  /// Arc tangent, using signs to determine quadrants. Overload for two ValueWithError arguments.
  /**
   *  *Computation:* \f$ atan2([v_y\pm e_y], [v_x\pm e_x]) = [atan2(v_y, v_x) \pm \sqrt{((v_ye_x)^2+(v_xe_y)^2}\left/\left(v_y^2+v_x^2\right)\right.] \f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<U, V>::type, P>
  atan2(const ValueWithError<U, P>& y, const ValueWithError<V, P>& x)
  {
    using std::atan2;
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                atan2(y.GetValue(), x.GetValue()),
                                1.0 / (y.GetValue() * y.GetValue() + x.GetValue() * x.GetValue()) *
                                detail::hypot(y.GetValue() * x.GetError(), x.GetValue() * y.GetError())
                               );
  }

  /// Arc tangent, using signs to determine quadrants. Overload for a general compatible first argument and a ValueWithError as second argument.
  /**
   *  *Computation:* \f$ atan2(y, [v_x\pm e_x]) = [atan2(y, v_x) \pm ye_x\left/\left(y^2+v_x^2\right)\right.] \f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<U, V>::type, P>
  atan2(const U& y, const ValueWithError<V, P>& x)
  {
    using std::atan2;
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                atan2(y, x.GetValue()),
                                1.0 / (y * y + x.GetValue() * x.GetValue()) * y * x.GetError()
                               );
  }

  /// Arc tangent, using signs to determine quadrants. Overload for a ValueWithError as first argument and a general compatible second argument.
  /**
   *  *Computation:* \f$ atan2([v_y\pm e_y], y) = [atan2(v_y, x) \pm xe_y\left/\left(v_y^2+x^2\right)\right.] \f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<U, V>::type, P>
  atan2(const ValueWithError<U, P>& y, const V& x)
  {
    using std::atan2;
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                atan2(y.GetValue(), x),
                                1.0 / (y.GetValue() * y.GetValue() + x * x) * x * y.GetError()
                               );
  }

  /// Hyperbolic arc sine.
  /**
   *  *Computation:* \f$ asinh([v\pm e]) = [asinh(v) \pm 1\left/\sqrt{1+v^2}\right.e] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  asinh(const ValueWithError<T, P>& v)
  {
    using std::asinh;
    using std::sqrt;
    return ValueWithError<T, P>(
                                asinh(v.GetValue()),
                                1.0 / sqrt(1.0 + v.GetValue() * v.GetValue()) * v.GetError()
                               );
  }

  /// Hyperbolic arc cosine.
  /**
   *  *Computation:* \f$ acosh([v\pm e]) = [acosh(v) \pm 1\left/\sqrt{v^2-1}\right.e] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  acosh(const ValueWithError<T, P>& v)
  {
    using std::acosh;
    using std::sqrt;
    return ValueWithError<T, P>(
                                acosh(v.GetValue()),
                                1.0 / sqrt(v.GetValue() * v.GetValue() - 1.0) * v.GetError()
                               );
  }

  /// Hyperbolic arc tangent.
  /**
   *  *Computation:* \f$ atanh([v\pm e]) = [atanh(v) \pm 1\left/\left(1-v^2\right)\right.e] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  atanh(const ValueWithError<T, P>& v)
  {
    using std::atanh;
    return ValueWithError<T, P>(
                                atanh(v.GetValue()),
                                1.0 / (1.0 - v.GetValue() * v.GetValue()) * v.GetError()
                               );
  }

  /// Cubic root.
  /**
   *  *Computation:* \f$ \sqrt[3]{[v\pm e]} = [\sqrt[3]{v} \pm 1\left/\left(3\sqrt[3]{v^2}\right)\right.e] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  cbrt(const ValueWithError<T, P>& v)
  {
    using std::cbrt;
    return ValueWithError<T, P>(
                                cbrt(v.GetValue()),
                                1.0/(3.0 * cbrt(v.GetValue() * v.GetValue())) * v.GetError()
                               );
  }

  /// Cosine.
  /**
   *  *Computation:* \f$ \cos([v\pm e]) = [\cos(v) \pm \sin(v)e] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  cos(const ValueWithError<T, P>& v)
  {
    using std::cos;
    using std::sin;
    return ValueWithError<T, P>(
                                cos(v.GetValue()),
                                sin(v.GetValue())*v.GetError()
                               );
  }

  /// Hyperbolic cosine.
  /**
   *  *Computation:* \f$ \cosh([v\pm e]) = [\cosh(v) \pm \sinh(v)e] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  cosh(const ValueWithError<T, P>& v)
  {
    using std::cosh;
    using std::sinh;
    return ValueWithError<T, P>(
                                cosh(v.GetValue()),
                                sinh(v.GetValue()) * v.GetError()
                               );
  }

  /// The error function.
  /**
   *  *Computation:* \f$ \textrm{erf}([v\pm e]) = [\textrm{erf}(v) \pm 2\exp(-v^2)e\left/\sqrt{\pi}\right.] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  erf(const ValueWithError<T, P>& v)
  {
    using std::erf;
    using std::exp;
    using boost::math::constants::root_pi;
    return ValueWithError<T, P>(
                                erf(v.GetValue()),
                                2.0 / root_pi<T>() * exp(-v.GetValue() * v.GetValue()) * v.GetError()
                               );
  }

  /// The complementary error function.
  /**
   *  *Computation:* \f$ \textrm{erfc}([v\pm e]) = [\textrm{erfc}(v) \pm 2\exp(-v^2)e\left/\sqrt{\pi}\right.] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  erfc(const ValueWithError<T, P>& v)
  {
    using std::erfc;
    using std::exp;
    using boost::math::constants::root_pi;
    return ValueWithError<T, P>(
                                erfc(v.GetValue()),
                                2.0 / root_pi<T>() * exp(-v.GetValue() * v.GetValue()) * v.GetError()
                               );
  }

  /// 2 raised to a given power.
  /**
   *  *Computation:* \f$ \textrm{exp2}([v\pm e]) = [\textrm{exp2}(v) \pm \ln(2)\textrm{exp2}(v)e] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  exp2(const ValueWithError<T, P>& v)
  {
    using std::exp2;
    using boost::math::constants::ln_two;
    return ValueWithError<T, P>(
                                exp2(v.GetValue()),
                                ln_two<T>() * exp2(v.GetValue()) * v.GetError()
                               );
  }

  /// Euler's number raised to a given exponential.
  /**
   *  *Computation:* \f$ \exp([v\pm e]) = [\exp(v) \pm \exp(v)e] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  exp(const ValueWithError<T, P>& v)
  {
    using std::exp;
    return ValueWithError<T, P>(
                                exp(v.GetValue()),
                                exp(v.GetValue()) * v.GetError()
                               );
  }

  /// Euler's number raised to a given exponential and shifted by -1.
  /**
   *  *Computation:* \f$ \textrm{expm1}([v\pm e]) = [\textrm{expm1}(v) \pm \exp(v)e] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  expm1(const ValueWithError<T, P>& v)
  {
    using std::expm1;
    using std::exp;
    return ValueWithError<T, P>(
                                expm1(v.GetValue()),
                                exp(v.GetValue()) * v.GetError()
                               );
  }

  /// Absolute value.
  /**
   *  *Computation:* \f$ \textrm{fabs}([v\pm e]) = [\|v\| \pm e] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  fabs(const ValueWithError<T, P>& v)
  {
    using std::fabs;
    return ValueWithError<T, P>(
                                fabs(v.GetValue()),
                                v.GetError()
                               );
  }

  /// Square root of the sum of squares of two numbers. Overload for two arguments of ValueWithError type.
  /**
   *  *Computation:* \f$ \textrm{hypot}([v_x\pm e_x], [v_y\pm e_y]) = [\textrm{hypot}(v_x, v_y) \pm \textrm{hypot}(v_xe_x, v_ye_y)\left/\left(v_x^2+v_y^2\right)\right.] \f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<U, V>::type, P>
  hypot(const ValueWithError<U, P>& x, const ValueWithError<V, P>& y)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                detail::hypot(x.GetValue(), y.GetValue()),
                                detail::hypot(x.GetValue() * x.GetError(), y.GetValue() * y.GetError())
                                / (x.GetValue() * x.GetValue() + y.GetValue() * y.GetValue())
                               );
  }

  /// Square root of the sum of squares of two numbers. Overload for one argument of a general compatible type and one argument of ValueWithError type.
  /**
   *  *Computation:* \f$ \textrm{hypot}(x, [v_y\pm e_y]) = [\textrm{hypot}(x, v_y) \pm v_ye_y\left/\left(x^2+v_y^2\right)\right.] \f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<V, U>::type, P>
  hypot(const U& x, const ValueWithError<V, P>& y)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                detail::hypot(x, y.GetValue()),
                                y.GetValue() * y.GetError() / (x * x + y.GetValue() * y.GetValue())
                               );
  }

  /// Square root of the sum of squares of two numbers. Overload for one argument of ValueWithError and one argument of general compatible type.
  /**
   *  *Computation:* \f$ \textrm{hypot}([v_x\pm e_x], y) = [\textrm{hypot}(v_x, y) \pm v_xe_x\left/\left(v_x^2+y^2\right)\right.] \f$
   */
  template<typename U, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<U, V>::type, P>
  hypot(const ValueWithError<U, P>& x, const V& y)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                detail::hypot(x.GetValue(), y),
                                x.GetValue() * x.GetError() / (x.GetValue() * x.GetValue() + y * y)
                               );
  }

  /// Natural logarithm of the absolute value of the gamma function.
  /**
   *  *Computation:* \f$ \ln(\left|\Gamma([v\pm e])\right|) = [\ln(\left|\Gamma(v)\right|) \pm \Psi(v)e] \f$,
   *  using the "digamma" function \f$ \Psi(x) = \textrm{d}\ln(\Gamma(x))/dx = \Gamma'(x)\left/\Gamma(x)\right. \f$.
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  lgamma(const ValueWithError<T, P>& v)
  {
    // lgamma(x) = ln(abs(Gamma(x)))
    // lgamma'(x) = (ln(abs(Gamma(x))))' = abs'(Gamma(x))/abs(Gamma(x)) = (sign(Gamma(x))*abs(Gamma'(x))) / abs(Gamma(x)) = sign(Gamma(x))*abs(DiGamma(x))
    using std::lgamma;
    return ValueWithError<T, P>(
                                lgamma(v.GetValue()),
                                detail::digamma(v.GetValue()) * v.GetError()
                               );
  }

  /// Natural logarithm.
  /**
   *  *Computation:* \f$ \ln([v\pm e]) = [\ln(v) \pm e/v] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  log(const ValueWithError<T, P>& v)
  {
    using std::log;
    return ValueWithError<T, P>(
                                log(v.GetValue()),
                                1.0 / v.GetValue() * v.GetError()
                               );
  }

  /// Base ten logarithm.
  /**
   *  *Computation:* \f$ \textrm{log10}([v\pm e]) = [\textrm{log10}(v) \pm e\left/(v\ln(10))\right.] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  log10(const ValueWithError<T, P>& v)
  {
    using std::log10;
    using boost::math::constants::ln_ten;
    return ValueWithError<T, P>(
                                log10(v.GetValue()),
                                1.0 / ( v.GetValue() * ln_ten<T>()) * v.GetError()
                               );
  }

  /// Natural logarithm of "1 plus a number".
  /**
   *  *Computation:* \f$ \textrm{log1p}([v\pm e]) = [\textrm{log1p}(v) \pm e\left/(1+v)\right.] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  log1p(const ValueWithError<T, P>& v)
  {
    using std::log1p;
    return ValueWithError<T, P>(
                                log1p(v.GetValue()),
                                1.0 / ( 1.0 + v.GetValue() ) * v.GetError()
                               );
  }

  /// Base two logarithm.
  /**
   *  *Computation:* \f$ \textrm{log2}([v\pm e]) = [\textrm{log2}(v) \pm e\left/(v\ln(2))\right.] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  log2(const ValueWithError<T, P>& v)
  {
    using std::log2;
    using boost::math::constants::ln_two;
    return ValueWithError<T, P>(
                                log2(v.GetValue()),
                                1.0 / ( v.GetValue() * ln_two<T>() ) * v.GetError()
                               );
  }

  /// Raises a number to the given power. Overload for one argument of type ValueWithError and one of a general compatible type.
  /**
   *  *Computation:* \f$ \textrm{pow}([v_x\pm e_x], y) = [\textrm{pow}(v_x, y) \pm y\textrm{pow}(v_x, y-1)e] \f$
   */
  template<typename T, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<T, V>::type, P>
  pow(const ValueWithError<T, P>& base, const V& exponent)
  {
    using std::pow;
    typedef typename detail::promote_args<T, V>::type R;
    return ValueWithError<R, P>(
                                pow(base.GetValue(), exponent),
                                exponent*pow(base.GetValue(), exponent-1)*base.GetError()
                               );
  }

  /// Raises a number to the given power. Overload for one argument of a general compatible type and one of ValueWithError type.
  /**
   *  *Computation:* \f$ \textrm{pow}(x, [v_y\pm e_y]) = [\textrm{pow}(x, v_y) \pm \ln(x)\textrm{pow}(x, v_y)e] \f$
   */
  template<typename T, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<V, T>::type, P>
  pow(const V& base, const ValueWithError<T, P>& exponent)
  {
    using std::pow;
    using std::log;
    typedef typename detail::promote_args<V, T>::type R;
    return ValueWithError<R, P>(
                                pow(base, exponent.GetValue()),
                                log(base) * pow(base, exponent.GetValue()) * exponent.GetError()
                               );
  }

  /// Raises a number to the given power. Overload for two arguments of type ValueWithError.
  /**
   *  *Computation:* \f$ \textrm{pow}([v_x\pm e_x], [v_y \pm e_y]) = [\textrm{pow}(v_x, v_y) \pm \textrm{pow}(v_x, v_y)\sqrt{(e_xv_y/v_x)^2+(e_y\ln(v_x))^2}] \f$
   */
  template<typename T, typename V, typename P>
  CONSTEXPR ValueWithError<typename detail::promote_args<T, V>::type, P>
  pow(const ValueWithError<T, P>& base, const ValueWithError<V, P>& exponent)
  {
    using std::pow;
    using std::log;
    typedef typename detail::promote_args<T, V>::type R;
    return ValueWithError<R, P>(
                                pow(base.GetValue(), exponent.GetValue()),
                                pow(base.GetValue(), exponent.GetValue())
                                * detail::hypot<R,R>(
                                                     exponent.GetValue() / base.GetValue() * base.GetError(),
                                                     log(base.GetValue()) * exponent.GetError()
                                                    )
                              );
  }

  /// Sine.
  /**
   *  *Computation:* \f$ \sin([v\pm e]) = [\sin(v) \pm \cos(v)e] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  sin(const ValueWithError<T, P>& v)
  {
    using std::cos;
    using std::sin;
    return ValueWithError<T, P>(
                                sin(v.GetValue()),
                                cos(v.GetValue()) * v.GetError()
                               );
  }

  /// Hyperbolic sine.
  /**
   *  *Computation:* \f$ \sinh([v\pm e]) = [\sinh(v) \pm \cosh(v)e] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  sinh(const ValueWithError<T, P>& v)
  {
    using std::cosh;
    using std::sinh;
    return ValueWithError<T, P>(
                                sinh(v.GetValue()),
                                cosh(v.GetValue()) * v.GetError()
                               );
  }

  /// Square root.
  /**
   *  *Computation:* \f$ \sqrt{[v\pm e]} = [\sqrt{v} \pm 1\left/\left(2\sqrt{v}\right)\right.e] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  sqrt(const ValueWithError<T, P>& v)
  {
    using std::sqrt;
    return ValueWithError<T, P>(
                                sqrt(v.GetValue()),
                                0.5 / sqrt(v.GetValue()) * v.GetError()
                               );
  }

  /// Tangent.
  /**
   *  *Computation:* \f$ \tan{[v\pm e]} = [\tan{v} \pm e\left/\cos^2(v)\right.] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  tan(const ValueWithError<T, P>& v)
  {
    using std::cos;
    using std::tan;
    return ValueWithError<T, P>(
                                tan(v.GetValue()),
                                v.GetError() / (cos(v.GetValue()) * cos(v.GetValue()))
                               );
  }

  /// Hyperbolic tangent.
  /**
   *  *Computation:* \f$ \tanh{[v\pm e]} = [\tanh{v} \pm e\left/\cosh^2(v)\right.] \f$
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  tanh(const ValueWithError<T, P>& v)
  {
    using std::cosh;
    using std::tanh;
    return ValueWithError<T, P>(
                                tanh(v.GetValue()),
                                v.GetError() / (cosh(v.GetValue()) * cosh(v.GetValue()))
                               );
  }

  /// The gamma function.
  /**
   *  *Computation:* \f$ \Gamma([v\pm e]) = [\Gamma(v) \pm \Psi(v)\Gamma(v)e] \f$,
   *  using the "digamma" function \f$ \Psi(x) = \textrm{d}\ln(\Gamma(x))/dx = \Gamma'(x)\left/\Gamma(x)\right. \f$.
   */
  template<typename T, typename P>
  CONSTEXPR ValueWithError<T, P>
  tgamma(const ValueWithError<T, P>& v)
  {
    using std::tgamma;
    return ValueWithError<T, P>(
                                tgamma(v.GetValue()),
                                detail::digamma(v.GetValue()) * tgamma(v.GetValue()) * v.GetError()
                               );

  }
  ///@}

  ///@defgroup invalidSpecializations Empty partial specializations for invalid types
  ///@{
  template<typename T, typename P>
  class ValueWithError<std::complex<T>, P> {};

  template<typename P>
  class ValueWithError<short, P> {};
  template<typename P>
  class ValueWithError<unsigned short, P> {};
  template<typename P>
  class ValueWithError<const unsigned short, P> {};

  template<typename P>
  class ValueWithError<int, P> {};
  template<typename P>
  class ValueWithError<unsigned int, P> {};
  template<typename P>
  class ValueWithError<const unsigned int, P> {};

  template<typename P>
  class ValueWithError<long, P> {};
  template<typename P>
  class ValueWithError<unsigned long, P> {};
  template<typename P>
  class ValueWithError<const unsigned long, P> {};

  template<typename P>
  class ValueWithError<long long, P> {};
  template<typename P>
  class ValueWithError<unsigned long long, P> {};
  template<typename P>
  class ValueWithError<const unsigned long long, P> {};
  ///@}

} // namespace error_propagation

#endif // VALUE_WITH_ERROR_HPP
