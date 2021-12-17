#include "gram/language/symbol/NonTerminal.h"

#include "gram/language/symbol/Rule.h"

using namespace gram;
using namespace std;

NonTerminal::NonTerminal(Rule& rule ) : rule(rule)  {
  //
}

Rule& NonTerminal::toRule() const {
  return rule;
}

bool NonTerminal::isTerminal() const {
  return false;
}

bool NonTerminal::isNonTerminal() const {
  return true;
}

bool NonTerminal::operator==(const NonTerminal& nonTerminal) const {
  return rule.getName() == nonTerminal.rule.getName();
}

bool NonTerminal::operator!=(const NonTerminal& nonTerminal) const {
  return !operator==(nonTerminal);
}

//与终端节点处定义相同方法211205
// string NonTerminal::getValue() const {
//   return value;
// }

//end_211205
