#ifndef GRAM_EVOLUTION_EVOLUTION
#define GRAM_EVOLUTION_EVOLUTION

#include <memory>

#include <gram/individual/processor/Processor.h>
#include <gram/population/Population.h>

namespace gram {
/**
 * Class.
 */
class Evolution {
 public:
  Evolution(std::shared_ptr<Processor> processor);
  Individual run(Population &population, int goal);
 private:
  std::shared_ptr<Processor> processor;
};
}

#endif // GRAM_EVOLUTION_EVOLUTION
