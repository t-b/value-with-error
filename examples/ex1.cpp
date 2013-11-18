#include "ValueWithError.hpp"

int main()
{
  using error_propagation::make_value;
  auto a = make_value(5.0,0.3);
  auto b = make_value(2.0,0.2);

  std::cout << "a*b=" << a * b << std::endl;
}
