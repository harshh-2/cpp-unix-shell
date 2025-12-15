#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while(true){
   std::cout << "$ ";                          //$ sign like windows shell
   std::string input;                          
   std::getline(std::cin,input);                // getting input using getline
   if(input=="exit"){
    break;                                        //REPL exit
   }
   std::cout<<input<<":"<<" command not found"<<std::endl;
  }

}
