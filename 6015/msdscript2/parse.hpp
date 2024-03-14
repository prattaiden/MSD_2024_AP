
#ifndef parse_hpp
#include "expr.h"
#define parse_hpp
#include <cstdio>

 //expr * parse_keyword(std::istream &in);
expr* parse_comparg(std::istream &in);
expr* parse_if(std::istream & in);
expr *parse_expr(std::istream &in);
expr* parse_addend(std::istream &in);
expr* parse_multicand(std::istream &in);
expr* parse_var(std::istream &in);
expr *parse_num(std::istream &in);
expr * parse_str(const std::string &str);

static void consume(std::istream &in, int expect);
void skip_whitespace(std::istream &in);

expr* parseInput();




#endif