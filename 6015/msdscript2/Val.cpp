//
// Created by Aiden Pratt on 3/12/24.
//

#include "Val.h"
#include "expr.h"

bool Val::is_true() {
    return false;
}

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
    else {
        return new NumVal(val * other_num->val);
    }
}

std::string NumVal::to_string() {
    return std::to_string(val);
}

Expr *NumVal::to_expr() {
    return nullptr;
}

bool NumVal::is_true() {
    throw std::invalid_argument("Cannot evaluate NumVal arguments");
}

//-------------------------------BOOlVAL--------------------------------//

BoolVal::BoolVal(bool TF) {
    this->TF = TF;
}

bool BoolVal::equals(Val *e) {
    BoolVal *a = dynamic_cast<BoolVal*>(e);
    if(a == nullptr){
        return false;
    }
    else{
        return(this->TF == a->TF);
    }
}

Val *BoolVal::add_to(Val *other_val) {
    throw std::runtime_error("Cannot add a boolean arguments");
}

Val *BoolVal::mult_to(Val *other_val) {
    throw std::runtime_error("Cannot multiply a boolean arguments");
}

std::string BoolVal::to_string() {
    if(TF){
        return "_true";
    }
    else{
        return "_false";
    }
}

Expr *BoolVal::to_expr() {
    return nullptr;
}

bool BoolVal::is_true() {
    if(TF){
        return true;
    }
    else{
        return false;
    }
}
