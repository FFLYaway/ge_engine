#ifndef GRAM_SYMBOL
#define GRAM_SYMBOL

namespace gram {
class Terminal;
class NonTerminal;
/**
 * Abstract class.
 */
class Symbol {
public:
  // Symbol(std::string str_name);//
  virtual ~Symbol() = default;
  const Terminal& toTerminal() const;
  const NonTerminal& toNonTerminal() const;
  virtual bool isTerminal() const = 0;
  virtual bool isNonTerminal() const = 0;
  bool operator==(const Symbol& symbol) const;
  bool operator!=(const Symbol& symbol) const;

  // std::string getStr() const;

// private://
  // std::string str_name;
};
}

#endif
