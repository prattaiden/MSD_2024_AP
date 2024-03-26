//
// Created by Aiden Pratt on 1/16/24.
//
#include "expr.h"
#include "catch.h"
#include <utility>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include "Val.h"
#include "pointer.h"

/**
 * \author Aiden Pratt
 * \file expr.cpp
 * \brief contains expression class and children classes: num, add, mult, var
 *
 */


class Val;

//------------------------------------------EXPR--------------------------------------------------------//
/**
* \brief Calculates distance between two points
* \param expr *e argument to check equals with
*/
bool expr::equals(expr *e) {
    return false;
}

/**
 *\brief definition for interp in expr
 *\return 0
 */
PTR(Val) expr::interp() {
    return new NumVal(0);
}

//------------------------------------------NUM--------------------------------------------------------//
/**
 * \brief num constructor implementation
 * \param val
 */
NumExpr :: NumExpr(int val){
    THIS->val = val;
}
/**
 *\brief checking equals
 * \param expression
 * \return true or false if the values are equal to each other
 */
bool NumExpr::equals(expr *e) {
    PTR(NumExpr) n = CAST(NumExpr)(e);///< object dynamic cast to check if parameter expression object is a num
    if(n == nullptr){
        return false;
    }
    else{
        return THIS->val == n->val;
    }
}
/**
 * \brief interps the num value
 * \return the val of this num object
 */
PTR(Val) NumExpr::interp() {
    return NEW(NumVal)(THIS->val);
}
/**
 *\brief checks if this object is a variable
 * \return returns that this num object is not a variable
 */
bool NumExpr::has_variable() {
    return false;
}
/**
 *\brief implementation of subst in num
 * \return returns a new num of the same value
 */
PTR(expr) NumExpr::subst(std::string string, PTR(expr)e) {
    return NEW(NumExpr)(THIS->val);
}
/**
 *\brief prints this value to the provided output stream
 * \param ostream
 */
void NumExpr::print(std::ostream &ostream) {
    ostream << THIS->val;
}

/**
 *\brief prints this value to the stream, calls print
 * \param ostream
 * \param precedence
 */
void NumExpr::pretty_print(std::ostream &ostream, precedence_t p, bool let_needs_parenthesis, int pos) {
    print(ostream);
}
//------------------------------------------VAR--------------------------------------------------------//

/**
 * \brief constructor for var class
 * \param var
 */
VarExpr :: VarExpr(std::string var) {
    THIS->var = var;
}

/**
 * \brief checks if var objects are teh same
 * \param e
 * \return returns true if the vars are the equals
 */
bool VarExpr::equals(PTR(expr)e) {
    PTR(VarExpr)v = CAST(VarExpr)(e);///< object dynamic cast to check if parameter expression object is a var
    if(v == nullptr){
        return false;
    }
    else{
        return THIS->var == v->var;
    }
}

/**
 * \brief interp implementation for var, returns a runtime error
 */
Val * VarExpr::interp(){
    throw std::runtime_error("no value for variable");
}

/**
 * \brief checks if this object is a variable
 * @return returns that this var object is a variable
 */
bool VarExpr::has_variable() {
    return true;
}

/**
 * \brief substitute method implementation for var class
 * \param string
 * \param expression
 * \return returns a new var object
 */
expr* VarExpr::subst(std::string string, PTR(expr)e) {
    if(THIS->var == string){
        return e;

    }
    return new VarExpr(THIS->var);
}

/**
 * \brief print implementation for var class, prints var to ostream
 * @param ostream
 */
void VarExpr::print(std::ostream &ostream) {
    ostream << THIS->var;
}

/**
 * \brief  pretty print implementation for var class
 * \param ostream
 * \param precedence
 */
void VarExpr::pretty_print(std::ostream &ostream, precedence_t p, bool let_pos, int pos) {
    ostream << THIS->var;
}

//------------------------------------------ADD--------------------------------------------------------//

/**
 * \brief add constructor implementation
 * \param left hand side
 * \param right hand side
 */
AddExpr :: AddExpr(PTR(expr)lhs, PTR(expr)rhs){
    THIS->lhs = lhs;
    THIS->rhs = rhs;
}
/**
 *\brief checks if add objects are the same
 * \param expression
 * \return true if the add expressions are equal
 */
bool AddExpr::equals(PTR(expr)e) {
    PTR(AddExpr)a = CAST(AddExpr)(e); ///< object dynamic cast to check if parameter expression object is a add
    if(a == nullptr){
        return false;
    }
    else{
        return (lhs->equals(a->lhs)) && (rhs->equals(a->rhs));
    }
}
/**
 * \brief interps the add object mathematically
 * \return return the mathematical addition equation of of name and value
 */
PTR(Val)AddExpr::interp(){
    return THIS->lhs->interp()->add_to(THIS->rhs->interp());
}

/**
 * \brief checks for a variable in the add object
 * \return returns true if there is a variable in the add objects name or value
 */
bool AddExpr::has_variable() {
    return THIS->lhs->has_variable() || THIS->rhs->has_variable();
}

/**
 * \brief substitutes the passed in string with the passed in expression if applicable
 * \param string
 * \param expression
 * \return returns a new add object with the substituted variables if applicable
 */
PTR(expr)AddExpr::subst(std::string string, PTR(expr)e) {
    return(new AddExpr(THIS->lhs->subst(string, e), (THIS->rhs->subst(string, e))));
}

/**
 * \brief prints the add object to the ostream
 * \param ostream
 */
void AddExpr::print(std::ostream &ostream) {
    ostream << "(";
    (THIS->lhs->print(ostream));
    ostream << "+";
    (THIS->rhs->print(ostream));
    ostream << ")";
}

/**
 * \brief prints the add object to the ostream in a pretty way
 * \param ostream
 *\@param precedence
 */
void AddExpr::pretty_print(std::ostream &ostream, precedence_t p, bool let_needs_parenthesis, int pos) {
    if(p > prec_add){
        ostream << "(";
    }
    (THIS->lhs->pretty_print(ostream, static_cast<precedence_t>(prec_add + 1), true, pos));
    ostream << " + ";
    (THIS->rhs->pretty_print(ostream, prec_none, true, pos));
    if(p > prec_add){
        ostream << ")";
    }
}


//------------------------------------------MULT--------------------------------------------------------//

/**
 * \brief mult constructor implementation
 * \param left hand side
 * \param right hand side
 */
MultExpr :: MultExpr(PTR(expr)lhs, PTR(expr)rhs) {
    THIS->lhs = lhs;
    THIS->rhs = rhs;
}
/**
 * \brief checks if add objects are the same
 * \param expression
 * \return true if the mult expressions are equal
 */
bool MultExpr::equals(PTR(expr)e) {
    PTR(MultExpr)m = CAST(MultExpr)(e);///< object dynamic cast to check if parameter expression object is a mult
    if(m == nullptr){
        return false;
    }
    else{
        return (lhs->equals(m->lhs)) && ((rhs->equals(m->rhs)));
    }

}
/**
 * \brief interps the mult object mathematically
 * \return return the mathematical addition equation of of name and value
 */
PTR(Val)MultExpr::interp(){
    return THIS->lhs->interp()->mult_to(THIS->rhs->interp());
}

/**
 * \brief checks for a variable in the add object
 * \return returns true if there is a variable in the add objects name or value
 */
bool MultExpr::has_variable() {
    return THIS->lhs->has_variable() || THIS->rhs->has_variable();
}

/**
 * \brief substitutes the passed in string with the passed in expression if applicable
 * \param string
 * \param expression
 * \return returns a new add object with the substituted variables if applicable
 */
PTR(expr) MultExpr::subst(std::string string, PTR(expr) e){
    return(new MultExpr(THIS->lhs->subst(string, e), (THIS->rhs->subst(string, e))));
}

/**
 * \brief prints the add object to the ostream
 * \param ostream
 */
void MultExpr::print(std::ostream &ostream) {
    ostream << "(";
    (THIS->lhs->print(ostream));
    ostream << "*";
    (THIS->rhs->print(ostream));
    ostream << ")";
}

/**
 * \brief prints the add object to the ostream in a pretty way
 * \param ostream
 *\@param precedence
 */
void MultExpr::pretty_print(std::ostream &ostream, precedence_t p, bool let_needs_parenthesis, int pos) {

    if(p > prec_mult){
        let_needs_parenthesis = false;
        ostream << "(";
    }
    (THIS->lhs->pretty_print(ostream, static_cast<precedence_t>(prec_mult + 1), let_needs_parenthesis, pos));
    ostream << " * ";
    (THIS->rhs->pretty_print(ostream, prec_mult, let_needs_parenthesis, pos));

    if(p > prec_mult){
        ostream << ")";
    }
}



//------------------------------------------LET--------------------------------------------------------//

LetExpr :: LetExpr(std::string lhs, PTR(expr)rhs, PTR(expr)body) {
    THIS->name = lhs;
    THIS->value = rhs;
    THIS->body = body;
}

bool LetExpr::equals(expr *e) {
    PTR(LetExpr)l = CAST(LetExpr)(e);
        if(l == nullptr){
            return false;
        }
    else{
        return l->name == name && l->value->equals(THIS->value) && l->body->equals(THIS->body);
    }
}

PTR(Val)LetExpr::interp() {
    PTR(expr)newExpr = THIS->body->subst(THIS->name, THIS->value);
    return newExpr->interp();
}

bool LetExpr::has_variable(){
    return THIS->value->has_variable() || THIS->body->has_variable();
};

//Sub if name == string in value
//Sub if name != string when string is contained in value
//Sub if name != string when string is contained in body
PTR(expr)LetExpr::subst(std::string string, PTR(expr)e) {

    if(THIS->name == string){
        return new LetExpr(THIS->name , THIS->value->subst(string, e), THIS->body);
    }
    else{
        return new LetExpr(THIS->name, THIS->value->subst(string, e), THIS->body->subst(string, e));
    }
}

void LetExpr::print(std::ostream &ostream) {
    ostream << "(_let ";
    ostream << THIS->name;
    ostream << "=";
    THIS->value->print(ostream);
    ostream<< " _in ";
    THIS->body->print(ostream);
    ostream << ")";
}

//todo meets let expectations explanation
void LetExpr::pretty_print(std::ostream &ostream, precedence_t p, bool let_needs_parenthesis, int pos) {
    //counts spaces
    if(p > prec_none && let_needs_parenthesis){
        ostream << "(";
    }

    int letPos = ostream.tellp();
    int n = letPos - pos;
    ostream << "_let " << THIS->name << " = " ;

    THIS->value->pretty_print(ostream, p, let_needs_parenthesis, letPos);
    ostream << "\n" << " ";
    //todo might not need this space
    int inPos = ostream.tellp();

    while(n > 0){
        ostream << " ";
        n--;
    }
    ostream << "_in  ";
    THIS->body->pretty_print(ostream, prec_none, let_needs_parenthesis, inPos);

    if(p > prec_none && let_needs_parenthesis){
        ostream << ")";
    }
}


//------------------------------------------EQ--------------------------------------------------------//

EqExpr::EqExpr(PTR(expr)lhs, PTR(expr)rhs) {
    THIS->lhs = lhs;
    THIS->rhs = rhs;
}

bool EqExpr::equals(PTR(expr)e) {
    PTR(EqExpr)a = CAST(EqExpr)(e);
    if(a == nullptr){
        return false;
    }
    else{
        return(lhs->equals(a->lhs) && rhs->equals(a->rhs));
    }
}

PTR(Val)EqExpr::interp() {
    return new (BoolVal)(lhs->interp()->equals(rhs->interp()));
}

void EqExpr::print(std::ostream&  ostream){
    ostream << "(";
    lhs->print(ostream);
    ostream << "==";
    rhs->print(ostream);
    ostream << ")";
}
//todo
void EqExpr::pretty_print(std::ostream &ostream, precedence_t p, bool let_needs_parenthesis, int pos) {
    lhs->pretty_print(ostream, p, false, 0);
    ostream << " == ";
    rhs->pretty_print(ostream,p, false, 0);
}

bool EqExpr::has_variable() {
    return false;
}

PTR(expr)EqExpr::subst(std::string string, expr *e) {
    expr* nextLHS = THIS->lhs->subst(string, e);
    expr* nextRHS = THIS->rhs->subst(string, e);
    return new EqExpr(nextLHS, nextRHS);
}

//------------------------------------------IF--------------------------------------------------------//

IfExpr::IfExpr(PTR(expr)IfPart, PTR(expr)ThenPart, PTR(expr)ElsePart) {
        THIS->IfPart = IfPart;
        THIS->ThenPart = ThenPart;
        THIS->ElsePart = ElsePart;
}

bool IfExpr::equals(expr *e) {
    PTR(IfExpr)a = CAST(IfExpr)(e);
    if(a == nullptr){
        return false;
    }
    else{
        return (IfPart->equals(a->IfPart) && ThenPart->equals(a->ThenPart) && ElsePart->equals(a->ElsePart));
    }
}

PTR(Val)IfExpr::interp() {
    if(IfPart->interp()->is_true()){
        return ThenPart->interp();
    }
    else{
        return ElsePart->interp();
    }
}

void IfExpr::print(std::ostream &ostream) {
    ostream << "(_if ";
    IfPart->print(ostream);
    ostream << "\n" << "_then";
    ThenPart->print(ostream);
    ostream << "\n" << "_else";
    ElsePart->print(ostream);
    ostream << ")";
}
//todo
void IfExpr::pretty_print(std::ostream &ostream, precedence_t p, bool let_needs_parenthesis, int pos) {
    if(p > prec_none && let_needs_parenthesis){
        ostream << "(";
    }

    int ifPos = ostream.tellp();
    int n = ifPos - pos;
    ostream << "_if " << THIS->IfPart;

    THIS->IfPart->pretty_print(ostream, p, let_needs_parenthesis, ifPos);
    ostream << "\n" << " ";
    //todo might not need this space
    int thenPos = ostream.tellp();

    while(n > 0){
        ostream << " ";
        n--;
    }
    ostream << "_then  ";
    THIS->ThenPart->pretty_print(ostream, prec_none, let_needs_parenthesis, thenPos);

    ostream << "\n" << " ";
    int elsePos = ostream.tellp();
    int f = elsePos - pos;

    while(f > 0){
        ostream << " ";
        f--;
    }
    ostream << "_else ";
    THIS->ElsePart->pretty_print(ostream, prec_none, let_needs_parenthesis, elsePos);

    if(p > prec_none && let_needs_parenthesis){
        ostream << ")";
    }
}

bool IfExpr::has_variable() {
    return false;
}

PTR(expr)IfExpr::subst(std::string string, PTR(expr)e) {
    PTR(expr)nextIf = THIS->IfPart->subst(string, e);
    PTR(expr)nextThen = THIS->ThenPart->subst(string, e);
    PTR(expr)nextElse = THIS->ElsePart->subst(string, e);

    return new IfExpr(nextIf, nextThen, nextElse);
}

//------------------------------------------BOOL--------------------------------------------------------//


BoolExpr::BoolExpr(bool TF) {
    THIS->TF = TF;
}

bool BoolExpr::equals(expr *e) {
    PTR(BoolExpr) a = CAST(BoolExpr)(e);
    if(a == nullptr){
        return false;
    }
    else{
        return (THIS->TF == a->TF);
    }
}
//todo
PTR(Val)BoolExpr::interp() {return new BoolVal(TF);}


void BoolExpr::print(std::ostream &ostream) {
    if(TF){
        ostream << "_true";
    }
    else{
        ostream << "_false";
    }
}
//todo
void BoolExpr::pretty_print(std::ostream &ostream, precedence_t p, bool let_needs_parenthesis, int pos) {
    print(ostream);
}

bool BoolExpr::has_variable() {
    return false;
}

PTR(expr)BoolExpr::subst(std::string string, PTR(expr)e) {
    return THIS;
}

//------------------------------------------FUNCTION--------------------------------------------------------//
FunExpr::FunExpr(std::string formal_arg, PTR(expr)body) {
    THIS->formal_arg = formal_arg;
    THIS->body = (body);
}

bool FunExpr::has_variable() {
    return false;
}

PTR(expr)FunExpr::subst(std::string string, expr *e) {
    if(formal_arg == string){
        return THIS;
    }
    else{
        return new FunExpr(formal_arg, body->subst(string, e));
    }
}

bool FunExpr::equals(expr *e) {
    FunExpr* a = dynamic_cast<FunExpr*> (e);
    if(a == nullptr){
        return false;
    }
    return (THIS->formal_arg == a->formal_arg && THIS-> body == a->body);
}


void FunExpr::print(std::ostream &ostream) {
    ostream << "(_fun " << "(" << THIS->formal_arg << ") " << THIS->body->to_string() << ")";
}

void FunExpr::pretty_print(std::ostream &ostream, precedence_t p, bool let_needs_parenthesis, int pos) {
    print(ostream);
    //counts spaces
//    std::streampos initial_pos = ostream.tellp();
//    if(p > prec_none && let_needs_parenthesis){
//        ostream << "(";
//    }
//    ostream << "(_fun (" << this->formal_arg << ") " ;
//    ostream << "\n" << " ";
//    std::streampos body_pos = ostream.tellp();
//
//    this->body->pretty_print(ostream, prec_none, let_needs_parenthesis, body_pos);
//
//    if(p > prec_none && let_needs_parenthesis){
//        ostream << ")";
//    }
//}
}

PTR(Val)FunExpr::interp(){
    return NEW(FunVal) (formal_arg, body);
}

//------------------------------------------CALL--------------------------------------------------------//
CallExpr::CallExpr(expr *to_be_called, expr *actual_arg) {
    THIS-> to_be_called = to_be_called;
    THIS-> actual_arg = actual_arg;
}

bool CallExpr::has_variable() {
    return false;
}

expr *CallExpr::subst(std::string string, expr *e) {
    return new CallExpr(THIS->to_be_called->subst(string, e), THIS->actual_arg->subst(string, e));
}

bool CallExpr::equals(expr *e) {
    CallExpr* a = dynamic_cast<CallExpr*> (e);
    if( a == nullptr){
        return false;
    }
    return(THIS->to_be_called == a->to_be_called && THIS->actual_arg == a->actual_arg);
}

Val *CallExpr::interp() {
    return THIS->to_be_called->interp()->call(actual_arg->interp());
}

void CallExpr::print(std::ostream &ostream) {
    to_be_called->print(ostream);
    ostream << "(";
    actual_arg->print(ostream);
    ostream << ")";

}

void CallExpr::pretty_print(std::ostream &ostream, precedence_t p, bool let_needs_parenthesis, int pos) {
    print(ostream);
}


