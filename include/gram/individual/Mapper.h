#ifndef GRAM_INDIVIDUAL_MAPPER
#define GRAM_INDIVIDUAL_MAPPER

#include <gram/individual/Genotype.h>
#include <gram/individual/Phenotype.h>
#include <gram/language/Grammar.h>

namespace gram {
/**
 * Class.
 */
class Mapper {
 public:
  Mapper(std::shared_ptr<Grammar> grammar);
  virtual Phenotype map(Genotype genotype) const;
 private:
  std::shared_ptr<Grammar> grammar;
  Phenotype &recursiveMap(Phenotype &phenotype,
                          std::shared_ptr<NonTerminal> nonTerminal,
                          Genotype genotype, unsigned long &geneCount) const;
};
}

#endif // GRAM_INDIVIDUAL_MAPPER
