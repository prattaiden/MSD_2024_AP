//
// Created by Aiden Pratt on 3/12/24.
//

#include "Val.h"

#include <utility>
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
        return new NumVal((unsigned)val + (unsigned)other_num->val);
}

Val *NumVal::mult_to(Val *other_val) {
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if (other_val == NULL) {
        throw std::runtime_error("multiplication of non-number");
    }
    else {
        return new NumVal((unsigned)val * (unsigned)other_num->val);
    }
}

expr *NumVal::to_expr() {
    return new NumExpr(val);
}

bool NumVal::is_true() {
    throw std::invalid_argument("Cannot evaluate NumVal arguments");
    return false;
}

Val *NumVal::call(Val *actual_arg) const {
    throw std::runtime_error("cannot call numval");
}

void NumVal::print(std::ostream &os) {
    os << std::to_string(val);
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

expr *BoolVal::to_expr() {
    return new BoolExpr(this->TF);
}

bool BoolVal::is_true() {
    if(TF){
        return true;
    }
    else{
        return false;
    }
}

Val *BoolVal::call(Val *actual_arg) const {
    throw std::runtime_error("cannot call boolVal");
}

void BoolVal::print(std::ostream &os) {
    os << (TF ? "_true" : "_false");
}

//------------------------------FUNVAL-------------------------------//
FunVal::FunVal(std::string formal_arg, expr *body) {
    this->formal_arg = formal_arg;
    this-> body = body;
}

expr *FunVal::to_expr() {
    return new FunExpr(this->formal_arg, this->body);
}

bool FunVal::equals(Val *v) {
    FunVal* a = dynamic_cast<FunVal*>(v);
    if(a == nullptr){
        return false;
    }
    else{
        return this->formal_arg == a->formal_arg && this->body == a->body;
    }
}

Val *FunVal::add_to(Val *other_val) {
    throw std::runtime_error("funVal cannot be added");
}

Val *FunVal::mult_to(Val *other_val) {
    throw std::runtime_error("funVal cannot be multiplied");
}

Val *FunVal::call(Val *actual_arg) const {
    return body->subst(formal_arg, actual_arg->to_expr())->interp();
}

bool FunVal::is_true() {
    return false;
}

void FunVal::print(std::ostream &ostream) {
}











