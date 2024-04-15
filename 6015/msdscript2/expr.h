//
// Created by Aiden Pratt on 1/16/24.
//

#ifndef msdscript_EXPR_H
#define msdscript_EXPR_H
#include <string>
#include <sstream>
#include "pointer.h"


class Val;
class Env;

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


CLASS(expr) {
public:
    virtual ~expr(){};
    virtual bool equals(PTR(expr)e) = 0;//each subclass must override
    virtual PTR(Val) interp(PTR(Env) env = nullptr) = 0 ;
    virtual bool has_variable() = 0;
    virtual PTR(expr) subst(std::string string , PTR(expr)e) = 0;
    virtual void print(std::ostream& ostream) = 0;

    std::string to_string(){
        std::stringstream st("");
        THIS->print(st);
        return st.str();
    }

    std::string to_pp_string(){
        std::stringstream st("");
        THIS->pretty_print_at(st);
        return st.str();
    }

    void pretty_print_at(std::ostream &ostream){
        THIS->pretty_print(ostream, prec_none, true, 0);
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
    bool equals(PTR(expr)e) override;
    PTR(Val) interp(PTR(Env) env = nullptr) override;
    bool has_variable() override;
    PTR(expr) subst(std::string string , PTR(expr)e) override;
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
    bool equals(PTR(expr) e) override;
    PTR (Val) interp(PTR(Env) env = nullptr) override;
    bool has_variable() override;
    PTR (expr) subst(std::string string , PTR(expr)e) override;
    void print(std::ostream &ostream) override;
    void pretty_print(std::ostream &override, precedence_t p, bool let_needs_parenthesis, int pos) override;
};

//-----------------------------------------------ADD-----------------------------------------------------//

/**
 *\brief Add class, child of expression class
 */
class AddExpr : public expr{
public:
    PTR(expr)lhs; ///< left hands side expression of Add class
    PTR(expr)rhs; ///< right hands side expression of Add class
    AddExpr(PTR(expr)lhs, PTR(expr)rhs);
    bool equals(PTR(expr)e) override;
    PTR(Val) interp(PTR(Env) env = nullptr) override;
    bool has_variable() override;
    PTR(expr) subst(std::string string , PTR(expr)e)override;
    void print(std::ostream &ostream) override;
    void pretty_print(std::ostream &override, precedence_t p, bool let_needs_parenthesis, int pos) override;
};

//----------------------------------------------MULT-----------------------------------------------------//

/**
 *\brief Mult class, child of expression class
 */
class MultExpr : public expr {
public:
    PTR(expr)lhs; ///< left hands side expression of Mult class
    PTR(expr)rhs; ///< right hands side expression of Mult class
    MultExpr(PTR(expr)lhs, PTR(expr)rhs);
    bool equals(PTR(expr)e) override;
    PTR(Val) interp(PTR(Env) env = nullptr) override;
    bool has_variable() override;
    PTR(expr) subst(std::string string , PTR(expr)e) override;
    void print(std::ostream &ostream) override;
    void pretty_print(std::ostream &override, precedence_t p, bool let_needs_parenthesis, int pos) override;
};

//----------------------------------------------------_LET------------------------------------------------//
class LetExpr : public expr{
public:
    std::string name;
    PTR(expr)value;
    PTR(expr)body;
    LetExpr(std::string lhsVar, PTR(expr)rhs, PTR(expr)body);
    bool equals(PTR(expr)e) override;
    PTR(Val) interp(PTR(Env) env = nullptr) override;
    bool has_variable() override;
    PTR(expr) subst(std::string string, PTR(expr)e) override;
    void print(std::ostream & ostream) override;
    void pretty_print(std::ostream &ostream, precedence_t p, bool let_needs_parenthesis, int pos) override;
};


//----------------------------------------------------_BOOL------------------------------------------------//
class BoolExpr : public expr{
public:
    bool TF;
    BoolExpr(bool TF);
    bool has_variable() override;
    bool equals(PTR(expr)e) override;
    PTR(expr) subst(std::string string , PTR(expr)e) override;
    PTR(Val) interp(PTR(Env) env = nullptr) override;
    void print(std::ostream& ostream) override;
    void pretty_print(std::ostream& ostream, precedence_t p, bool let_needs_parenthesis, int pos) override;
};
//----------------------------------------------------_IF------------------------------------------------//
class IfExpr : public expr{
public:
    PTR(expr) IfPart;
    PTR(expr) ThenPart;
    PTR(expr) ElsePart;

    IfExpr(PTR(expr) IfPart, PTR(expr) ThenPart, PTR(expr) ElsePart);

    bool has_variable() override;
    bool equals(PTR(expr) e) override;
    PTR(Val) interp(PTR(Env)env = nullptr) override;
    PTR(expr)subst(std::string string , PTR(expr)e) override;
    void print(std::ostream& ostream) override;
    void pretty_print(std::ostream& ostream, precedence_t p, bool let_needs_parenthesis, int pos) override;
};

//----------------------------------------------------_EQ------------------------------------------------//

class EqExpr : public expr {
public:
    PTR(expr) lhs;
    PTR(expr) rhs;
    EqExpr(PTR(expr)lhs, PTR(expr)rhs);
    bool has_variable() override;
    PTR(expr) subst(std::string string , PTR(expr)e) override;
    bool equals(PTR(expr)e) override;
    PTR(Val) interp(PTR(Env) env = nullptr) override;
    void print(std::ostream& ostream) override;
    void pretty_print(std::ostream& ostream, precedence_t p, bool let_needs_parenthesis, int pos) override;

};

//----------------------------------------------------_FUN------------------------------------------------//

class FunExpr : public expr{
public:
    std::string formal_arg;
    PTR(expr)body;
    bool has_variable() override;
    PTR(expr) subst(std::string string , PTR(expr)e) override;
    bool equals(PTR(expr)e) override;
    PTR(Val) interp(PTR(Env) env = nullptr) override;
    void print(std::ostream& ostream) override;
    void pretty_print(std::ostream& ostream, precedence_t p, bool let_needs_parenthesis, int pos) override;
    FunExpr(std::string formal_arg, PTR(expr) body);
};


//------------------------------------------CALL--------------------------------------------------------//

class CallExpr : public expr{
public:
    PTR(expr) to_be_called;
    PTR(expr) actual_arg;
    CallExpr(PTR(expr) to_be_called, PTR(expr) actual_arg);
    bool has_variable() override;
    PTR(expr) subst(std::string string , PTR(expr)e) override;
    bool equals(PTR(expr)e) override;
    PTR(Val) interp(PTR(Env)env = nullptr) override;
    void print(std::ostream& ostream) override;
    void pretty_print(std::ostream& ostream, precedence_t p, bool let_needs_parenthesis, int pos) override;
};

#endif //ASSIGNMENT2_EXPR_H