#include <catch.hpp>
#include <fakeit.hpp>

#include <gram/individual/mutation/BernoulliDistributionStepGenerator.h>
#include <gram/util/number_generator/MinimalNumberGenerator.h>

using namespace fakeit;
using namespace gram;
using namespace std;

TEST_CASE("Bernoulli distribution step generator generates statistically good steps",
          "[bernoulli_distribution_step_generator") {
  unsigned long testSize = 1000000;

  auto numberGenerator = make_unique<MinimalNumberGenerator>();

  BernoulliDistributionStepGenerator stepGenerator(0.5, move(numberGenerator));

  unsigned long totalSteps = 0;

  for (unsigned long i = 0; i < testSize; i++) {
    totalSteps += stepGenerator.generateStep();
  }

  REQUIRE(totalSteps > testSize * 0.99);
  REQUIRE(totalSteps < testSize * 1.01);
}
