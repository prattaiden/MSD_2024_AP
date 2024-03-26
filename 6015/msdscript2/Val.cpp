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
    THIS->val = val;
}

 bool NumVal::equals(Val *e) {
    PTR(NumVal)a = CAST(NumVal)(e);
    if (a == nullptr) {
         return false;
    }
    else {
        return (THIS->val == a->val);
    }
}


PTR(Val)NumVal::add_to(PTR(Val)other_val) {
    PTR(NumVal)other_num = CAST(NumVal)(other_val);
    if (other_val == nullptr) {
        throw std::runtime_error("addition of non-number");
    }
    else
        return new NumVal((unsigned)val + (unsigned)other_num->val);
}

PTR(Val)NumVal::mult_to(PTR(Val)other_val) {
    PTR(NumVal)other_num = CAST(NumVal)(other_val);
    if (other_val == NULL) {
        throw std::runtime_error("multiplication of non-number");
    }
    else {
        return new NumVal((unsigned)val * (unsigned)other_num->val);
    }
}

PTR(expr)NumVal::to_expr() {
    return new NumExpr(val);
}

bool NumVal::is_true() {
    throw std::invalid_argument("Cannot evaluate NumVal arguments");
    return false;
}

PTR(Val)NumVal::call(PTR(Val)actual_arg) const {
    throw std::runtime_error("cannot call numval");
}

void NumVal::print(std::ostream &os) {
    os << std::to_string(val);
}

//-------------------------------BOOlVAL--------------------------------//

BoolVal::BoolVal(bool TF) {
    THIS->TF = TF;
}

bool BoolVal::equals(Val *e) {
    PTR(BoolVal)a = CAST(BoolVal)(e);
    if(a == nullptr){
        return false;
    }
    else{
        return(THIS->TF == a->TF);
    }
}

PTR(Val)BoolVal::add_to(PTR(Val)other_val) {
    throw std::runtime_error("Cannot add a boolean arguments");
}

PTR(Val)BoolVal::mult_to(PTR(Val)other_val) {
    throw std::runtime_error("Cannot multiply a boolean arguments");
}

PTR(expr)BoolVal::to_expr() {
    return new BoolExpr(THIS->TF);
}

bool BoolVal::is_true() {
    if(TF){
        return true;
    }
    else{
        return false;
    }
}

PTR(Val)BoolVal::call(PTR(Val)actual_arg) const {
    throw std::runtime_error("cannot call boolVal");
}

void BoolVal::print(std::ostream &os) {
    os << (TF ? "_true" : "_false");
}

//------------------------------FUNVAL-------------------------------//
FunVal::FunVal(std::string formal_arg, PTR(expr)body) {
    THIS->formal_arg = formal_arg;
    THIS-> body = body;
}

PTR(expr)FunVal::to_expr() {
    return new FunExpr(THIS->formal_arg, THIS->body);
}

bool FunVal::equals(Val *v) {
    PTR(FunVal) a = CAST(FunVal)(v);
    if(a == nullptr){
        return false;
    }
    else{
        return THIS->formal_arg == a->formal_arg && THIS->body == a->body;
    }
}

PTR(Val)FunVal::add_to(PTR(Val)other_val) {
    throw std::runtime_error("funVal cannot be added");
}

PTR(Val)FunVal::mult_to(PTR(Val)other_val) {
    throw std::runtime_error("funVal cannot be multiplied");
}

PTR(Val)FunVal::call(PTR(Val)actual_arg) const {
    return body->subst(formal_arg, actual_arg->to_expr())->interp();
}

bool FunVal::is_true() {
    throw std::runtime_error("true/false not reasonable for FunVal");
}

void FunVal::print(std::ostream &ostream) {
}











