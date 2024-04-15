#include <iostream>
#include "parse.hpp"

void skip_whitespace(std::istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}

void consume(std::istream &in, int expect) {
    int c = in.get();

    if (c!=expect) {
        std::cout << "expecting: " << (char)expect  << "actual:  " << (char ) c << "\n";
        throw std::runtime_error("consume mismatch");
    }
}

static std::string parse_keyword(std::istream & inn) {
    std::string keyword;

    while (true) {
        int a = inn.peek();
        if (isalpha(a)) {
            consume(inn, a);
            char c = a;
            keyword += c;
        }
        else
            break;
    }

    return keyword;
}

PTR(expr) parse_multicand(std::istream &in) {
    PTR(expr) e = parse_inner(in);
    while (in.peek() == '(') {
        consume(in, '(');
        PTR(expr) actual_arg = parse_expr(in);
        consume(in, ')');
        e = NEW(CallExpr)(e, actual_arg);
    }
    return e;
}

PTR(expr) parse_var(std::istream &in){
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
    return NEW(VarExpr)(var);
}
//todo

PTR(expr) parse_let(std::istream &in){

    //todo// (_let <var> = <expr> _in <expr>)//

//    std::string let = "_let";
//
//    for(char letter : let){
//        int c = in.peek();
//        if(c==letter){
//            consume(in, c);
//        }
//    }

    skip_whitespace(in);

    PTR(expr) var = parse_var(in);

    skip_whitespace(in);

    consume(in, '=');

    skip_whitespace(in);

    PTR(expr) expr1 = parse_expr(in);

    std::string inWord = "_in";

    for(char letter : inWord){
        int c = in.peek();
        if(c==letter){
            consume(in, c);
        }
    }

    skip_whitespace(in);

    PTR(expr) expr2 = parse_expr(in);

    return NEW(LetExpr)(var->to_string(), expr1, expr2);

}

//todo rename to inner for hw 10
PTR(expr) parse_fun(std::istream& in){
    skip_whitespace(in);


    skip_whitespace(in);
    consume(in, '(');
    std::string var = parse_keyword(in);
    skip_whitespace(in);
    consume(in, ')');

    skip_whitespace(in);

    PTR(expr) e = parse_expr(in);

    skip_whitespace(in);

    return NEW(FunExpr)(var, e);

}

PTR(expr) parse_inner(std::istream &in) {
    std::string keyword;

    skip_whitespace(in);
    int c = in.peek();
    if ((c == '-') || isdigit(c)){
        return parse_num(in);
    }
    else if (c == '(') {
        consume(in, '(');
        PTR(expr) e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')'){
            throw std::runtime_error("invalid input ')' ");
        }
        return e;
    }
    else if(isalpha(c)){
        return parse_var(in);
    }
    else if(c == '_'){
        consume(in, '_');
        //return parse_let(in);
        keyword = parse_keyword(in);
        if(keyword == "let"){
            return parse_let(in);
        }
        else if(keyword == "if"){
            return parse_if(in);
        }
        else if(keyword == "true"){
            PTR(expr) e = NEW(BoolExpr)(true);
            return e;
        }
        else if(keyword == "false"){
            PTR(expr) e = NEW(BoolExpr)(false);
            return e;
        }
        else if(keyword == "fun"){
            return parse_fun(in);
        }
    }
    else {
        consume(in, c);
        throw std::runtime_error("invalid input ");
    }
    return nullptr;
}

PTR(expr) parse_if(std::istream &inn){
    skip_whitespace(inn);

    PTR(expr) IfPart = parse_expr(inn);

    skip_whitespace(inn);

    consume(inn,'_');
    consume(inn,'t');
    consume(inn,'h');
    consume(inn,'e');
    consume(inn,'n');

    PTR(expr) ThenPart = parse_expr(inn);

    skip_whitespace(inn);

    consume(inn, '_');
    consume(inn, 'e');
    consume(inn, 'l');
    consume(inn, 's');
    consume(inn, 'e');

    skip_whitespace(inn);

    PTR(expr) ElsePart = parse_expr(inn);

    return NEW(IfExpr)(IfPart, ThenPart, ElsePart);

}


PTR(expr) parse_num(std::istream &inn) {
    int n = 0;
    bool negative = false;

    if (inn.peek() == '-') {
        negative = true;
        //inn.get();
        consume (inn, '-');
        if(!isdigit(inn.peek())){
            throw std::runtime_error("invalid input digit") ;
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

    return NEW(NumExpr)(n);
}

PTR(expr) parse_expr(std::istream &in) {

    PTR(expr) e;

    e = parse_addend(in);

    skip_whitespace(in);

    int c = in.peek();
    if (c == '=') {
        consume(in, '=');
        consume(in, '=');
        PTR(expr) rhs = parse_expr(in);
        return NEW(EqExpr)(e, rhs);
    }
    else if (c == '+') {
        consume(in, '+');
        PTR(expr) rhs = parse_expr(in);
        return NEW(AddExpr)(e, rhs);
    }
    else{
        return  e;
    }
}

PTR(expr) parse_addend(std::istream &in) {

    PTR(expr) e;

    e = parse_multicand(in);

    skip_whitespace(in);

    int c = in.peek();

    while (c == '*') {
        consume(in, '*');
        skip_whitespace(in) ;
        PTR(expr) rhs = parse_addend(in);
        return NEW(MultExpr)(e, rhs);
    }
    return e ;
}


PTR(expr) parse(std::istream &in) {
    PTR(expr) e = parse_expr(in);
    skip_whitespace(in);
    if ( !in.eof() ) {
        throw std::runtime_error("invalid input eof") ;
    }

    return e;
}

PTR(expr) parse_str(const std::string &str) {
    std::istringstream iss(str);
    return parse(iss);
}

//helpers
