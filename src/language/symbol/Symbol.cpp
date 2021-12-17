#include "gram/language/symbol/Symbol.h"


#include "gram/language/symbol/NonTerminal.h"
#include "gram/language/symbol/Terminal.h"

using namespace gram;
using namespace std;

// Symbol::Symbol(string str_name) : str_name(str_name){//symbol的字符串属性
//   //
// }

const Terminal& Symbol::toTerminal() const {
  return static_cast<const Terminal&>(*this);
}

const NonTerminal& Symbol::toNonTerminal() const {
  return static_cast<const NonTerminal&>(*this);
}

bool Symbol::operator==(const Symbol& symbol) const {
  if (isTerminal()) {
    if (!symbol.isTerminal()) {
      return false;
    }

    return toTerminal() == symbol.toTerminal();
  }

  if (isNonTerminal()) {
    if (!symbol.isNonTerminal()) {
      return false;
    }

    return toNonTerminal() == symbol.toNonTerminal();
  }

  return false;
}

bool Symbol::operator!=(const Symbol& symbol) const {
  return !operator==(symbol);

//添加返回字符串
// string Symbol::getStr() const {
//   return str_name;
// }

}
