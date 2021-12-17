#include "gram/language/grammar/ContextFreeGrammar.h"

#include <algorithm>

#include "gram/error/InvalidGrammar.h"
#include "gram/language/symbol/Rule.h"

using namespace gram;
using namespace std;

//成员变量
//1.start(string类型)
// 2.哈希表类型的rules
ContextFreeGrammar::ContextFreeGrammar() : start("") {
  //
}

//哈希表rules添加rule
void ContextFreeGrammar::addRule(unique_ptr<Rule> rule) {
  if (start.length() == 0) {
    start = rule->getName();
  } 

  rules[rule->getName()] = move(rule);
}

//grammer中的rules为成员变量，数据结构为哈希表
//给rule赋值
Rule& ContextFreeGrammar::ruleNamed(string name) {
  Rule* rule = rules[name].get();

  //当前rule为空时，先初始化
  if (rule == nullptr) {
    auto newRule = make_unique<Rule>(name);//新建

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
