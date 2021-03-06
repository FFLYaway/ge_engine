#ifndef GRAM_CONTEXT_FREE_GRAMMAR
#define GRAM_CONTEXT_FREE_GRAMMAR

#include <memory>
#include <string>
#include <unordered_map>

#include "gram/language/symbol/Rule.h"

namespace gram {
/**
 * Class.
 */
class ContextFreeGrammar {
public:
  ContextFreeGrammar();
  void addRule(std::unique_ptr<Rule> rule);
  Rule& ruleNamed(std::string name);
  Rule& startRule();

  // delete the rule
  void deleteRule(std::string name);
  bool existOfRule(std::string name);

private:
  std::string start;
  std::unordered_map<std::string, std::unique_ptr<Rule>> rules;//rules的结构为哈希表
};
}

#endif
