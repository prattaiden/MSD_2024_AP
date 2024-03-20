//
// Created by Aiden Pratt on 3/12/24.
//

#ifndef MSDSCRIPT2_VAL_H
#define MSDSCRIPT2_VAL_H

#include <iostream>

class expr;

class Val {
public:
    virtual bool equals(Val *e) = 0;
    virtual Val* add_to(Val* other_val) = 0;
    virtual Val* mult_to(Val* other_val) = 0;
    virtual std::string to_string() = 0;
    virtual expr* to_expr() = 0;
    virtual bool is_true() = 0;

};


class NumVal : public Val {
public:
    int val;
    explicit NumVal(int val);
    bool equals(Val *e) override;
    Val* add_to(Val* other_val) override;
    Val* mult_to(Val* other_val) override;
    std::string to_string() override;
    expr* to_expr() override;
    bool is_true() override;

};

class BoolVal : public Val{
public:
    bool TF;
    BoolVal(bool TF);
    bool equals(Val *e) override;
    Val* add_to(Val* other_val) override;
    Val* mult_to(Val* other_val) override;
    std::string to_string() override;
    expr* to_expr() override;
    bool is_true() override;
};

class FunVal : public Val{
public:
    std::string formal_arg;
    expr *body;
    explicit FunVal(std::string formal_arg, expr *body);
    expr* to_expr() override;
    bool equals (Val *v) override;
    Val* add_to(Val* other_val) override;
    Val* mult_to(Val* other_val) override;
    //void print(std::ostream &ostream);
    bool is_true() override;
    Val* call(Val* actual_arg);
};

#endif //MSDSCRIPT2_VAL_H
