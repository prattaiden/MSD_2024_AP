#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>
#include "shelpers.hpp"

int main(int argc, char *argv[]) {
    std::string input;

    while(std::getline(std::cin , input)) {

        if(input == "exit"){
            std::cout << "exit was called, bye";
            exit(0);
        }

        //tokenizing the input "command"
        std::vector<std::string> tokens = tokenize(input);

        //getting the command structure

        std::vector<Command> commands = getCommands(tokens);

        for(int i =0; i < commands.size(); i++){
            std::cout << commands[i] << "\n";
        }
        //assuming just one line command for now
        const Command &command = commands[0];

        //forking the program after getting the command?
        pid_t pid = fork();

        if(pid<0){
            perror("fork failed");
        }
        if(pid == 0){
            std::cout << "Child: hello my pid is: " << getpid();
            //use the dup2 command to point to the file outputFd
            //close(1);
            dup2(command.inputFd, 0);
            dup2(command.outputFd, 1);
            //const_cast
            execvp(command.execName.c_str(), const_cast<char* const*>(command.argv.data()));
            perror("\nexecvp failed");
            exit(1);
        }

        else{
            //in parent, wait on child
            std::cout << "PARENT: my pid is: " << pid << "\n";
            wait(nullptr);
            std::cout << "PARENT: child with pid " << pid << " is done" << std::endl;
        }
    }
}