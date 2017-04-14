#include <gram/language/mapper/ContextFreeMapper.h>

using namespace gram;
using namespace std;

ContextFreeMapper::ContextFreeMapper(shared_ptr<ContextFreeGrammar> grammar, unsigned long wrappingLimit)
    : grammar(grammar), wrappingLimit(wrappingLimit) {
  //
}

Phenotype ContextFreeMapper::map(const Genotype& genotype) {
  Phenotype phenotype;
  stack<Symbol*> symbols;

  unsigned long wrappings = 0;
  unsigned long codonIndex = 0;

  Rule& startRule = grammar->startRule();
  unsigned long optionIndex = genotype[codonIndex] % startRule.size();
  Option& startOption = startRule.optionAt(optionIndex);
  pushOption(symbols, startOption);
  codonIndex += 1;

  while (!symbols.empty()) {
    auto& symbol = symbols.top();
    symbols.pop();

    if (symbol->isTerminal()) {
      auto terminal = dynamic_cast<Terminal*>(symbol);
      phenotype += terminal->getValue();
    } else {
      if (codonIndex == genotype.size()) {
        codonIndex = 0;
        wrappings += 1;

        if (wrappings > wrappingLimit) {
          throw logic_error("Wrapping limit exceeded during genotype-phenotype mapping.");
        }
      }

      auto nonTerminal = dynamic_cast<NonTerminal*>(symbol);
      Rule& rule = nonTerminal->toRule();
      optionIndex = genotype[codonIndex] % rule.size();
      Option& option = rule.optionAt(optionIndex);
      pushOption(symbols, option);
      codonIndex += 1;
    }
  }

  return phenotype;
}

void ContextFreeMapper::pushOption(stack<Symbol*>& symbols, Option& option) const {
  auto& optionSymbols = option.getSymbols();

  for (long i = option.size() - 1; i >= 0; i--) {
    symbols.push(optionSymbols[i].get());
  }
}
