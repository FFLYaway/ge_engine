#ifndef GRAM_POPULATION_INDIVIDUALS
#define GRAM_POPULATION_INDIVIDUALS

#include <memory>
#include <vector>

#include <gram/individual/comparer/IndividualComparer.h>
#include <gram/individual/Individual.h>

namespace gram {
/**
 * Class.
 */
class Individuals {
 public:
  void addIndividual(Individual individual);
  const Individual& bestIndividual(IndividualComparer& comparer) const;
  Individual& operator[](unsigned long index);
  unsigned long size() const;
  void reserve(unsigned long count);
  std::vector<Individual>::iterator begin();
  std::vector<Individual>::iterator end();
  std::vector<Individual>::const_iterator begin() const;
  std::vector<Individual>::const_iterator end() const;
 private:
  std::vector<Individual> individuals;
};
}

#endif // GRAM_POPULATION_INDIVIDUALS
