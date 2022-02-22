#include "gram/util/logger/NullLogger.h"

#include "gram/population/Population.h"

using namespace gram;

void NullLogger::logProgress(const Population& population) {
  static_cast<void>(population);

  //
  unsigned long numberOfGeneration = population.generationNumber();
  Fitness bestFitnessOfGeneration = population.lowestFitness();
  printf("当前代数:%lu\t优势适应度:%f\n" , numberOfGeneration , bestFitnessOfGeneration);
}

void NullLogger::logResult(const Population& population) {
  static_cast<void>(population);

  //
  unsigned long finalGenerationNumber = population.generationNumber();
  Fitness bestFitnessOfOutcome = population.lowestFitness();
  printf("最终代数:%lu\t最优适应度:%f\n" , finalGenerationNumber , bestFitnessOfOutcome);
}
