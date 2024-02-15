#include <iostream>
#include "parse.hpp"


expr *parse_multicand(std::istream &in) {
    skip_whitespace(in);
    int c = in.peek();
    if ((c == '-') || isdigit(c)){
        return parse_num(in);
    }
    else if (c == '(') {
        consume(in, '(');
        expr *e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')'){
            throw std::runtime_error("missing close parenthesis");
           // return e;
        }
    }
    else {
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}

expr *parse_num(std::istream &inn) {
    int n = 0;
    bool negative = false;

    if (inn.peek() == '-') {
        negative = true;
        //inn.get();
        consume (inn, '-');
        if(!isdigit(inn.peek())){
            throw std::runtime_error("num...") ;
        }
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
        n = n * -1;

    return new Num(n);
}

expr *parse_expr(std::istream &in) {

    expr *e;

    e = parse_addend(in);

    skip_whitespace(in);

    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        expr *rhs = parse_expr(in);
        return new Add(e, rhs);
    }
    else{
        return  e;
    }
}

expr *parse_addend(std::istream &in) {

    expr *e;

    e = parse_multicand(in);

    skip_whitespace(in);

    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        skip_whitespace(in) ;
        expr *rhs = parse_addend(in);
        return new Mult(e, rhs);
    }
    else{
        return e ;
    }
}
void consume(std::istream &in, int expect) {
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

expr *parse(std::istream &in) {
    expr* e;
    e = parse_expr(in);
    skip_whitespace(in);
    if ( !in.eof() ) {
        throw std::runtime_error("Invalid input") ;
    }

    return e;
}

expr* parseInput(){
    std::string input;
    getline( std::cin, input);
    std::cout << "input : " << input << std::endl;
    std::stringstream ss(input);
    return parse_expr(ss);

}

//todo
//expr* parse_let(){
//
//}
//expr* parse_var(){
//
//}