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

//add symbol of closed
Terminal seqOfClose("</Sequence>");
Terminal fallOfClose("</Fallback>");

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
  fp.open("//home//dcs//autoclose.xml", ios::out);

  fp << "<?xml version=\"1.0\"?>" << endl;
  fp << "<root main_tree_to_execute=\"BehaviorTree\">" << endl;
  fp << "<BehaviorTree ID=\"BehaviorTree\">" << endl;


  while (!symbols.empty()) {
    auto symbol = symbols.back();
    symbols.pop_back();

    if (symbol->isTerminal()) {
      auto terminal = symbol->toTerminal();

      if(terminal.getValue() != "</Sequence>" && terminal.getValue() != "</Fallback>") {
        phenotype += terminal.getValue();
      }

      stringForPrint += terminal.getValue();

    } else {
      if (codonIndex == genotype.size()) {
        codonIndex = 0;
        wrappings += 1;

        if (wrappings > wrappingLimit) {
          throw WrappingLimitExceeded(wrappingLimit);
        }
      }

      auto nonTerminal = symbol->toNonTerminal();
      Rule& rule = nonTerminal.toRule();

      //recognize the rule of GE
      string nameOfRule = rule.getName();
      regex pattern("^\\{([a-zA-Z][a-zA-Z0-9-]*)\\}");
      regex pattern_limit("^\\*([a-zA-Z][a-zA-Z0-9-]*)\\*");
      smatch matches;
      smatch matches_limit;

      //auto close the control node
      if(regex_search(nameOfRule, matches, pattern)) {
        string nameOfPrint = matches[1];
        string headOfPrint = "<" + nameOfPrint + ">";

        stringForPrint += headOfPrint;//write down the head of xml file

        optionIndex = genotype[codonIndex] % rule.size();
        Option& option = rule[optionIndex];
        pushOptionAndString(option, nameOfPrint);
        codonIndex += 1;
      }


      //recognize the limited behavior occurrences
      else if(regex_search(nameOfRule, matches_limit, pattern_limit)) {
        optionIndex = genotype[codonIndex] % rule.size();
        Option& option = rule[optionIndex];

        //behavior occurrences limit
        rule.delOption(optionIndex);

        //logger of limit behavior occurrences
        string sizeOflimitRule = to_string(rule.size());
        sizeOflimitRule = "<!--" + sizeOflimitRule + "-->";
        stringForPrint += sizeOflimitRule;

        pushOption(option);
        
        codonIndex += 1;
      } 

      else {
        optionIndex = genotype[codonIndex] % rule.size();
        Option& option = rule[optionIndex];
        pushOption(option);
        codonIndex += 1;
      } 

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

void ContextFreeMapper::pushOptionAndString(Option& option, string controlNode) {
  if(controlNode == "Sequence") {
    unsigned long optionSize = option.size();

    symbols.push_back(move(&seqOfClose));

    for (long i = optionSize - 1; i >= 0; i--) {
    symbols.push_back(&option[i]);
    }
  }
  else {
    unsigned long optionSize = option.size();

    symbols.push_back(move(&fallOfClose));

    for (long i = optionSize - 1; i >= 0; i--) {
    symbols.push_back(&option[i]);
    }
  }

}