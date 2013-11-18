#ifndef  VALUE_WITH_ERROR_HPP
#define VALUE_WITH_ERROR_HPP

#include "../DetailValueWithError.hpp"
#include "../ValueWithErrorComparisonPolicy.hpp"

/// All documentation is located in ../cpp11/ValueWithError.hpp which
/// is a superset of the functionality implemented here.
namespace error_propagation {

  using std::abs; // required so that ADL works in the ctor initializer list

  template<typename T, typename P>
  void swap(ValueWithError<T, P>& lhs, ValueWithError<T, P>& rhs)
  {
    lhs.swap(rhs);
  }

  template<typename T, typename P>
  class ValueWithError
  {
    public:
    typedef T value_type;
    typedef P policy_type;

    explicit ValueWithError(const T& value, const T& error)
      :
      m_value(value),
      m_error(abs(error))
    {}

    explicit ValueWithError(const T& value)
      :
      m_value(value),
      m_error()
    {}

    ValueWithError()
      :
      m_value(),
      m_error()
    {}

    ///@name Copy assignment
    ///@{
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
    ///@}

    ///@name Copy constructor
    ///@{
    template<typename U>
    ValueWithError(const ValueWithError<U, P>& rhs)
      :
      m_value(rhs.GetValue()),
      m_error(rhs.GetError())
    {}
    ///@}

    ///@name Access functions
    ///@{
    inline const T& GetValue() const
    {
      return m_value;
    }

    inline const T& GetError() const
    {
      return m_error;
    }
    ///@}

    ///@name Arithmetic operators
    ///@{
    template<typename U>
    ValueWithError<T, P>&
    operator*=(const ValueWithError<U, P>& rhs)
    {
      *this = (*this) * rhs;
      return *this;
    }

    template<typename U>
    ValueWithError<T, P>&
    operator*=(const U& rhs)
    {
      *this = (*this) * rhs;
      return *this;
    }

    template<typename U>
    ValueWithError<T, P>&
    operator/=(const ValueWithError<U, P>& rhs)
    {
      *this = (*this) / rhs;
      return *this;
    }

    template<typename U>
    ValueWithError<T, P>&
    operator/=(const U& rhs)
    {
      *this = (*this) / rhs;
      return *this;
    }

    template<typename U>
    ValueWithError<T, P>&
    operator+=(const ValueWithError<U, P>& rhs)
    {
      *this = (*this) + rhs;
      return *this;
    }

    template<typename U>
    ValueWithError<T, P>&
    operator+=(const U& rhs)
    {
      *this = (*this) + rhs;
      return *this;
    }

    template<typename U>
    ValueWithError<T, P>&
    operator-=(const ValueWithError<U, P>& rhs)
    {
      *this = (*this) - rhs;
      return *this;
    }

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

  template<typename T, typename U>
  const ValueWithError<typename detail::promote_args<T, U>::type, DEFAULT_POLICY_CLASS>
  make_value(T value, U error)
  {
    typedef typename detail::promote_args<T, U>::type R;
    return ValueWithError<R, DEFAULT_POLICY_CLASS>(value, error);
  }

  ///@name Arithmetic operator definitions
  ///
  ///NOT DEFINED: %, bitwise operators, increment, decrement
  ///@{
  template<typename U, typename V, typename P>
  const ValueWithError<typename detail::promote_args<U, V>::type, P>
  operator*(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                lhs.GetValue() * rhs.GetValue(),
                                detail::hypot<R>(rhs.GetValue() * lhs.GetError(),lhs.GetValue() * rhs.GetError())
                               );
  }

  template<typename U, typename V, typename P>
  const ValueWithError<typename detail::promote_args<U, V>::type, P>
  operator*(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                lhs.GetValue() * rhs,
                                lhs.GetError() * rhs
                               );
  }

  template<typename U, typename V, typename P>
  const ValueWithError<typename detail::promote_args<V, U>::type, P>
  operator*(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    typedef typename detail::promote_args<V, U>::type R;
    return ValueWithError<R, P>(
                                rhs.GetValue() * lhs,
                                rhs.GetError() * lhs
                               );
  }


  template<typename U, typename V, typename P>
  const ValueWithError<typename detail::promote_args<U, V>::type, P>
  operator/(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                lhs.GetValue() / rhs.GetValue(),
                                detail::hypot( lhs.GetError(), lhs.GetValue() / rhs.GetValue() * rhs.GetError()) / rhs.GetValue()
                               );
  }

  template<typename U, typename V, typename P>
  const ValueWithError<typename detail::promote_args<U, V>::type, P>
  operator/(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                lhs.GetValue() / rhs,
                                lhs.GetError() / rhs
                               );
  }

  template<typename U, typename V, typename P>
  const ValueWithError<typename detail::promote_args<V, U>::type, P>
  operator/(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    typedef typename detail::promote_args<V, U>::type R;
    return ValueWithError<R, P>(
                                lhs / rhs.GetValue(),
                                (lhs * rhs.GetError()) / ( rhs.GetValue() * rhs.GetValue())
                               );
  }


  template<typename U, typename V, typename P>
  const ValueWithError<typename detail::promote_args<U, V>::type, P>
  operator+(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                lhs.GetValue() + rhs.GetValue(),
                                detail::hypot(lhs.GetError(), rhs.GetError())
                               );
  }

  template<typename U, typename V, typename P>
  const ValueWithError<typename detail::promote_args<U, V>::type, P>
  operator+(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                lhs.GetValue() + rhs,
                                lhs.GetError()
                               );
  }

  template<typename U, typename V, typename P>
  const ValueWithError<typename detail::promote_args<V, U>::type, P>
  operator+(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    typedef typename detail::promote_args<V, U>::type R;
    return ValueWithError<R, P>(
                                rhs.GetValue() + lhs,
                                rhs.GetError()
                               );
  }


  template<typename U, typename V, typename P>
  const ValueWithError<typename detail::promote_args<U, V>::type, P>
  operator-(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                lhs.GetValue() - rhs.GetValue(),
                                detail::hypot(lhs.GetError(), rhs.GetError())
                               );
  }

  template<typename U, typename V, typename P>
  const ValueWithError<typename detail::promote_args<U, V>::type, P>
  operator-(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                lhs.GetValue() - rhs,
                                lhs.GetError()
                               );
  }

  template<typename U, typename V, typename P>
  const ValueWithError<typename detail::promote_args<V, U>::type, P>
  operator-(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    typedef typename detail::promote_args<V, U>::type R;
    return ValueWithError<R, P>(
                                lhs - rhs.GetValue(),
                                rhs.GetError()
                               );
  }
  ///@}

  ///@name Comparison operator definitions
  ///
  ///NOT DEFINED: !
  ///@{
  template<typename U, typename V, typename P>
  const bool
  operator==(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    return P::Equal(lhs, rhs);
  }

  template<typename U, typename V, typename P>
  const bool
  operator==(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    return P::Equal(lhs, rhs);
  }

  template<typename U, typename V, typename P>
  const bool
  operator==(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    return P::Equal(lhs,rhs);
  }


  template<typename U, typename V, typename P>
  const bool
  operator!=(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    return ( ! (lhs==rhs) );
  }

  template<typename U, typename V, typename P>
  const bool
  operator!=(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    return ( ! (lhs==rhs) );
  }

  template<typename U, typename V, typename P>
  const bool
  operator!=(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    return ( ! (lhs==rhs) );
  }

  template<typename U, typename V, typename P>
  const bool
  operator<(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    return (rhs > lhs);
  }

  template<typename U, typename V, typename P>
  const bool
  operator<(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    return (rhs > lhs);
  }

  template<typename U, typename V, typename P>
  const bool
  operator<(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    return (rhs > lhs);
  }

  template<typename U, typename V, typename P>
  const bool
  operator>(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    return P::GreaterThan(lhs, rhs);
  }

  template<typename U, typename V, typename P>
  const bool
  operator>(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    return P::GreaterThan(lhs, rhs);
  }

  template<typename U, typename V, typename P>
  const bool
  operator>(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    return P::GreaterThan(lhs, rhs);
  }

  template<typename U, typename V, typename P>
  const bool
  operator<=(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    return (rhs >= lhs);
  }

  template<typename U, typename V, typename P>
  const bool
  operator<=(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    return (rhs >= lhs);
  }

  template<typename U, typename V, typename P>
  const bool
  operator<=(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    return (rhs >= lhs);
  }


  template<typename U, typename V, typename P>
  const bool
  operator>=(const ValueWithError<U, P>& lhs, const ValueWithError<V, P>& rhs)
  {
    return P::GreaterOrEqual(lhs, rhs);
  }

  template<typename U, typename V, typename P>
  const bool
  operator>=(const ValueWithError<U, P>& lhs, const V& rhs)
  {
    return P::GreaterOrEqual(lhs, rhs);
  }

  template<typename U, typename V, typename P>
  const bool
  operator>=(const U& lhs, const ValueWithError<V, P>& rhs)
  {
    return P::GreaterOrEqual(lhs, rhs);
  }
  ///@}

  ///@name Conversion operators
  ///@{
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
    // standard output format: "(value+-error)", additional whitespace is silently skipped
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

  ///@name Mathematical function overloads
  ///
  ///List of all functions:
  ///Taken from C++ 2003 standard page 601 and C++ 2011 standard page 984
  ///
  ///not overloaded: ceil, copysign, fma, fmax, fmin, fmod, frexp,
  ///                ilogb, ldexp, llrint, llround, logb, lrint, lround,
  ///                modf, nan, nanf, nanl, nearbyint, nextafter, nexttoward,
  ///                remainder, remquo, rint, round, scalbln, scalbn, trunc,
  ///                fpclassify, isgreaterequal islessequal, isnan, isunordered,
  ///                isfinite, isinf, islessgreater isnormal, signbit, isgreater,
  ///                isless
  ///@{
  template<typename T, typename P>
  const ValueWithError<T, P>
  abs(const ValueWithError<T, P>& v)
  {
    return ValueWithError<T, P>(
                                abs(v.GetValue()),
                                v.GetError()
                               );
  }

  template<typename T, typename P>
  const ValueWithError<T, P>
  acos(const ValueWithError<T, P>& v)
  {
    using std::acos;
    using std::sqrt;
    return ValueWithError<T, P>(
                                acos(v.GetValue()),
                                1.0 / sqrt(1.0 - v.GetValue() * v.GetValue()) * v.GetError()
                               );
  }

  template<typename T, typename P>
  const ValueWithError<T, P>
  asin(const ValueWithError<T, P>& v)
  {
    using std::asin;
    using std::sqrt;
    return ValueWithError<T, P>(
                                asin(v.GetValue()),
                                1.0 / sqrt(1.0 - v.GetValue() * v.GetValue()) * v.GetError()
                               );
  }

  template<typename T, typename P>
  const ValueWithError<T, P>
  atan(const ValueWithError<T, P>& v)
  {
    using std::atan;
    return ValueWithError<T, P>(
                                atan(v.GetValue()),
                                1.0 / (1.0 + v.GetValue() * v.GetValue()) * v.GetError()
                               );
  }

  template<typename U, typename V, typename P>
  const ValueWithError<typename detail::promote_args<U, V>::type, P>
  atan2(const ValueWithError<U, P>& y, const ValueWithError<V, P>& x)
  {
    using std::atan2;
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                atan2(y.GetValue(),
                                x.GetValue()),1.0 / (y.GetValue() * y.GetValue() + x.GetValue() * x.GetValue())
                                * detail::hypot(y.GetValue() * x.GetError(), x.GetValue() * y.GetError())
                               );
  }

  template<typename U, typename V, typename P>
  const ValueWithError<typename detail::promote_args<U, V>::type, P>
  atan2(const U& y, const ValueWithError<V, P>& x)
  {
    using std::atan2;
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                atan2(y, x.GetValue()),
                                1.0 / (y * y + x.GetValue() * x.GetValue()) * y * x.GetError()
                               );
  }

  template<typename U, typename V, typename P>
  const ValueWithError<typename detail::promote_args<U, V>::type, P>
  atan2(const ValueWithError<U, P>& y, const V& x)
  {
    using std::atan2;
    typedef typename detail::promote_args<U, V>::type R;
    return ValueWithError<R, P>(
                                atan2(y.GetValue(), x),
                                1.0 / (y.GetValue() * y.GetValue() + x * x) * x * y.GetError()
                               );
  }

  template<typename T, typename P>
  const ValueWithError<T, P>
  cos(const ValueWithError<T, P>& v)
  {
    using std::cos;
    using std::sin;
    // sign of derivative is stripped off by abs() in constructor
    return ValueWithError<T, P>(
                                cos(v.GetValue()),
                                sin(v.GetValue()) * v.GetError()
                               );
  }

  template<typename T, typename P>
  const ValueWithError<T, P>
  cosh(const ValueWithError<T, P>& v)
  {
    using std::cosh;
    using std::sinh;
    return ValueWithError<T, P>(
                                cosh(v.GetValue()),
                                sinh(v.GetValue()) * v.GetError()
                               );
  }

  template<typename T, typename P>
  const ValueWithError<T, P>
  exp(const ValueWithError<T, P>& v)
  {
    using std::exp;
    return ValueWithError<T, P>(
                                exp(v.GetValue()),
                                exp(v.GetValue()) * v.GetError()
                               );
  }

  template<typename T, typename P>
  const ValueWithError<T, P>
  fabs(const ValueWithError<T, P>& v)
  {
    using std::fabs;
    return ValueWithError<T, P>(
                               fabs(v.GetValue()),
                               v.GetError()
                              );
  }

  template<typename T, typename P>
  const ValueWithError<T, P>
  log(const ValueWithError<T, P>& v)
  {
    using std::log;
    return ValueWithError<T, P>(
                                log(v.GetValue()),
                                1.0 / v.GetValue() * v.GetError()
                               );
  }

  template<typename T, typename P>
  const ValueWithError<T, P>
  log10(const ValueWithError<T, P>& v)
  {
    using std::log10;
    using boost::math::constants::ln_ten;
    return ValueWithError<T, P>(
                                log10(v.GetValue()),
                                1.0 / ( v.GetValue() * ln_ten<T>()) * v.GetError()
                               );
  }

  template<typename T, typename V, typename P>
  const ValueWithError<typename detail::promote_args<T, V>::type, P>
  pow(const ValueWithError<T, P>& base, const V& exponent)
  {
    using std::pow;
    typedef typename detail::promote_args<T, V>::type R;
    return ValueWithError<R, P>(
                                pow(base.GetValue(), exponent),
                                exponent*pow(base.GetValue(), exponent-1)*base.GetError()
                               );
  }

  template<typename T, typename V, typename P>
  const ValueWithError<typename detail::promote_args<V, T>::type, P>
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

  template<typename T, typename V, typename P>
  const ValueWithError<typename detail::promote_args<T, V>::type, P>
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

  template<typename T, typename P>
  const ValueWithError<T, P>
  sin(const ValueWithError<T, P>& v)
  {
    using std::cos;
    using std::sin;
    return ValueWithError<T, P>(
                                sin(v.GetValue()),
                                cos(v.GetValue()) * v.GetError()
                               );
  }

  template<typename T, typename P>
  const ValueWithError<T, P>
  sinh(const ValueWithError<T, P>& v)
  {
    using std::cosh;
    using std::sinh;
    return ValueWithError<T, P>(
                                sinh(v.GetValue()),
                                cosh(v.GetValue()) * v.GetError()
                               );
  }

  template<typename T, typename P>
  const ValueWithError<T, P>
  sqrt(const ValueWithError<T, P>& v)
  {
    using std::sqrt;
    return ValueWithError<T, P>(
                                sqrt(v.GetValue()),
                                0.5 / sqrt(v.GetValue()) * v.GetError()
                               );
  }

  template<typename T, typename P>
  const ValueWithError<T, P>
  tan(const ValueWithError<T, P>& v)
  {
    using std::cos;
    using std::tan;
    return ValueWithError<T, P>(
                                 tan(v.GetValue()),
                                 v.GetError() / (cos(v.GetValue()) * cos(v.GetValue()))
                                );
  }

  template<typename T, typename P>
  const ValueWithError<T, P>
  tanh(const ValueWithError<T, P>& v)
  {
    using std::cosh;
    using std::tanh;
    return ValueWithError<T, P>(
                                tanh(v.GetValue()),
                                v.GetError() / (cosh(v.GetValue()) * cosh(v.GetValue()))
                               );
  }
  ///@}

  ///@defgroup invalidSpecializations Partial specializations for invalid types
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
  ///@}

} // namespace error_propagation

#endif // VALUE_WITH_ERROR_HPP
