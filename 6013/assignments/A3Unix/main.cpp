#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>
#include "shelpers.hpp"

/*Aiden Pratt
*CS-6013 HW3 Unix Shell
*my own implementation of a shell which can take a handful of commands and perform them
*todo add more here
*/

void changeDirectoryCommand(std::vector<std::string> &tokens) {
    //cd hard coding
    if (tokens[0] == "cd") {
        //directory is set to "HOME" if cd is only command, else input following cd is used as a filepath
        std::string directory = (tokens.size() == 1) ? getenv("HOME") : tokens[1];

        if (chdir(directory.c_str()) != 0) {
            perror("failed to change directory");
        }
    }
}

void printVector(std::vector<Command> &commands){
    //printing the current command vector
    for(int i =0; i < commands.size(); i++){
        std::cout << commands[i] << "\n";
    }
}




int main(int argc, char *argv[]) {
    std::string input;

    while(true) {
        std::cout << "? ";

        if(!std::getline(std::cin , input)){
            std::cout << "i am in here ";
            exit(0);
        }

        if(input == "exit"){
            std::cout << "exit was called, bye";
            exit(0);
        }

        //tokenizing the input "command"
        std::vector<std::string> tokens = tokenize(input);

        changeDirectoryCommand(tokens);

        //getting the command structure
        std::vector<Command> commands = getCommands(tokens);

        //printing vector
        printVector(commands);

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
            dup2(command.inputFd, 0); //input
            dup2(command.outputFd, 1); //output
            //todo need to close the outputs, in the parent

            //closing the file descriptors
            if(command.inputFd != STDIN_FILENO) close(command.inputFd);
            if(command.outputFd != STDOUT_FILENO) close(command.outputFd);


            execvp(command.execName.c_str(), const_cast<char* const*>(command.argv.data()));
            perror("\nexecvp failed");
            exit(1);
        }

        else{
            //in parent, wait on child
            //closing file descriptors associated with pipes
            if(command.inputFd != STDIN_FILENO) close(command.inputFd);
            if(command.outputFd != STDOUT_FILENO) close(command.outputFd);
            bool runInBackground = false;
            //checking for "&" which indicates process to run in background
            if(!tokens.empty() && tokens.back() == "&"){
                runInBackground = true;
               // removing the "&" from the tokens
                tokens.pop_back();
            }
            if(!runInBackground){
                //if it is not running in background, wait for hte child process to finish
                waitpid(pid, nullptr, 0);
                std::cout << "PARENT: done waiting for child to run.\n";
            }
            else{
                std::cout << "PARENT: child is running in background, I will not wait\n";
            }

        }
    }
}