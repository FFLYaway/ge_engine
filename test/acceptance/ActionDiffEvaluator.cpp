#include "ActionDiffEvaluator.h"

#include <memory>
#include <string>

//add by 
#include <unordered_set>
#include <regex>

#include "gram/util/helper.h"

#include "gram/evaluation/Evaluator.h"
#include "gram/individual/Fitness.h"
#include "gram/individual/Phenotype.h"
#include "gram/language/mapper/ContextFreeMapper.h"

using namespace gram;
using namespace std;

ActionDiffEvaluator::ActionDiffEvaluator(unsigned long desired) : desired(desired) {
  //
}

Fitness ActionDiffEvaluator::evaluate(const Phenotype& phenotype) noexcept {
  try {
    return calculateFitness(phenotype);
  } catch (...) {
    return 1000;
  }
}

Fitness ActionDiffEvaluator::calculateFitness(string program) {
  regex pattern("^name=\'([a-zA-Z][a-zA-Z0-9-]*)$!");
  smatch matches;

  if(regex_search(program, matches, pattern)) {
      return false;
  }

  unsigned long length = matches.size();

  unsigned long sizeOfSet = 0;

  for(unsigned long cnt = 0; cnt < length; cnt++) {
      unordered_set<string> actionSet;
      actionSet.emplace(matches[cnt]);
      sizeOfSet = actionSet.size();
  }
  

  Fitness fitness(desired - sizeOfSet);

  return fitness;
}


