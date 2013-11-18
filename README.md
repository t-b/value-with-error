### ValueWithError

C++98/11 header-only class for gaussian error propagation.

It allows to answer the question *"What is the error of measured values with errors after an arithmetic operation?"* for the general case.

For example
```
[5.0 +/- 0.3] + [2.0 +/- 0.2] = ?
```
can be implemented as
```cpp
#include "ValueWithError.hpp"

int main()
{
  using error_propagation::make_value;
  auto a = make_value(5.0,0.3);
  auto b = make_value(2.0,0.2);

  std::cout << "a * b = " << a * b << std::endl;
}
```
and gives a * b = (10+-1.16619).

It is designed as a drop-in replacement for an arithmetic type as *all* arithmetic operations from the C++ standard library are implemented.

For the internal value type the following types have been tested:
- double
- float
- boost::multiprecision::cpp\_dec\_100/50
- Eigen::Array
- std::valarray

#### Requirements (minimum versions)
- C++ compiler (see below)
- Boost 1.55
- doxygen 1.8.5 (only for building the documentation)
- cmake 2.8.11 (only for building the test cases)

#### Usage
1. git clone https://github.com/t-b/value-with-error.git
2. Add value-with-error/src/cpp11 or value-with-error/src/cpp98 to your include path
3. Add ```#include "ValueWithError.hpp"``` and start using error_propagation::ValueWithError<T,P>

#### Tested C++ compiler
##### Windows
- C++98
  - VC 8-11
- C++11/C++98
  - VC 12
  - mingw gcc 4.8.1
  - mingw clang 3.3

##### Linux
- C++98
  - gcc 4.6
- C++11/C++98
  - gcc 4.7/4.8/4.9 trunk
  - clang 3.3/3.4 trunk

### License

This software is released under the "Boost Software License 1.0" see http://www.opensource.org/licenses/bsl1.0.html.
