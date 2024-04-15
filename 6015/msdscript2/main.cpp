/**
* \mainpage MSDScript
* \author Aiden Pratt
* \date 02-06-2024
*/

#include <iostream>
#include "cmdline.hpp"
#include "expr.h"
#include "parse.hpp"

/**
 * \file main.cpp
 * \brief main file for calling use_arguments
 */


int main(int argc, char** argv){
    try {
        run_mode_t mode = use_arguments(argc, argv);
        PTR(expr) n;

        switch(mode){
            case do_nothing:
                break;
            case do_print:
                n = parse_expr(std::cin);
                std::cout << n->to_string() << "\n";
                break;
            case do_interp:
                n = parse_expr(std::cin);
                std::cout << n->interp() << "\n" ;
                break;
            case do_pretty_print:
                n = parse_expr(std::cin);
                std::cout << n->to_pp_string() << "\n";
                break;
        }

        return 0;
    }
    catch (std::runtime_error exn) {
        std::cerr << exn.what() << "\n";
        return 1;
    }

}