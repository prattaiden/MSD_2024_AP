#include <iostream>
#include "parse.hpp"



expr *parse_multicand (std::istream & inn) {
    skip_whitespace (inn);

    int c = inn.peek();
    if ((c == '-') || isdigit(c))
        return parse_num(inn);
    else if (c == '(') {
        consume(inn, '(');
        expr *e = parse_expr(inn);
        skip_whitespace(inn);
        c = inn.get();
        if (c != ')')
            throw std::runtime_error("missing close parenthesis");
        return e;
    } else {
        consume(inn, c);
        throw std::runtime_error("invalid input");
    }
}

expr *parse_addend(std::istream &in) {
    expr *e;
    e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        expr *rhs = parse_expr(in);
        return new Add(e, rhs);
    } else
        return e;
}

static expr *parse_expr(std::istream &in) {
    expr *e;
    //read in LHS
    e = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();

    //Check if add expression
    if (c == '+') {
        //consume "+", after plus is the rhs <expr>
        consume(in, '+');
        expr *rhs = parse_expr(in);
        return new Add(e, rhs);
    } else
        return e;
}


expr *parse_num(std::istream &inn) {
    int n = 0;
    bool negative = false;

    if (inn.peek() == '-') {
        negative = true;
        consume (inn, '-');

    }

    while (1) {
        int c = inn.peek();
        if (isdigit(c))
        {
            consume(inn, c);
            n = n*10 + (c - '0');
        }
        else
            break;
    }

    if (negative)
        n = -n;

    return new Num(n);
}


static void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c!=expect) {
        throw std::runtime_error("consume mismatch");
    }
}

void skip_whitespace(std::istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}