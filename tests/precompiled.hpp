#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#endif

#include <boost/type_traits.hpp>
#include <boost/timer/timer.hpp>
#include <boost/math/special_functions/digamma.hpp>
#include <boost/math/special_functions/fpclassify.hpp>
#include <boost/math/special_functions/hypot.hpp>
#include <boost/math/special_functions/pow.hpp>
#include <boost/math/tools/promotion.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

#if defined __GNUC__ \
            && ( __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 7) ) \
            && !defined __clang__
#pragma GCC diagnostic pop
#endif

#include <cmath>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <valarray>

#if __cplusplus >= 201103L
  #include <type_traits>
  #include <initializer_list>
#endif
