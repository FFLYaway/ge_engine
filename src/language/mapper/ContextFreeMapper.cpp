#include "gram/language/mapper/ContextFreeMapper.h"

#include "gram/error/WrappingLimitExceeded.h"
#include "gram/language/grammar/ContextFreeGrammar.h"
#include "gram/language/symbol/NonTerminal.h"
#include "gram/language/symbol/Option.h"
#include "gram/language/symbol/Rule.h"
#include "gram/language/symbol/Symbol.h"
#include "gram/language/symbol/Terminal.h"

#include "gram/language/parser/BnfRuleParser.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace gram;
using namespace std;

ContextFreeMapper::ContextFreeMapper(unique_ptr<ContextFreeGrammar> grammar, unsigned long wrappingLimit)
    : grammar(move(grammar)), wrappingLimit(wrappingLimit) {
  symbols.reserve(512);
}

Phenotype ContextFreeMapper::map(const Genotype& genotype) {
  symbols.clear();

  Phenotype phenotype;
  phenotype.reserve(512);


  Phenotype stringForPrint;
  stringForPrint.reserve(1024);

  unsigned long wrappings = 0;
  unsigned long codonIndex = 0;

  Rule& startRule = grammar->startRule();
  unsigned long optionIndex = genotype[codonIndex] % startRule.size();
  Option& startOption = startRule[optionIndex];
  pushOption(startOption);
  codonIndex += 1;

  //head of the xml file
  ofstream fp;
  fp.open("//home//dcs//gram//test//acceptance//BehaviorTree.xml", ios::out);

  fp << "<?xml version=\"1.0\"?>" << endl;
  fp << "<root main_tree_to_execute=\"BehaviorTree\">" << endl;
  fp << "<BehaviorTree ID=\"BehaviorTree\">" << endl;


  while (!symbols.empty()) {
    auto symbol = symbols.back();
    symbols.pop_back();

    if (symbol->isTerminal()) {
      auto terminal = symbol->toTerminal();

      if(terminal.getValue() != "</Sequence>" && terminal.getValue() != "</Fallback>"&& terminal.getValue() != "<Fallback>"&& terminal.getValue() != "<Sequence>") {
        phenotype += terminal.getValue();
      }

      //string diff to print
      stringForPrint += terminal.getValue();

    } else {
      if (codonIndex == genotype.size()) {
        codonIndex = 0;
        wrappings += 1;

        if (wrappings > wrappingLimit) {
          break;
          // throw WrappingLimitExceeded(wrappingLimit);
        }
      }

      auto nonTerminal = symbol->toNonTerminal();
      Rule& rule = nonTerminal.toRule();

      //recognize the limited behavior occurrences
      // else if(regex_search(nameOfRule, matches_limit, pattern_limit)) {
      //   optionIndex = genotype[codonIndex] % rule.size();
      //   Option& option = rule[optionIndex];

      //   //behavior occurrences limit
      //   rule.delOption(optionIndex);

      //   //logger of limit behavior occurrences
      //   string sizeOflimitRule = to_string(rule.size());
      //   sizeOflimitRule = "<!-- rule size:" + sizeOflimitRule + "-->";
      //   stringForPrint += sizeOflimitRule;

      //   pushOption(option);
        
      //   codonIndex += 1;
      // } 
        optionIndex = genotype[codonIndex] % rule.size();
        Option& option = rule[optionIndex];
        pushOption(option);
        codonIndex += 1;

    }
  }

  fp << stringForPrint <<endl;
  fp << "</BehaviorTree>" << endl;
  fp << "</root>" << endl;
  fp.close();

  return phenotype;
}

void ContextFreeMapper::pushOption(Option& option) {
  unsigned long optionSize = option.size();

  for (long i = optionSize - 1; i >= 0; i--) {
    symbols.push_back(&option[i]);
  }
}