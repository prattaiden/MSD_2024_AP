//
// Created by Aiden Pratt on 1/16/24.
//

#ifndef msdscript_EXPR_H
#define msdscript_EXPR_H
#include <string>
#include <sstream>
#include "Val.h"

/**
 * \file expr.h
 * \author Aiden Pratt
 * \brief contains declarations for methods of each class
 */

/**
 * \brief enums that determine precedence values
 * \enum prec_none, prec_add, prec_mult, prec_let
 */
typedef enum {
    prec_none = 0,
    prec_add = 1,
    prec_mult = 2,
} precedence_t;

/**g
 *expression class and parent of num, add, mult, var
 */
//------------------------------------------------------EXPR------------------------------------------------------//


class expr {
public:
    virtual bool equals(expr *e) = 0;//each subclass must override
    virtual Val * interp() = 0;
    virtual bool has_variable() = 0;
    virtual expr* subst(std::string string , expr *e) = 0;
    virtual void print(std::ostream&) = 0;

    std::string to_string(){
        std::stringstream st("");
        this->print(st);
        return st.str();
    }

    std::string to_pp_string(){
        std::stringstream st("");
        this->pretty_print_at(st);
        return st.str();
    }

    void pretty_print_at(std::ostream &ostream){
        this->pretty_print(ostream, prec_none, true, 0);
    }

    virtual void pretty_print(std::ostream &ostream, precedence_t p, bool let_needs_parenthesis, int pos) = 0;
};

//----------------------------------------------NUM-------------------------------------------------------//

/**
 *\brief Num class, child of expression class
 */
class NumExpr : public expr {
public:
    int val; ///< int that will be the Num value
    explicit NumExpr(int val);
    bool equals(expr *e) override;
    Val * interp() override;
    bool has_variable() override;
    expr* subst(std::string string , expr *e) override;
    void print(std::ostream &ostream) override;
    void pretty_print(std::ostream &override, precedence_t p, bool let_needs_parenthesis, int pos) override;

};

//----------------------------------------------------VAR------------------------------------------------//

/**
 *\brief Var class, child of expression class
 */
class VarExpr : public expr{
public:
    std::string var; ///< string that will be the var
    VarExpr(std::string var);
    bool equals(expr *e) override;
    Val * interp() override;
    bool has_variable() override;
    expr* subst(std::string string , expr *e) override;
    void print(std::ostream &ostream) override;
    void pretty_print(std::ostream &override, precedence_t p, bool let_needs_parenthesis, int pos) override;
};

//-----------------------------------------------ADD-----------------------------------------------------//

/**
 *\brief Add class, child of expression class
 */
class AddExpr : public expr{
public:
    expr *lhs; ///< left hands side expression of Add class
    expr *rhs; ///< right hands side expression of Add class
    AddExpr(expr *lhs, expr *rhs);
    bool equals(expr *e) override;
    Val * interp() override;
    bool has_variable() override;
    expr* subst(std::string string , expr *e) override;
    void print(std::ostream &ostream) override;
    void pretty_print(std::ostream &override, precedence_t p, bool let_needs_parenthesis, int pos) override;
};

//----------------------------------------------MULT-----------------------------------------------------//

/**
 *\brief Mult class, child of expression class
 */
class MultExpr : public expr {
public:
    expr *lhs; ///< left hands side expression of Mult class
    expr *rhs; ///< right hands side expression of Mult class
    MultExpr(expr *lhs, expr *rhs);
    bool equals(expr *e) override;
    Val * interp() override;
    bool has_variable() override;
    expr* subst(std::string string , expr *e) override;
    void print(std::ostream &ostream) override;
    void pretty_print(std::ostream &override, precedence_t p, bool let_needs_parenthesis, int pos) override;
};

//----------------------------------------------------_LET------------------------------------------------//
class LetExpr : public expr{
public:
    std::string name;
    expr *value;
    expr *body;
    LetExpr(std::string lhsVar, expr *rhs, expr *body);
    bool equals(expr *e) override;
    Val * interp() override;
    bool has_variable() override;
    expr* subst(std::string string, expr *e) override;
    void print(std::ostream & ostream) override;
    void pretty_print(std::ostream &ostream, precedence_t p, bool let_needs_parenthesis, int pos) override;
};


//----------------------------------------------------_BOOL------------------------------------------------//
class BoolExpr : public expr{
public:

};
//----------------------------------------------------_IF------------------------------------------------//
class IfExpr : public expr{
public:
    Expr*
};

//----------------------------------------------------_EQ------------------------------------------------//

class EqExpr : public Expr {
public:
};

#endif //ASSIGNMENT2_EXPR_H
