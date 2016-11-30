#include <gtest/gtest.h>

#include <gram/evolution/operator/Mutation.h>
#include <gram/fake/util/FakeNumberGenerator.h>

using namespace gram::evolution;
using namespace gram::population::individual;
using namespace gram::fake::util;

TEST(mutation_operator_test, test_it_mutates_one_gene) {
  Genotype genotype{1, 1, 1};
  Genotype expectedGenotype{1, 1, 3};

  FakeNumberGenerator numberGenerator{2, 3};

  Mutation mutation(numberGenerator);
  Genotype mutatedGenotype = mutation.apply(genotype);

  ASSERT_EQ(expectedGenotype, mutatedGenotype);
}
