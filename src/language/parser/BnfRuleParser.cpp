#include "gram/language/parser/BnfRuleParser.h"

#include <algorithm>
#include <memory>
#include <regex>
#include <string>
#include <vector>

#include "gram/error/InvalidGrammar.h"
#include "gram/language/symbol/NonTerminal.h"
#include "gram/language/symbol/Option.h"
#include "gram/language/symbol/Rule.h"
#include "gram/language/symbol/Terminal.h"
#include "gram/util/helper.h"

using namespace gram;
using namespace std;

//输出类contextFreeMapper的grammer
//line是一行由\n分隔的语法
ContextFreeGrammar BnfRuleParser::parse(string input) const {
  // todo: handle multi-line rules

  ContextFreeGrammar grammar;

  vector<string> lines = explode(input, "\n");

  for (auto line : lines) {
    parseRule(grammar, line);//依次输入line，grammer为输出
  }

  return grammar;
}

//解析rule
//弹出name，剪切name和等号，此时已经起名字
void BnfRuleParser::parseRule(ContextFreeGrammar& grammar, string& line) const {
  string name;

  line = left_trim(line);

  if (line.empty()) {
    return;
  }

  //通过判断返回name，此时line是已经被裁剪过的
  if (!parseNonTerminal(name, line) || !parseEquals(line)) {
    throw InvalidGrammar();
  }

  Rule& rule = grammar.ruleNamed(name);

  while (line.length() > 0) {
    unique_ptr<Option> option = parseOption(grammar, line);
    rule.addOption(move(option));
  }
}

//输入的是剪切过后的
unique_ptr<Option> BnfRuleParser::parseOption(ContextFreeGrammar& grammar, string& line) const {
  auto option = make_unique<Option>();//新建

  while (line.length() > 0) {
    string name;
    string value;

    if (parseNonTerminal(name, line)) {
      Rule& rule = grammar.ruleNamed(name);
      auto nonTerminal = make_unique<NonTerminal>(rule);
      option->addNonTerminal(move(nonTerminal));
    } else if (parseTerminal(value, line)) {
      auto terminal = make_unique<Terminal>(value);
      option->addTerminal(move(terminal));
    } else if (parsePipe(line)) {
      break;
    } else {
      throw InvalidGrammar();
    }
  }

  return option;
}

//name为::=前的内容，匹配了所有内容
bool BnfRuleParser::parseNonTerminal(string& name, string& line) const {
  regex pattern("^<([a-zA-Z][a-zA-Z0-9-]*)>");
  smatch matches;

  if (!regex_search(line, matches, pattern)) {
    return false;
  }

  name = matches[1];
  line = line.substr(name.length() + 2);
  line = left_trim(line);

  return true;
}

//
bool BnfRuleParser::parseTerminal(string& value, string& line) const {
  regex pattern("^\"([a-zA-Z0-9| -!#$%&\\(\\)\\*\\+,-\\./:;<=>?@\\[\\\\\\]\\^_`{}~]+)\"");
  smatch matches;

  if (!regex_search(line, matches, pattern)) {
    return false;
  }

  value = matches[1];
  line = line.substr(value.length() + 2);
  line = left_trim(line);

  return true;
}

//无内容返回
bool BnfRuleParser::parseEquals(string& line) const {
  regex pattern("^::=");
  smatch matches;

  if (!regex_search(line, matches, pattern)) {
    return false;
  }

  line = line.substr(3);
  line = left_trim(line);

  return true;
}

//无内容返回，剪切分割符
bool BnfRuleParser::parsePipe(string& line) const {
  regex pattern("^\\|");
  smatch matches;

  if (!regex_search(line, matches, pattern)) {
    return false;
  }

  line = line.substr(1);
  line = left_trim(line);

  return true;
}
