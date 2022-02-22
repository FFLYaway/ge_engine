#ifndef GRAM_TEST_ACTION_DIFF_EVALUATOR
#define GRAM_TEST_ACTION_DIFF_EVALUATOR

#include <memory>
#include <string>

#include "gram/evaluation/Evaluator.h"
#include "gram/individual/Fitness.h"
#include "gram/individual/Phenotype.h"
#include "gram/language/mapper/ContextFreeMapper.h"

class ActionDiffEvaluator : public gram::Evaluator {
public:
  ActionDiffEvaluator(unsigned long desired);
  gram::Fitness evaluate(const gram::Phenotype& phenotype) noexcept override;
  gram::Fitness calculateFitness(std::string program);
  

private:
  unsigned long desired;;
};

#endif 
