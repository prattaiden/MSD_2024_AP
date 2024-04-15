//
// Created by Aiden Pratt on 3/28/24.
//

#include "Env.h"

PTR(Env) Env::empty = NEW(EmptyEnv) ();



//empty
EmptyEnv::EmptyEnv() {
}

PTR(Val) EmptyEnv::lookup(std::string find_name) {
    throw std::runtime_error("free variable: " + find_name);
}

//extended env
ExtendedEnv::ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest) {
    this->name = name;
    this->val = val;
    this->rest = rest;
}

std::shared_ptr<Val> ExtendedEnv::lookup(std::string find_name) {
        if(find_name == name){
            return val;
        }
        else {
            return rest->lookup(find_name);
        }
}


