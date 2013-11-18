#pragma once

// set default policy
#ifndef DEFAULT_POLICY_CLASS
#define DEFAULT_POLICY_CLASS ExactValueAndIgnoreErrorPolicy
#endif // not DEFAULT_POLICY_CLASS

namespace error_propagation {

  class ExactValueAndIgnoreErrorPolicy;
  class CompareWithinErrorIntervalsPolicy;
  template<typename T, typename P = DEFAULT_POLICY_CLASS>
  class ValueWithError;

} // namespace error_propagation

