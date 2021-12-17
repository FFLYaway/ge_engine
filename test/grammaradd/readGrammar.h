// #include <iostream>
// #include <fstream>
// #include <string>

// using namespace std;

// string readGrammar(string filename){
//   ifstream myGrammar(filename);
//   string stringGrammar;
//   if(!myGrammar.is_open()) {
//     cout<<"Error Reading Grammar"<<endl;
//   }
//   while(!myGrammar.eof()){
//       getline(myGrammar , stringGrammar);
//       stringGrammar += stringGrammar;
//   }

//   myGrammar.close();
//   return stringGrammar;
// }