#ifndef GRAM_STRING_REPLACE_FUNCTION
#define GRAM_STRING_REPLACE_FUNCTION

#include <memory>
#include <string>

#include "gram/util/helper.h"

using namespace std;

class StringReplace {
public:
  StringReplace(std::string grammarString);
  string& replace_all_distinct(string& str, const string& old_value, const string& new_value); 
  string replaceTogrammar();    

private:
  std::string grammarString;

};

#endif 
