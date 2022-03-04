#include "StringReplace.h"

#include <memory>
#include <string>

#include <regex>

// #include "gram/util/helper.h"
using namespace std;

StringReplace::StringReplace(std::string grammarString):grammarString(grammarString){
  //
}


string& replace_all_distinct(string& str, const string& old_value, const string& new_value)     
{     
  for(string::size_type pos(0); pos!=string::npos; pos+=new_value.length()) {     
    if((pos=str.find(old_value,pos)) != string::npos) { 
      str.replace(pos,old_value.length(),new_value);  
    }   
    else { break; }    
  }     
  return str;     
}

string replaceTogrammar() {
  // split the grammar
  vector<string> lines;

  // \n
  string delimiter = "\n";
  unsigned long end = grammarString.find(delimiter);

  while (end != string::npos) {
    lines.push_back(grammarString.substr(0, end));

    grammarString = grammarString.substr(end + delimiter.length());

    end = grammarString.find(delimiter);
  }

  lines.push_back(grammarString);

  // change the string

  for(unsigned long i = 0; i < lines.size(); ++i) {
    regex pattern("^<\\{([a-zA-Z][a-zA-Z0-9-]*)\\}>");
    smatch mathches;

    if(!regex_search(lines[i], mathches, pattern)) {}
    else {
      string name = mathches[1];
      // regex pattern_change_equal("^::=");
      // regex pattern_change_pipe("^\\|");

      if(name == "Sequence") {
        replace_all_distinct(lines[i], "{Sequence}", "Sequence");
        replace_all_distinct(lines[i], "::=", "::=<s>");
        replace_all_distinct(lines[i], "|", "</s>|<s>");
        lines[i] += "</s>";
                
      } 
      else {
        replace_all_distinct(lines[i], "{Fallback}", "Fallback");
        replace_all_distinct(lines[i], "::=", "::=<f>");
        replace_all_distinct(lines[i], "|", "</f>|<f>");
        lines[i] += "</f>";
      }
    }
  }

  // add the label
  string labelOfControlNode = "<s> ::=\"<Sequence>\"\n"
                              "</s> ::=\"</Sequence>\"\n"
                              "<f> ::=\"<Fallback>\"\n"
                              "</f> ::=\"</Fallback>\"";

  // return the the grammar by add
  string tostring_grammar;
  for(unsigned long len = 0; len < lines.size(); ++len) {
    lines[len] += "\n";
    tostring_grammar += lines[len];
  }

  return tostring_grammar + labelOfControlNode;
}