#include <iostream>
#include "parse.hpp"

expr *parse_num(std::istream &inn) {
    int n = 0;
    bool isNegative = false;

    while (true) {
        int c = inn.get();

        if(inn.peek() == '-'){
            isNegative = true;
            inn.get(); //consume, remove it

        }

        if (isdigit(c))
        {
            n = n*10 + (c - '0');
        }
        else
            break;
    }

    if(isNegative){
        n = -n;
    }
    
    return new Num(n);
}
