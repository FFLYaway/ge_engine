#include "gram/util/helper.h"

#include <algorithm>

#include <cctype>

using namespace std;

//对语法进行切片处理，pieses为向量组
vector<string> gram::explode(string input, string delimiter) {
  vector<string> pieces;

  unsigned long end = input.find(delimiter);//换行符的位置

  while (end != string::npos) {
    pieces.push_back(input.substr(0, end));

    input = input.substr(end + delimiter.length());

    end = input.find(delimiter);
  }

  pieces.push_back(input);

  return pieces;
}

//从input开始删除空格
string gram::left_trim(string input) {
  input.erase(input.begin(), find_if(input.begin(), input.end(), [](char symbol) {
    return !isspace(symbol);
  }));

  return input;
}
