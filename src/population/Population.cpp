#include <gram/population/Population.h>

using namespace gram::population;
using namespace gram::population::individual;

Population::Population(std::vector<Individual> individuals) : individuals(individuals) {
  //
}

Individual &Population::operator[](int index) {
  return individuals[index];
}
