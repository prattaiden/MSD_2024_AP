#include <iostream>
#include "parse.hpp"


expr* parse_var(std::istream &in){
    std::string var;
    while(true){
        int c = in.peek();
        if(isalpha(c)){
            consume(in, c);
            var += static_cast<char>(c);
        }else{
            break;
        }
    }
    return new VarExpr(var);
}
//todo
expr* parse_let(std::istream &in){

    //todo// (_let <var> = <expr> _in <expr>)//

    std::string let = "_let";

    for(char letter : let){
        int c = in.peek();
        if(c==letter){
            consume(in, c);
        }
    }

    skip_whitespace(in);

    expr *var = parse_var(in);

    skip_whitespace(in);

    consume(in, '=');

    skip_whitespace(in);

    expr *expr1 = parse_expr(in);

    std::string inWord = "_in";

    for(char letter : inWord){
        int c = in.peek();
        if(c==letter){
            consume(in, c);
        }
    }

    skip_whitespace(in);

    expr *expr2 = parse_expr(in);

    return new LetExpr(var->to_string(), expr1, expr2);

}

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
            throw std::runtime_error("invalid input");
        }
        return e;
    }
    else if(isalpha(c)){
        return parse_var(in);
    }
    else if(c == '_'){
        return parse_let(in);
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
            throw std::runtime_error("invalid input") ;
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

    return new NumExpr(n);
}

expr *parse_expr(std::istream &in) {

    expr *e;

    e = parse_addend(in);

    skip_whitespace(in);

    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        expr *rhs = parse_expr(in);
        return new AddExpr(e, rhs);
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
        return new MultExpr(e, rhs);
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
        throw std::runtime_error("invalid input") ;
    }

    return e;
}


expr* parse_str(const std::string &str) {
    std::istringstream iss(str);
    return parse(iss);
}

//helpers
