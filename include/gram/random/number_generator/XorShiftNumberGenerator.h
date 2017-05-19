#ifndef GRAM_XORSHIFT_NUMBER_GENERATOR
#define GRAM_XORSHIFT_NUMBER_GENERATOR

#include <cstdint>

#include "gram/random/number_generator/NumberGenerator.h"

namespace gram {
/**
 * Class.
 */
class XorShiftNumberGenerator : public NumberGenerator {
public:
  XorShiftNumberGenerator();
  unsigned long generate() override;
  unsigned long getMax() override;

private:
  std::uint32_t state;
};
}

#endif
