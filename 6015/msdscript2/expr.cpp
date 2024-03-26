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
Val * expr::interp() {
    return new NumVal(0);
}

//------------------------------------------NUM--------------------------------------------------------//
/**
 * \brief num constructor implementation
 * \param val
 */
NumExpr :: NumExpr(int val){
    this->val = val;
}
/**
 *\brief checking equals
 * \param expression
 * \return true or false if the values are equal to each other
 */
bool NumExpr::equals(expr *e) {
    NumExpr *n = dynamic_cast<NumExpr*>(e);///< object dynamic cast to check if parameter expression object is a num
    if(n == nullptr){
        return false;
    }
    else{
        return this->val == n->val;
    }
}
/**
 * \brief interps the num value
 * \return the val of this num object
 */
Val * NumExpr::interp() {
    return new NumVal(this->val);
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
expr* NumExpr::subst(std::string string, expr *e) {
    return new NumExpr(this->val);
}
/**
 *\brief prints this value to the provided output stream
 * \param ostream
 */
void NumExpr::print(std::ostream &ostream) {
    ostream << this->val;
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
    this->var = var;
}

/**
 * \brief checks if var objects are teh same
 * \param e
 * \return returns true if the vars are the equals
 */
bool VarExpr::equals(expr *e) {
    VarExpr *v = dynamic_cast<VarExpr*>(e);///< object dynamic cast to check if parameter expression object is a var
    if(v == nullptr){
        return false;
    }
    else{
        return this->var == v->var;
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
expr* VarExpr::subst(std::string string, expr *e) {
    if(this->var == string){
        return e;

    }
    return new VarExpr(this->var);
}

/**
 * \brief print implementation for var class, prints var to ostream
 * @param ostream
 */
void VarExpr::print(std::ostream &ostream) {
    ostream << this->var;
}

/**
 * \brief  pretty print implementation for var class
 * \param ostream
 * \param precedence
 */
void VarExpr::pretty_print(std::ostream &ostream, precedence_t p, bool let_pos, int pos) {
    ostream << this->var;
}

//------------------------------------------ADD--------------------------------------------------------//

/**
 * \brief add constructor implementation
 * \param left hand side
 * \param right hand side
 */
AddExpr :: AddExpr(expr *lhs, expr *rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}
/**
 *\brief checks if add objects are the same
 * \param expression
 * \return true if the add expressions are equal
 */
bool AddExpr::equals(expr *e) {
    AddExpr *a = dynamic_cast<AddExpr*>(e); ///< object dynamic cast to check if parameter expression object is a add
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
Val * AddExpr::interp(){
    return this->lhs->interp()->add_to(this->rhs->interp());
}

/**
 * \brief checks for a variable in the add object
 * \return returns true if there is a variable in the add objects name or value
 */
bool AddExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

/**
 * \brief substitutes the passed in string with the passed in expression if applicable
 * \param string
 * \param expression
 * \return returns a new add object with the substituted variables if applicable
 */
expr* AddExpr::subst(std::string string, expr *e) {
    return(new AddExpr(this->lhs->subst(string, e), (this->rhs->subst(string, e))));
}

/**
 * \brief prints the add object to the ostream
 * \param ostream
 */
void AddExpr::print(std::ostream &ostream) {
    ostream << "(";
    (this->lhs->print(ostream));
    ostream << "+";
    (this->rhs->print(ostream));
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
    (this->lhs->pretty_print(ostream, static_cast<precedence_t>(prec_add + 1), true, pos));
    ostream << " + ";
    (this->rhs->pretty_print(ostream, prec_none, true, pos));
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
MultExpr :: MultExpr(expr *lhs, expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
/**
 * \brief checks if add objects are the same
 * \param expression
 * \return true if the mult expressions are equal
 */
bool MultExpr::equals(expr *e) {
    MultExpr *m = dynamic_cast<MultExpr*>(e);///< object dynamic cast to check if parameter expression object is a mult
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
Val * MultExpr::interp(){
    return this->lhs->interp()->mult_to(this->rhs->interp());
}

/**
 * \brief checks for a variable in the add object
 * \return returns true if there is a variable in the add objects name or value
 */
bool MultExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

/**
 * \brief substitutes the passed in string with the passed in expression if applicable
 * \param string
 * \param expression
 * \return returns a new add object with the substituted variables if applicable
 */
expr* MultExpr::subst(std::string string, expr *e) {
    return(new MultExpr(this->lhs->subst(string, e), (this->rhs->subst(string, e))));
}

/**
 * \brief prints the add object to the ostream
 * \param ostream
 */
void MultExpr::print(std::ostream &ostream) {
    ostream << "(";
    (this->lhs->print(ostream));
    ostream << "*";
    (this->rhs->print(ostream));
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
    (this->lhs->pretty_print(ostream, static_cast<precedence_t>(prec_mult + 1), let_needs_parenthesis, pos));
    ostream << " * ";
    (this->rhs->pretty_print(ostream, prec_mult, let_needs_parenthesis, pos));

    if(p > prec_mult){
        ostream << ")";
    }
}



//------------------------------------------LET--------------------------------------------------------//

LetExpr :: LetExpr(std::string lhs, expr *rhs, expr *body) {
    this->name = lhs;
    this->value = rhs;
    this->body = body;
}

bool LetExpr::equals(expr *e) {
    LetExpr *l = dynamic_cast<LetExpr*>(e);
        if(l == nullptr){
            return false;
        }
    else{
        return l->name == name && l->value->equals(this->value) && l->body->equals(this->body);
    }
}

Val * LetExpr::interp() {
    expr *newExpr = this->body->subst(this->name, this->value);
    return newExpr->interp();
}

bool LetExpr::has_variable(){
    return this->value->has_variable() || this->body->has_variable();
};

//Sub if name == string in value
//Sub if name != string when string is contained in value
//Sub if name != string when string is contained in body
expr* LetExpr::subst(std::string string, expr *e) {

    if(this->name == string){
        return new LetExpr(this->name , this->value->subst(string, e), this->body);
    }
    else{
        return new LetExpr(this->name, this->value->subst(string, e), this->body->subst(string, e));
    }
}

void LetExpr::print(std::ostream &ostream) {
    ostream << "(_let ";
    ostream << this->name;
    ostream << "=";
    this->value->print(ostream);
    ostream<< " _in ";
    this->body->print(ostream);
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
    ostream << "_let " << this->name << " = " ;

    this->value->pretty_print(ostream, p, let_needs_parenthesis, letPos);
    ostream << "\n" << " ";
    //todo might not need this space
    int inPos = ostream.tellp();

    while(n > 0){
        ostream << " ";
        n--;
    }
    ostream << "_in  ";
    this->body->pretty_print(ostream, prec_none, let_needs_parenthesis, inPos);

    if(p > prec_none && let_needs_parenthesis){
        ostream << ")";
    }
}


//------------------------------------------EQ--------------------------------------------------------//

EqExpr::EqExpr(expr* lhs, expr* rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqExpr::equals(expr *e) {
    EqExpr * a = dynamic_cast<EqExpr*>(e);
    if(a == nullptr){
        return false;
    }
    else{
        return(lhs->equals(a->lhs) && rhs->equals(a->rhs));
    }
}

Val *EqExpr::interp() {
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

expr *EqExpr::subst(std::string string, expr *e) {
    expr* nextLHS = this->lhs->subst(string, e);
    expr* nextRHS = this->rhs->subst(string, e);
    return new EqExpr(nextLHS, nextRHS);
}

//------------------------------------------IF--------------------------------------------------------//

IfExpr::IfExpr(expr *IfPart, expr *ThenPart, expr *ElsePart) {
        this->IfPart = IfPart;
        this->ThenPart = ThenPart;
        this->ElsePart = ElsePart;
}

bool IfExpr::equals(expr *e) {
    IfExpr *a = dynamic_cast<IfExpr*>(e);
    if(a == nullptr){
        return false;
    }
    else{
        return (IfPart->equals(a->IfPart) && ThenPart->equals(a->ThenPart) && ElsePart->equals(a->ElsePart));
    }
}

Val *IfExpr::interp() {
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
    ostream << "_if " << this->IfPart;

    this->IfPart->pretty_print(ostream, p, let_needs_parenthesis, ifPos);
    ostream << "\n" << " ";
    //todo might not need this space
    int thenPos = ostream.tellp();

    while(n > 0){
        ostream << " ";
        n--;
    }
    ostream << "_then  ";
    this->ThenPart->pretty_print(ostream, prec_none, let_needs_parenthesis, thenPos);

    ostream << "\n" << " ";
    int elsePos = ostream.tellp();
    int f = elsePos - pos;

    while(f > 0){
        ostream << " ";
        f--;
    }
    ostream << "_else ";
    this->ElsePart->pretty_print(ostream, prec_none, let_needs_parenthesis, elsePos);

    if(p > prec_none && let_needs_parenthesis){
        ostream << ")";
    }
}

bool IfExpr::has_variable() {
    return false;
}

expr* IfExpr::subst(std::string string, expr *e) {
    expr* nextIf = this->IfPart->subst(string, e);
    expr* nextThen = this->ThenPart->subst(string, e);
    expr* nextElse = this->ElsePart->subst(string, e);

    return new IfExpr(nextIf, nextThen, nextElse);
}

//------------------------------------------BOOL--------------------------------------------------------//


BoolExpr::BoolExpr(bool TF) {
    this->TF = TF;
}

bool BoolExpr::equals(expr *e) {
    BoolExpr* a = dynamic_cast<BoolExpr*> (e);
    if(a == nullptr){
        return false;
    }
    else{
        return (this->TF == a->TF);
    }
}
//todo
Val *BoolExpr::interp() {return new BoolVal(TF);}


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

expr *BoolExpr::subst(std::string string, expr *e) {
    return this;
}

//------------------------------------------FUNCTION--------------------------------------------------------//
FunExpr::FunExpr(std::string formal_arg, expr *body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::has_variable() {
    return false;
}

expr *FunExpr::subst(std::string string, expr *e) {
    if(formal_arg == string){
        return this;
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
    return (this->formal_arg == a->formal_arg && this-> body == a->body);
}


void FunExpr::print(std::ostream &ostream) {
    ostream << "(_fun " << "(" << this->formal_arg << ") " << this->body->to_string() << ")";
}

void FunExpr::pretty_print(std::ostream &ostream, precedence_t p, bool let_needs_parenthesis, int pos) {
    //print(ostream);
    //counts spaces
    std::streampos initial_pos = ostream.tellp();
    if(p > prec_none && let_needs_parenthesis){
        ostream << "(";
    }
    ostream << "(_fun (" << this->formal_arg << ") " ;
    ostream << "\n" << " ";
    std::streampos body_pos = ostream.tellp();

    this->body->pretty_print(ostream, prec_none, let_needs_parenthesis, body_pos);

    if(p > prec_none && let_needs_parenthesis){
        ostream << ")";
    }
}

Val* FunExpr::interp(){
    return new FunVal(formal_arg, body);
}

//------------------------------------------CALL--------------------------------------------------------//
CallExpr::CallExpr(expr *to_be_called, expr *actual_arg) {
    this-> to_be_called = to_be_called;
    this-> actual_arg = actual_arg;
}

bool CallExpr::has_variable() {
    return false;
}

expr *CallExpr::subst(std::string string, expr *e) {
    return new CallExpr(this->to_be_called->subst(string, e), this->actual_arg->subst(string, e));
}

bool CallExpr::equals(expr *e) {
    CallExpr* a = dynamic_cast<CallExpr*> (e);
    if( a == nullptr){
        return false;
    }
    return(this->to_be_called == a->to_be_called && this->actual_arg == a->actual_arg);
}

Val *CallExpr::interp() {
    return this->to_be_called->interp()->call(actual_arg->interp());
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


