#include <iostream>
#include <string>
#include<unordered_map>
#include<functional>
#include<sstream>
#include<cstdlib>
#include <sys/stat.h>

bool running = true;            // shell running variable 

//Helper functions

bool file_exists(const std::string& path) {           //check if file exists
    struct stat st;
    return stat(path.c_str(), &st) == 0;
}

bool is_executable(const std::string& path) {             //if its executable
#ifdef _WIN32
    return file_exists(path);
#else
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return false;

    return (st.st_mode & S_IXUSR) ||
           (st.st_mode & S_IXGRP) ||
           (st.st_mode & S_IXOTH);
#endif
}



//echo cmd to print
void echo(const std::string& args) {
  std::cout << args << std::endl;
}


//type cmd 
void type_cmd(const std::string& command, const std::unordered_map<std::string,std::function<void(const std::string&)>>& builtins_commands) {
  if (builtins_commands.find(command) != builtins_commands.end()) {
    std::cout << command << " is a shell builtin" << std::endl;
    return;
  } 
  
  const char* path_env = std::getenv("PATH");          //get path
    if (!path_env) {
        std::cout << command << ": not found\n";
        return;
    }

    std::string path_str = path_env;

#ifdef _WIN32
    const char delimiter = ';';                     //windows
#else 
    const char delimiter = ':';
#endif

    std::stringstream ss(path_str);
    std::string dir;

    while (std::getline(ss, dir, delimiter)) {
        if (dir.empty()) continue;

#ifdef _WIN32
        std::string full = dir + "\\" + command + ".exe";
        if (is_executable(full)) {
            std::cout << command << " is " << full << "\n";
            return;
        }
#else
        std::string full = dir + "/" + command;
        if (is_executable(full)) {
            std::cout << command << " is " << full << "\n";
            return;
        }
#endif
    }

    std::cout << command << ": not found" << std::endl;
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
    builtins_commands["type"]=[&](const std::string& args){
      type_cmd(args,builtins_commands);                  //lambda function to work on multiple varibale
    };

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
