
#ifndef parse_hpp
#include "expr.h"
#define parse_hpp
#include <cstdio>
#include "pointer.h"

 //expr * parse_keyword(std::istream &in);
PTR(expr) parse_comparg(std::istream &in);
PTR(expr) parse_if(std::istream & in);
PTR(expr) parse_expr(std::istream &in);
PTR(expr) parse_addend(std::istream &in);
PTR(expr) parse_inner(std::istream &in);
PTR(expr) parse_var(std::istream &in);
PTR(expr) parse_num(std::istream &in);
PTR(expr) parse_str(const std::string &str);
PTR(expr) parse_if(std::istream  &in);

static void consume(std::istream &in, int expect);
void skip_whitespace(std::istream &in);

expr* parseInput();




#endif