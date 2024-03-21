//
// Created by Aiden Pratt on 1/11/24.
//
#include <iostream>
#include "cmdline.hpp"
#define CATCH_CONFIG_RUNNER
#include "catch.h"

/**
 * \file cmdline.cpp
 * \brief implementation of use_arguments for the command line
 */
run_mode_t use_arguments(int argc, char **argv) {
    int length = argc;
    bool testFlag = false;
    run_mode_t mode;

    for (int i = 1; i < length; i++) {
        std::string argVString = argv[i];

        //--help
        if (argVString.compare("--help") == 0) {
            std::cout <<"use the following tags:\n";
            std::cout << "'--test'\n";
            exit(0);
        }

        //--test
        if(argVString.compare("--test") == 0) {
            if (!testFlag) {
                if(Catch::Session().run(1,argv) != 0){
                    std::cout << "a test failed\n";
                    exit(1);
                }
                std::cout << "all tests passed\n";
                testFlag = true;
            }else{
                std::cerr << "error: --tests already ran\n";
                exit(1);
            }
        }

        //--interp
        if(argVString.compare("--interp") == 0){
            mode = do_interp;
        }

        //--print
        if(argVString.compare("--print") == 0){
            mode = do_print;
        }

        if(argVString.compare("--pretty_print")==0){
            mode = do_pretty_print;
        }
       // else if()

//        else{
//            std::cerr << "error: bad flag, use '--help' for more info\n";
//            exit(1);
//        }
    }


    return mode;
}
