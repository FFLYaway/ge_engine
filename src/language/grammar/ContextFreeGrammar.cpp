#include "gram/language/grammar/ContextFreeGrammar.h"

#include <algorithm>

#include "gram/error/InvalidGrammar.h"
#include "gram/language/symbol/Rule.h"

using namespace gram;
using namespace std;

ContextFreeGrammar::ContextFreeGrammar() : start("") {
  //
}

void ContextFreeGrammar::addRule(unique_ptr<Rule> rule) {
  if (start.length() == 0) {
    start = rule->getName();
  }

  rules[rule->getName()] = move(rule);
}

Rule& ContextFreeGrammar::ruleNamed(string name) {
  Rule* rule = rules[name].get();

  if (rule == nullptr) {
    auto newRule = make_unique<Rule>(name);

    rule = newRule.get();

    rules[name] = move(newRule);
  }

  if (start.length() == 0) {
    start = rule->getName();
  }

  return *rule;
}

Rule& ContextFreeGrammar::startRule() {
  if (start.length() == 0) {
    throw InvalidGrammar();
  }

  return *rules[start].get();
}

void ContextFreeGrammar::deleteRule(string name) {
  // delete the rule
  rules.erase(name);
  
  // // delete the rule symbol in other rule
  // for(auto iter = rules.begin(); iter != rules.end(); ++iter) {
  //   //

  // }
}

bool ContextFreeGrammar::existOfRule(string name) {
  if(rules.find(name) != rules.end()) { return true; }
  else return false;
}
