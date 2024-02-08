//
// Created by Aiden Pratt on 1/16/24.
//
#include "expr.h"
#include "catch.h"
#include <utility>
#include <stdexcept>
#include <iostream>
#include <sstream>

/**
 * \author Aiden Pratt
 * \file expr.cpp
 * \brief contains expression class and children classes: num, add, mult, var
 *
 */

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
int expr::interp() {
    return 0;
}

//------------------------------------------NUM--------------------------------------------------------//
/**
 * \brief num constructor implementation
 * \param val
 */
Num :: Num(int val){
    this->val = val;
}
/**
 *\brief checking equals
 * \param expression
 * \return true or false if the values are equal to each other
 */
bool Num::equals(expr *e) {
    Num *n = dynamic_cast<Num*>(e);///< object dynamic cast to check if parameter expression object is a num
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
int Num::interp() {
    return val;
}
/**
 *\brief checks if this object is a variable
 * \return returns that this num object is not a variable
 */
bool Num::has_variable() {
    return false;
}
/**
 *\brief implementation of subst in num
 * \return returns a new num of the same value
 */
expr* Num::subst(std::string string, expr *e) {
    return new Num(this->val);
}
/**
 *\brief prints this value to the provided output stream
 * \param ostream
 */
void Num::print(std::ostream &ostream) {
    ostream << this->val;
}

/**
 *\brief prints this value to the stream, calls print
 * \param ostream
 * \param precedence
 */
void Num::pretty_print(std::ostream &ostream, precedence_t p) {
    print(ostream);
}

//------------------------------------------ADD--------------------------------------------------------//

/**
 * \brief add constructor implementation
 * \param left hand side
 * \param right hand side
 */
Add :: Add(expr *lhs, expr *rhs){
    this->lhs = lhs;
    this->rhs = rhs;
}
/**
 *\brief checks if add objects are the same
 * \param expression
 * \return true if the add expressions are equal
 */
bool Add::equals(expr *e) {
    Add *a = dynamic_cast<Add*>(e); ///< object dynamic cast to check if parameter expression object is a add
    if(a == nullptr){
        return false;
    }
    else{
        return (lhs->equals(a->lhs)) && (rhs->equals(a->rhs));
    }
}
/**
 * \brief interps the add object mathematically
 * \return return the mathematical addition equation of of lhs and rhs
 */
int Add::interp(){
    return this->lhs->interp() + this->rhs->interp();
}

/**
 * \brief checks for a variable in the add object
 * \return returns true if there is a variable in the add objects lhs or rhs
 */
bool Add::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

/**
 * \brief substitutes the passed in string with the passed in expression if applicable
 * \param string
 * \param expression
 * \return returns a new add object with the substituted variables if applicable
 */
expr* Add::subst(std::string string, expr *e) {
    return(new Add(this->lhs->subst(string, e),(this->rhs->subst(string,e))));
}

/**
 * \brief prints the add object to the ostream
 * \param ostream
 */
void Add::print(std::ostream &ostream) {
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
void Add::pretty_print(std::ostream &ostream, precedence_t p) {
    if(p > prec_add){
        ostream << "(";
    }
    (this->lhs->pretty_print(ostream, prec_add));
    ostream << " + ";
    (this->rhs->pretty_print(ostream, prec_add));
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
Mult :: Mult(expr *lhs, expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
/**
 * \brief checks if add objects are the same
 * \param expression
 * \return true if the mult expressions are equal
 */
bool Mult::equals(expr *e) {
    Mult *m = dynamic_cast<Mult*>(e);///< object dynamic cast to check if parameter expression object is a mult
    if(m == nullptr){
        return false;
    }
    else{
        return (lhs->equals(m->lhs)) && ((rhs->equals(m->rhs)));
    }

}
/**
 * \brief interps the mult object mathematically
 * \return return the mathematical addition equation of of lhs and rhs
 */
int Mult::interp(){
    return this->lhs->interp() * this->rhs->interp();
}

/**
 * \brief checks for a variable in the add object
 * \return returns true if there is a variable in the add objects lhs or rhs
 */
bool Mult::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

/**
 * \brief substitutes the passed in string with the passed in expression if applicable
 * \param string
 * \param expression
 * \return returns a new add object with the substituted variables if applicable
 */
expr* Mult::subst(std::string string, expr *e) {
    return(new Mult(this->lhs->subst(string, e),(this->rhs->subst(string,e))));
}

/**
 * \brief prints the add object to the ostream
 * \param ostream
 */
void Mult::print(std::ostream &ostream) {
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
void Mult::pretty_print(std::ostream &ostream, precedence_t p) {

    if(p > prec_mult){
        ostream << "(";
    }
        (this->lhs->pretty_print(ostream, static_cast<precedence_t>(prec_mult + 1)));
        ostream << " * ";
        (this->rhs->pretty_print(ostream, prec_mult));

    if(p > prec_mult){
        ostream << ")";
    }
}
//------------------------------------------VAR--------------------------------------------------------//

/**
 * \brief constructor for var class
 * \param var
 */
Var :: Var(std::string var) {
    this->var = var;
}

/**
 * \brief checks if var objects are teh same
 * \param e
 * \return returns true if the vars are the equals
 */
bool Var::equals(expr *e) {
    Var *v = dynamic_cast<Var*>(e);///< object dynamic cast to check if parameter expression object is a var
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
int Var::interp(){
    throw std::runtime_error("no value for variable");
}

/**
 * \brief checks if this object is a variable
 * @return returns that this var object is a variable
 */
bool Var::has_variable() {
    return true;
}

/**
 * \brief substitute method implementation for var class
 * \param string
 * \param expression
 * \return returns a new var object
 */
expr* Var::subst(std::string string, expr *e) {
    if(this->var == string){
        return e;

    }
    return new Var(this->var);
}

/**
 * \brief print implementation for var class, prints var to ostream
 * @param ostream
 */
void Var::print(std::ostream &ostream) {
    ostream << this->var;
}

/**
 * \brief  pretty print implementation for var class
 * \param ostream
 * \param precedence
 */
void Var::pretty_print(std::ostream &ostream, precedence_t p) {
    ostream << this->var;
}

//------------------------------------------LET--------------------------------------------------------//

Let :: Let(std::string lhsVar, expr *rhs, expr *body) {
    this->lhsVar = lhsVar;
    this->rhs = rhs;
    this->body = body;
}

bool Let::equals(expr *e) {
    Let *l = dynamic_cast<Let*>(e);
        if(l == nullptr){
            return false;
        }
    else{
        return l->lhsVar == lhsVar && l->rhs->equals(this->rhs) && l->body->equals(this->body);
    }
}

int Let::interp() {
    //expr new newEXpr = call subst on body
    //return newExpr ->interp

    expr *newExpr = this->body->subst(this->lhsVar, this->rhs);
    return newExpr->interp();
}

bool Let::has_variable(){
    return this->rhs->has_variable() || this->body->has_variable();
};

expr* Let::subst(std::string string, expr *e) {


}

void Let::print(std::ostream &ostream) {
    ostream << "(_let";
    ostream << this->lhsVar;
    ostream << "=";
    this->rhs->print(ostream);
    ostream<< " _in";
    this->body->print(ostream);
    ostream << ")";

    //whatever lhs is in body needs to be replaced with rhs , in body
}

void Let::pretty_print(std::ostream &ostream, precedence_t p) {}





