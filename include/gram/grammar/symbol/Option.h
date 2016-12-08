#ifndef GRAM_GRAMMAR_SYMBOL_OPTION
#define GRAM_GRAMMAR_SYMBOL_OPTION

#include <memory>
#include <unordered_map>

#include <gram/grammar/symbol/NonTerminal.h>
#include <gram/grammar/symbol/Terminal.h>

namespace gram {
namespace grammar {
// Forward declaration.
class NonTerminal;
/**
 * Class.
 */
class Option {
 public:
  Option();
  void addNonTerminal(std::shared_ptr<NonTerminal> nonTerminal);
  void addTerminal(Terminal &terminal);
  bool hasNonTerminalAt(unsigned long index);
  bool hasTerminalAt(unsigned long index);
  std::shared_ptr<NonTerminal> nonTerminalAt(unsigned long index);
  Terminal &terminalAt(unsigned long index);
  int size();
  bool operator==(const Option &option) const;
  bool operator!=(const Option &option) const;
 private:
  std::unordered_map<unsigned long, std::shared_ptr<NonTerminal>> nonTerminals;
  std::unordered_map<unsigned long, Terminal *> terminals;
  int symbolCount;
};
}
}

#endif // GRAM_GRAMMAR_SYMBOL_OPTION
