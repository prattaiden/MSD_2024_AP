//
// Created by Aiden Pratt on 2/20/24.
//
#include <iostream>
#include "test_msdscript.h"
#include "exec.h"

int main(int argc, char **argv) {
    srand (clock());
    for (int i = 0; i < 10; i++) {
        std::string in = random_expr_string();
    }

    if(argc == 2) {
        const char *const interp_argv[] = {argv[1], "--interp"};
        const char *const print_argv[] = {argv[1], "--print"};
        const char *const pp_argv[] = {argv[1], "--pretty_print"};

        for (int i = 0; i < 100; i++) {
            std::string in = random_expr_string();
            std::cout << "Trying " << in << "\n";
            ExecResult interp_result = exec_program(2, interp_argv, in);
            ExecResult print_result = exec_program(2, print_argv, in);
            ExecResult pp_result = exec_program(2, pp_argv, in);
            ExecResult interp_again_result = exec_program(2, interp_argv, print_result.out);
            if (interp_again_result.out != interp_result.out)
                throw std::runtime_error("different result for printed");
        }
    }
    //this is used for testing with the other programs
    if(argc == 3){
        const char *const interp_myMSD_argv[] = {argv[1], "--interp"};
        const char *const interp_tester_argv[] = {argv[2], "--interp"};

        const char *const print_myMSD_argv[] = {argv[1], "--print"};
        const char *const print_tester_argv[] = {argv[2], "--print"};

        const char *const pp_myMSD_argv[] = {argv[1], "--pretty_print"};
        const char *const pp_tester_argv[] = {argv[2], "--pretty-print"};

        for (int i = 0; i < 100; i++) {
            std::string in = random_expr_string();
           // std::cout << "Trying" << in << "\n";

            ExecResult interp_myMSD_result = exec_program(2, interp_myMSD_argv, in);
            ExecResult interp_tester_result = exec_program(2, interp_tester_argv, in);

            ExecResult print_myMSD_result = exec_program(2, print_myMSD_argv, in);
            ExecResult print_tester_result = exec_program(2, print_tester_argv, in);

            ExecResult pp_myMSD_result = exec_program(2, pp_myMSD_argv, in);
            ExecResult pp_tester_result = exec_program(2, pp_tester_argv, in);


            if(interp_myMSD_result.out != interp_tester_result.out){
                std::cout << "my result(interp): " << interp_myMSD_result.out;
                std::cout << "tester result(interp): " << interp_tester_result.out  << "\n" << "\n";
            }

            if(print_myMSD_result.out != print_tester_result.out){
                std::cout << "my result(print): " << print_myMSD_result.out;
                std::cout << "tester result(print): " << print_tester_result.out  << "\n" << "\n";
            }

            if(pp_myMSD_result.out != pp_tester_result.out){
                std::cout << "my result(pp): \n" << pp_myMSD_result.out << "\n";
                std::cout << "tester result(pp): \n" << pp_tester_result.out  << "\n" << "\n";
            }
        }
    }
    return 0;
}

std::string rand_var(){
    std::string s = "";
    for (int i = 0; i < rand()%6;i++ ){
        s+=rand() % 26 + 'a';
    }
    return s;
}

std::string random_let_body(std::string lhs) {

    std::string errorMessage = "Error in random_expr \n";
    int random = rand() % 100;
    switch (random) {
        case 0 ... 33:
            return lhs + "+" + random_expr_string();
        case 34 ... 66:
            return lhs + "*" + random_expr_string();
        case 67 ... 90:
            return random_expr_string() + "+" + lhs;
        default:
            return random_expr_string() + "*" + lhs;
    }
}

std::string random_expr_string(){
    if ((rand() % 10) < 6)
        return std::to_string(rand());
    else {
        int random = rand() % 100;
        switch (random) {
            case 0 ... 20:
                return "(" + random_expr_string() + ")";
            case 21 ... 32:
                return random_expr_string() + " + " + random_expr_string();
            case 33 ... 45:
                return random_expr_string() + " * " + random_expr_string();
            case 46 ... 65:
                return rand_var();
            case 66 ... 76:
                return ("_let " + rand_var() + "=" + random_expr_string() + "_in " + random_let_body(random_expr_string()));
            case 77 ... 95:
                return "-" + std::to_string(rand());
            default:
                return random_expr_string();
        }
    }
}