//
// Created by Aiden Pratt on 3/28/24.
//

#ifndef MSDSCRIPT2_ENV_H
#define MSDSCRIPT2_ENV_H

#include <string>
#include "pointer.h"

class Val;
class expr;


class Env {
public:
    static PTR(Env) empty;
    virtual PTR(Val) lookup(std::string find_name) = 0;

};

class EmptyEnv : public Env{
public:
    EmptyEnv();

    PTR(Val) lookup(std::string find_name) override;
};

class ExtendedEnv : public Env{
public:
    std::string name;
    PTR(Val) val;
    PTR(Env) rest;
    ExtendedEnv(std::string name, PTR(Val) val , PTR(Env) rest);

    PTR(Val) lookup(std::string find_name) override;
};


#endif //MSDSCRIPT2_ENV_H
