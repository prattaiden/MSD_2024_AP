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
    run_mode_t mode = use_arguments(argc,argv);
    expr *n;

    if(mode == do_print){
        n = parse_expr(std::cin);
        std::cout << n->to_string()<<"\n";
        }
    if(mode == do_interp){
        n = parse_expr(std::cin);
        std::cout << "value: " << n->interp() << "\n";
    }
    if(mode == do_pretty_print){
        n = parse_expr(std::cin);
        std::cout << n->to_pp_string()<<"\n";
    }

    return 0;
}