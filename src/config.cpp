#include "mantella_bits/config.hpp"

namespace mant {
  bool cacheSamples(true);
  bool recordSamples(false);
#if defined(__CLING__)
  bool verboseOutput(true);
#else
  bool verboseOutput(false);
#endif
}
