#include <gram/population/reproduction/Reproduction.h>

using namespace gram;
using namespace std;

Reproduction::Reproduction(unique_ptr<IndividualSelector> selector,
                     unique_ptr<Crossover> crossover,
                     unique_ptr<Mutation> mutation)
    : selector(move(selector)), crossover(move(crossover)), mutation(move(mutation)) {
  //
}

Individuals Reproduction::reproduce(const Individuals &individuals) const {
  Individuals children;

  unsigned long size = individuals.size();

  for (unsigned long i = 0; i < size; i++) {
    Individual parent1 = selector->select(individuals);
    Individual parent2 = selector->select(individuals);

    Individual child = parent1.mateWith(parent2, *crossover);
    child.mutate(*mutation);

    children.addIndividual(make_shared<Individual>(child));
  }

  return children;
}