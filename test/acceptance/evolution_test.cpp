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
#include "gram/operator/mutation/CodonMutation.h"
#include "gram/language/grammar/ContextFreeGrammar.h"
#include "gram/language/mapper/ContextFreeMapper.h"
#include "gram/language/parser/BnfRuleParser.h"
#include "gram/population/Population.h"
#include "gram/population/initializer/RandomInitializer.h"
#include "gram/population/reproducer/PassionateReproducer.h"
#include "gram/operator/selector/TournamentSelector.h"
#include "gram/random/number_generator/StdNumberGenerator.h"
#include "gram/random/Probability.h"
#include "gram/util/logger/NullLogger.h"

#include "StringDiffEvaluator.h"
#include "StringReplace.h"
#include "StringReplace.cpp"

#include <regex>

using namespace fakeit;
using namespace gram;
using namespace std;



TEST_CASE("evolution_test") {
  auto numberGenerator1 = make_unique<StdNumberGenerator<mt19937>>();
  auto numberGenerator2 = make_unique<StdNumberGenerator<mt19937>>();
  auto numberGenerator3 = make_unique<StdNumberGenerator<mt19937>>();
  auto numberGenerator4 = make_unique<StdNumberGenerator<mt19937>>();
  auto numberGenerator5 = make_unique<StdNumberGenerator<mt19937>>();
  auto stepGenerator = make_unique<BernoulliStepGenerator>(Probability(0.1), move(numberGenerator5));

  auto comparer = make_unique<LowFitnessComparer>();
  auto selector = make_unique<TournamentSelector>(20, move(numberGenerator1), move(comparer));
  auto mutation = make_unique<CodonMutation>(move(stepGenerator), move(numberGenerator2));
  auto crossover = make_unique<OnePointCrossover>(move(numberGenerator3));
  auto reproducer = make_unique<PassionateReproducer>(move(selector), move(crossover), move(mutation));

  /* behavior
  condition:{"picked a?", "a at pos p?", "a on b?"};
  action:{"pick a!", "place on a!", "place at pos p!", "put a on b!", "put a at pos p!", "apply force a!"};
  */

  //test_grammarString01
  string grammarString_origin = "<startRule> ::= <{Sequence}>|<{Fallback}>\n"

                         "<{Sequence}> ::= <{Fallback}><{Fallback}>| <condition><{Fallback}>|<condition><*action*> "\
                         "|<{Fallback}><actions>|<actions>\n"

                         "<{Fallback}> ::= <{Sequence}><{Sequence}>|<condition><{Sequence}> |<condition> <*action*> "\
                         "|<{Sequence}><actions>| <actions>\n"

                         "<condition> ::= \"<Condition ID=\'Condition\' name=\'picked a?\'/>\" "\
                         "| \"<Condition ID=\'Condition\' name=\'a at pos p?\'/>\"        "\
                         "| \"<Condition ID=\'Condition\' name=\'a on b?\'/>\" \n"

                         "<actions> ::= <*action*>|<*action*><*action*> \n"

                         "<*action*> ::= \"<Action ID=\'Action\' name=\'pick a!\'/>\"     "\
                         "| \"<Action ID=\'Action\' name=\'place on a!\'/>\"              "\
                         "| \"<Action ID=\'Action\' name=\'place at pos p!\'/>\"          "\
                         " | \"<Action ID=\'Action\' name=\'put a on b!\'/>\"             "\
                         "| \"<Action ID=\'Action\' name=\'put a at pos p!\'/>\"          "\
                         "| \"<Action ID=\'Action\' name=\'apply force a!\'/>\"\n ";

  auto grammar_to = make_unique<StringReplace>(grammarString_origin);
  string grammarString = grammar_to.replaceTogrammar();
  //test_grammarString02
  // string grammarString = "<startRule> ::= <{Sequence}>\n"
  //                        "<{Sequence}> ::= <{Sequence}> <*Fallback*> | <*Fallback*><{Sequence}> | <*Fallback*>\n"
  //                        "<*Fallback*> ::= \"g\"|\"r\"|\"a\"|\"m\"";

  BnfRuleParser parser;

  auto grammar1 = make_unique<ContextFreeGrammar>(parser.parse(grammarString));
  auto grammar2 = make_unique<ContextFreeGrammar>(parser.parse(grammarString));
  auto mapper1 = make_unique<ContextFreeMapper>(move(grammar1), 1); //the second parameter mean the condx appear occurrence
  auto mapper2 = make_unique<ContextFreeMapper>(move(grammar2), 1);

  RandomInitializer initializer(move(numberGenerator4), 50);

  // auto evaluator = make_unique<StringDiffEvaluator>("gram");

  auto evaluator = make_unique<StringDiffEvaluator>("<Condition ID='Condition' name='picked a?'/><Action ID='Action' name='put a on b!'/><Action ID='Action' name='put a at pos p!'/><Action ID='Action' name='apply force a!'/>");

  auto evaluatorCache = make_unique<EvaluatorCache>(move(evaluator));
  auto evaluationDriver = make_unique<SingleThreadDriver>(move(mapper1), move(evaluatorCache));
  auto logger = make_unique<NullLogger>();

  Evolution evolution(move(evaluationDriver), move(logger));

  Population population = initializer.initialize(200, move(reproducer));

  Population lastGeneration = evolution.run(move(population), [](Population& currentPopulation) -> bool {
    return currentPopulation.lowestFitness() == 0.0;
  });

  const Individual& result = lastGeneration.individualWithLowestFitness();

  REQUIRE(result.fitness() == 0.0);
  // REQUIRE(result.serialize(*mapper2) == "gram");

  REQUIRE(result.serialize(*mapper2) == "<Condition ID='Condition' name='picked a?'/><Action ID='Action' name='put a on b!'/><Action ID='Action' name='put a at pos p!'/><Action ID='Action' name='apply force a!'/>");
}