#ifndef GRAM_INDIVIDUAL_NUMBER_MUTATION_OPERATOR
#define GRAM_INDIVIDUAL_NUMBER_MUTATION_OPERATOR

#include <memory>

#include <gram/individual/operator/Mutation.h>
#include <gram/util/bool_generator/BoolGenerator.h>
#include <gram/util/number_generator/NumberGenerator.h>

namespace gram {
/**
 * Class.
 */
class NumberMutation : public Mutation {
 public:
  NumberMutation(std::unique_ptr<BoolGenerator> boolGenerator, std::unique_ptr<NumberGenerator> numberGenerator);
  virtual Genotype apply(Genotype genotype) const;
 private:
  std::unique_ptr<BoolGenerator> boolGenerator;
  std::unique_ptr<NumberGenerator> numberGenerator;
};
}

#endif // GRAM_INDIVIDUAL_NUMBER_MUTATION_OPERATOR
