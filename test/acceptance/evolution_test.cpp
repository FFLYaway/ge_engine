#include "gram/Evolution.h"

#include <functional>
#include <string>

#include <catch.hpp>
#include <fakeit.hpp>

#include "gram/evaluation/driver/SingleThreadDriver.h"
#include "gram/evaluation/EvaluatorCache.h"
#include "gram/individual/Individual.h"
#include "gram/operator/selector/comparer/LowFitnessComparer.h"
#include "gram/operator/crossover/OnePointCrossover.h"
#include "gram/operator/mutation/BernoulliStepGenerator.h"
#include "gram/operator/mutation/FastCodonMutation.h"
#include "gram/language/grammar/ContextFreeGrammar.h"
#include "gram/language/mapper/ContextFreeMapper.h"
#include "gram/language/parser/BnfRuleParser.h"
#include "gram/population/Population.h"
#include "gram/population/initializer/RandomInitializer.h"
#include "gram/population/reproducer/PassionateReproducer.h"
#include "gram/operator/selector/TournamentSelector.h"
#include "gram/random/number_generator/XorShiftNumberGenerator.h"
#include "gram/util/Probability.h"
#include "gram/util/logger/NullLogger.h"

#include "StringDiffEvaluator.h"

using namespace fakeit;
using namespace gram;
using namespace std;

TEST_CASE("evolution_test") {
  auto numberGenerator1 = make_unique<XorShiftNumberGenerator>();
  auto numberGenerator2 = make_unique<XorShiftNumberGenerator>();
  auto numberGenerator3 = make_unique<XorShiftNumberGenerator>();
  auto numberGenerator4 = make_unique<XorShiftNumberGenerator>();
  auto numberGenerator5 = make_unique<XorShiftNumberGenerator>();
  auto stepGenerator = make_unique<BernoulliStepGenerator>(Probability(0.1), move(numberGenerator5));

  auto comparer = make_unique<LowFitnessComparer>();
  auto selector = make_unique<TournamentSelector>(20, move(numberGenerator1), move(comparer));
  auto mutation = make_unique<FastCodonMutation>(move(stepGenerator), move(numberGenerator2));
  auto crossover = make_unique<OnePointCrossover>(move(numberGenerator3));
  auto reproducer = make_unique<PassionateReproducer>(move(selector), move(crossover), move(mutation));

  string grammarString = "<word> ::= <word> <char> | <char>\n"
                         "<char> ::= \"g\" | \"r\" | \"a\" | \"m\"";

  BnfRuleParser parser;

  auto grammar = make_shared<ContextFreeGrammar>(parser.parse(grammarString));
  auto mapper = make_unique<ContextFreeMapper>(grammar, 1);

  RandomInitializer initializer(move(numberGenerator4), 50);

  auto evaluator = make_unique<StringDiffEvaluator>("gram");
  auto evaluatorCache = make_unique<EvaluatorCache>(move(evaluator));
  auto evaluationDriver = make_unique<SingleThreadDriver>(move(mapper), move(evaluatorCache));
  auto logger = make_unique<NullLogger>();

  Evolution evolution(move(evaluationDriver), move(logger));

  Population population = initializer.initialize(200, move(reproducer));

  Population lastGeneration = evolution.run(move(population), [](Population& currentPopulation) -> bool {
    return currentPopulation.lowestFitness() == 0.0;
  });

  const Individual& result = lastGeneration.individualWithLowestFitness();

  auto resultMapper = make_unique<ContextFreeMapper>(grammar, 1);

  REQUIRE(result.fitness() == 0.0);
  REQUIRE(result.serialize(*resultMapper) == "gram");
}
