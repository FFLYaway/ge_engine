#ifndef GRAM_NON_TERMINAL
#define GRAM_NON_TERMINAL

#include "gram/language/symbol/Symbol.h"

namespace gram {
class Rule;
/**
 * Class.
 */
class NonTerminal : public Symbol {
public:
  NonTerminal(Rule& rule);//加入字符串的引入
  Rule& toRule() const;
  bool isTerminal() const override;
  bool isNonTerminal() const override;
  bool operator==(const NonTerminal& nonTerminal) const;
  bool operator!=(const NonTerminal& nonTerminal) const;

  //非终端原本没有getValue的需求，只需要对规则进行选择
  // std::string getValue() const;//需要对带语义的内部节点进行遍历211205

private:
  Rule& rule;
  // std::string value;//需要定义为私有
};
}

#endif
