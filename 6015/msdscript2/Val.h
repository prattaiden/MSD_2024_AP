//
// Created by Aiden Pratt on 3/12/24.
//

#ifndef MSDSCRIPT2_VAL_H
#define MSDSCRIPT2_VAL_H

#include <string>
#include <iostream>
#include "expr.h"
#include "pointer.h"

class expr;

CLASS (Val) {
public:
    virtual bool equals(PTR(Val) e) = 0;
    virtual PTR(Val) add_to(PTR(Val) other_val) = 0;
    virtual PTR(Val) mult_to(PTR(Val) other_val) = 0;
    virtual void print(std::ostream &os) = 0;
    virtual PTR(expr) to_expr() = 0;
    virtual bool is_true() = 0;
    virtual PTR(Val) call(PTR(Val) actual_arg) const = 0;

    std::string to_string(){
        std::stringstream st("");
        THIS->print(st);
        return st.str();
    }
};


class NumVal : public Val {
public:
    int val;
    explicit NumVal(int val);
    bool equals(PTR(Val)e) override;
    virtual void print(std::ostream &os) override;
    PTR(Val) add_to(PTR(Val) other_val) override;
    PTR(Val) mult_to(PTR(Val) other_val) override;
    PTR(expr) to_expr() override;
    bool is_true() override;
    PTR(Val) call(PTR(Val) actual_arg) const override;

};

class BoolVal : public Val{
public:
    bool TF;
    BoolVal(bool TF);
    bool equals(PTR(Val)e) override;
    virtual void print(std::ostream &os) override;
    PTR(Val) add_to(PTR(Val) other_val) override;
    PTR(Val) mult_to(PTR(Val) other_val) override;
    PTR(expr) to_expr() override;
    bool is_true() override;
    PTR(Val) call(PTR(Val) actual_arg) const override;
};

class FunVal : public Val{
public:
    std::string formal_arg;
    PTR(expr) body;
    FunVal(std::string formal_arg, expr *body);
    PTR(expr) to_expr() override;
    bool equals (PTR(Val)v) override;
    PTR(Val) add_to(PTR(Val) other_val) override;
    PTR(Val) mult_to(PTR(Val) other_val) override;
    void print(std::ostream &ostream) override;
    bool is_true() override;
    PTR(Val) call(PTR(Val) actual_arg) const override;
};

#endif //MSDSCRIPT2_VAL_H
