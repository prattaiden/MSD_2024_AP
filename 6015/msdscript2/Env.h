//
// Created by Aiden Pratt on 3/28/24.
//

#ifndef MSDSCRIPT2_ENV_H
#define MSDSCRIPT2_ENV_H

#include "pointer.h"
#include "val.h"

class Env {
public:
    static PTR(Env) empty;
    virtual PTR(Val) lookup(std::string find_name) = 0;

};

class EmptyEnv : public Env{
public:
    EmptyEnv();

    PTR(Val) lookup(std::string find_name){
        throw std::runtime_error("free variable: " + find_name);
    }
};

class ExtendedEnv : public Env{
public:
    std::string name;
    PTR(Val) val;
    PTR(Env) rest;
    ExtendedEnv(std::string name, PTR(Val) val , PTR(Env) rest);

    PTR(Val) lookup(std::string find_name){
        if(find_name == name){
            return val;
        }
        else{
            return rest->lookup(find_name);
        }
    }
};


#endif //MSDSCRIPT2_ENV_H
