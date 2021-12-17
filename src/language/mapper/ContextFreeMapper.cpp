#include "gram/language/mapper/ContextFreeMapper.h"

#include "gram/error/WrappingLimitExceeded.h"
#include "gram/language/grammar/ContextFreeGrammar.h"
#include "gram/language/symbol/NonTerminal.h"
#include "gram/language/symbol/Option.h"
#include "gram/language/symbol/Rule.h"
#include "gram/language/symbol/Symbol.h"
#include "gram/language/symbol/Terminal.h"

#include <stdio.h>
#include <cstring>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace gram;
using namespace std;

//私有成员变量有向量类型symbol
ContextFreeMapper::ContextFreeMapper(unique_ptr<ContextFreeGrammar> grammar, unsigned long wrappingLimit)
    : grammar(move(grammar)), wrappingLimit(wrappingLimit) {
  symbols.reserve(512);
}
  
Phenotype ContextFreeMapper::map(const Genotype& genotype) {
  symbols.clear();

  Phenotype phenotype;                    //表征型
  phenotype.reserve(512);

  unsigned long wrappings = 0;
  unsigned long codonIndex = 0;                                       //数值字符串索引

  //起点规则
  Rule& startRule = grammar->startRule();                                 //输入：grammar
  unsigned long optionIndex = genotype[codonIndex] % startRule.size();    //一条语法中规则选择的索引
  Option& startOption = startRule[optionIndex];                           //选择第一行语法对应的规则
  pushOption(startOption);
  codonIndex += 1;

  //写文件头_211203
  // std::string startName = startRule.getName();//开始点名字
  std::string str_fileHead = "<?xml version=\"1.0\"?>\n\
<root main_tree_to_execute=\"BehaviorTree\">\n\
    <BehaviorTree ID=\"BehaviorTree\">";
  ofstream xmlOut("behaviorTreeXml.xml");
  xmlOut<<str_fileHead<<endl;

  int indentationIndex = 1;          //缩进索引参数
  int terminalNodeCnt = 0;

  //symbol的大小
  int sizeOfSymbols = symbols.size();//比较大小

  std::unordered_map<int, std::string> endNonterminalNode;
  endNonterminalNode.reserve(3);

  //由symbol映射为phenotype
  while (!symbols.empty()) {
    //弹出非终端
    auto symbol = symbols.back();
    symbols.pop_back();

    std::string verticalSpace = "";
    std::string verticalSpace_end = "";


    //终端节点
    if (symbol->isTerminal()){                               //终端规则
      auto terminal = symbol->toTerminal();
      phenotype += terminal.getValue();

      std::string terminalNode = terminal.getValue(); 

      //写入终端内容
      if(terminalNodeCnt == 0) {
        verticalSpace.append(4*(indentationIndex + 1) , ' ');
        xmlOut<<verticalSpace<<"<AlwaysSuccess name=\""<<terminalNode<<"\"/>"<<endl;
      }
      else {
        verticalSpace.append(4*(indentationIndex + 2) , ' ');
        xmlOut<<verticalSpace<<"<AlwaysSuccess name=\""<<terminalNode<<"\"/>"<<endl;

        verticalSpace_end.append(4*(indentationIndex + 1) , ' ');
        std::string str = endNonterminalNode[indentationIndex + 1];
        xmlOut<<verticalSpace_end<<"</"<<str<<">"<<endl;
      }


      terminalNodeCnt += 1;
    } 
    
    //非终端节点（内部节点）
    else {
      if (codonIndex == genotype.size()) {              //出现包装事件抛出异常
        codonIndex = 0;
        wrappings += 1;

        if (wrappings > wrappingLimit) {
          throw WrappingLimitExceeded(wrappingLimit);
        }
      }

      //未达到终端的处理
      auto nonTerminal = symbol->toNonTerminal();
      Rule& rule = nonTerminal.toRule();
      optionIndex = genotype[codonIndex] % rule.size();
      Option& option = rule[optionIndex];                //option为当前语法中选择的规则
      pushOption(option);
      codonIndex += 1;

      int IndexOfSymbols = symbols.size();               //symbols的长度

      //修改缩进格式大小
      if(sizeOfSymbols < IndexOfSymbols) indentationIndex += 1;
      else if(sizeOfSymbols > IndexOfSymbols) indentationIndex -= 1;

      //写非终端内容
      std::string NonTerminalNode = rule.getName();
      verticalSpace.append(4*indentationIndex , ' ');   //由参数进行缩进格式的修改
       
      //写入非终端内容
      if(NonTerminalNode == "Sequence" || NonTerminalNode == "Fallback"){
        xmlOut<<verticalSpace<<"<"<<NonTerminalNode<<">"<<endl;
        endNonterminalNode[indentationIndex] = NonTerminalNode;
      } 

      sizeOfSymbols = IndexOfSymbols;//计算


    }


  }

  //写入文件尾
  std::string str_fileBack = "    </BehaviorTree>\n\
</root>";
  xmlOut<<str_fileBack<<endl;
  xmlOut.close();

  return phenotype;
}

//由后到前
void ContextFreeMapper::pushOption(Option& option) {
  unsigned long optionSize = option.size();

  for (long i = optionSize - 1; i >= 0; i--) {
    symbols.push_back(&option[i]);
  }
}