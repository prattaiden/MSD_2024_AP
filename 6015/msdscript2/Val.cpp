//
// Created by Aiden Pratt on 3/12/24.
//

#include "Val.h"
#include "expr.h"
#include "Env.h"

bool Val::is_true() {
    return false;
}

//------------------------------------------NUM VAL--------------------------------------------------------//

NumVal::NumVal(int val) {
    this->val = val;
}

 bool NumVal::equals(PTR(Val) e) {
    PTR(NumVal)a = CAST(NumVal)(e);
    if (a == nullptr) {
         return false;
    }
    else {
        return (this->val == a->val);
    }
}


PTR(Val)NumVal::add_to(PTR(Val)other_val) {
    PTR(NumVal)other_num = CAST(NumVal)(other_val);
    if (other_val == nullptr) {
        throw std::runtime_error("addition of non-number");
    }
    else
        return NEW(NumVal)((unsigned)val + (unsigned)other_num->val);
}

PTR(Val)NumVal::mult_to(PTR(Val)other_val) {
    PTR(NumVal)other_num = CAST(NumVal)(other_val);
    if (other_val == nullptr) {
        throw std::runtime_error("multiplication of non-number");
    }
    else {
        return NEW(NumVal)((unsigned)val * (unsigned)other_num->val);
    }
}

PTR(expr)NumVal::to_expr() {
    return NEW(NumExpr)(val);
}

bool NumVal::is_true() {
    throw std::invalid_argument("Cannot evaluate NumVal arguments");
    return false;
}

PTR(Val)NumVal::call(PTR(Val)actual_arg)  {
    throw std::runtime_error("cannot call numval");
}

void NumVal::print(std::ostream &os) {
    os << std::to_string(val);
}

std::string NumVal::to_string(){
    return std::to_string(val);
}

//-------------------------------BOOlVAL--------------------------------//

BoolVal::BoolVal(bool TF) {
    this->TF = TF;
}

bool BoolVal::equals(PTR(Val)e) {
    PTR(BoolVal)a = CAST(BoolVal)(e);
    if(a == nullptr){
        return false;
    }
    else{
        return(this->TF == a->TF);
    }
}

PTR(Val)BoolVal::add_to(PTR(Val)other_val) {
    throw std::runtime_error("Cannot add a boolean arguments");
}

PTR(Val)BoolVal::mult_to(PTR(Val)other_val) {
    throw std::runtime_error("Cannot multiply a boolean arguments");
}

PTR(expr)BoolVal::to_expr() {
    return NEW(BoolExpr)(this->TF);
}

bool BoolVal::is_true() {
    if(TF){
        return true;
    }
    else{
        return false;
    }
}

PTR(Val)BoolVal::call(PTR(Val)actual_arg) {
    throw std::runtime_error("cannot call boolVal");
}

void BoolVal::print(std::ostream &os) {
    os << (TF ? "_true" : "_false");
}

std::string BoolVal::to_string(){
    if(TF){
        return "_true";
    }
    else{
        return "_false";
    }
}

//------------------------------FUNVAL-------------------------------//
FunVal::FunVal(std::string formal_arg, PTR(expr)body, PTR(Env) env) {
    if(env == nullptr){
        env = Env::empty ;
    }
    this->formal_arg = formal_arg;
    this-> body = body;
    this->env = env;
}

PTR(expr)FunVal::to_expr() {
    return NEW(FunExpr)(this->formal_arg, this->body);
}

bool FunVal::equals(PTR(Val)v) {
    PTR(FunVal) a = CAST(FunVal)(v);
    if(a == nullptr){
        return false;
    }
    else{
        return this->formal_arg == a->formal_arg && this->body == a->body;
    }
}

PTR(Val)FunVal::add_to(PTR(Val)other_val) {
    throw std::runtime_error("funVal cannot be added");
}

PTR(Val)FunVal::mult_to(PTR(Val)other_val) {
    throw std::runtime_error("funVal cannot be multiplied");
}

PTR(Val)FunVal::call(PTR(Val)actual_arg) {
    return this->body->interp(NEW(ExtendedEnv)(this->formal_arg, actual_arg, this->env));
}

bool FunVal::is_true() {
    throw std::runtime_error("true/false not reasonable for FunVal");
}

void FunVal::print(std::ostream &ostream){
    ostream << "function";
}

std::string FunVal::to_string(){
    return "[function]";
}











