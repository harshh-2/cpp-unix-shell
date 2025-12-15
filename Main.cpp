#include <iostream>
#include <string>
#include<unordered_map>
#include<functional>
#include<windows.h>
#include<sstream>
#include<cstdlib>

bool running = true;            // shell running variable 

//echo cmd to print
void echo(const std::string& args) {
  std::cout << args << std::endl;
}

 //exit cmd to exit
void exit_shell(const std::string& args){
running=false;
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  //builtin cmds storage 
  std::unordered_map<std::string,std::function<void(const std::string&)>> builtins_commands;
    builtins_commands["echo"]=echo;
    builtins_commands["exit"]=exit_shell;

  while(running){

    // input

   std::cout << "$ ";
   std::string input;
   std::getline(std::cin,input);

   if(input.empty()){
    continue;
   }

   //input parsing 

   std::size_t pos=input.find(' ');               
   std::string cmd=input.substr(0,pos);         //get the cmd
   std::string args= (pos==std::string::npos) ? "": input.substr(pos+1);        //get the arguments
  
   if (builtins_commands.count(cmd)) {
      builtins_commands[cmd](args);                                       //to run the cmd
   } 
   
   else {
      std::cout << cmd << ": command not found\n";
   }

}  //while loop ends

}
