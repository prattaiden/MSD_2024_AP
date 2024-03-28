//
// Created by Aiden Pratt on 3/28/24.
//

#include "Env.h"

PTR(Env) Env::empty = NEW(EmptyEnv) ();

EmptyEnv::EmptyEnv() {

}


ExtendedEnv::ExtendedEnv(std::string name, std::shared_ptr<Val> val, std::shared_ptr<Env> rest) {
    this->name = name;
    this->val = val;
    this->rest = rest;
}
