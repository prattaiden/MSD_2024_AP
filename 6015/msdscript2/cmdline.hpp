//
// Created by Aiden Pratt on 1/11/24.
//
#ifndef msdscript_CMDLINE_H
#define msdscript_CMDLINE_H


/**
 * \file cmdline.hpp
 * \brief file with declaration of use_arguments
 * @param argc
 * @param argv
 */

typedef enum{
    do_nothing,
    do_interp,
    do_print,
    do_pretty_print
}run_mode_t;

run_mode_t use_arguments(int argc, char **argv);




#endif //msdscript_CMDLINE_H