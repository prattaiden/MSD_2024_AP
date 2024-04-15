//
// Created by Aiden Pratt on 3/28/24.
//

#include "Env.h"

PTR(Env) Env::empty = NEW(EmptyEnv) ();

//Env::~Env();



//empty
//EmptyEnv::EmptyEnv() {
//}

PTR(Val) EmptyEnv::lookup(std::string find_name) {
    throw std::runtime_error("free variable: " + find_name);
}

//extended env
ExtendedEnv::ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest) {
    this->name = std::move(name);
    this->val = std::move(val);
    this->rest = std::move(rest);
}

PTR(Val) ExtendedEnv::lookup(std::string find_name) {
        if(find_name == name){
            return val;
        }
        else {
            return rest->lookup(find_name);
        }
}


