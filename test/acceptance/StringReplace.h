#ifndef GRAM_STRING_REPLACE_FUNCTION
#define GRAM_STRING_REPLACE_FUNCTION

#include <memory>
#include <string>
#include <vector>

class StringReplace {
public:
  StringReplace(std::string grammarString);
  std::string& replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value); 
  std::string replaceTogrammar();    

private:
  std::string grammarString;

};

#endif 
