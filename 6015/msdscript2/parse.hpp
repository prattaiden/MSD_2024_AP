
#ifndef parse_hpp
#include "expr.h"
#define parse_hpp
#include <cstdio>

static expr *parse_expr(std::istream &in);
expr* parse_addend(std::istream &in);
expr *parse_num(std::istream &in);

static void consume(std::istream &in, int expect);
void skip_whitespace(std::istream &in);


#endif