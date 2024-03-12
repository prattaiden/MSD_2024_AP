//
// Created by Aiden Pratt on 3/12/24.
//

#include "Val.h"
#include "expr.h"

//------------------------------------------NUM VAL--------------------------------------------------------//

NumVal::NumVal(int val) {
    this->val = val;
}

 bool NumVal::equals(Val *e) {
    NumVal *a = dynamic_cast<NumVal*>(e);
     if (a == nullptr) {
         return false;
     }
     else
         return (this->val == a->val);
}


Val *NumVal::add_to(Val *other_val) {
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if (other_val == nullptr) {
        throw std::runtime_error("addition of non-number");
    }
    else
        return new NumVal(val + other_num->val);
}

Val *NumVal::mult_to(Val *other_val) {
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if (other_val == NULL) {
        throw std::runtime_error("multiplication of non-number");
    }
    else
        return new NumVal(val * other_num->val);
}

std::string NumVal::to_string() {
    return std::to_string(val);
}

