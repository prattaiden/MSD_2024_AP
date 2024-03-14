//
// Created by Aiden Pratt on 3/12/24.
//

#ifndef MSDSCRIPT2_VAL_H
#define MSDSCRIPT2_VAL_H

#include <iostream>

class Expr;

class Val {
public:
    virtual bool equals(Val *e) = 0;
    virtual Val* add_to(Val* other_val) = 0;
    virtual Val* mult_to(Val* other_val) = 0;
    virtual std::string to_string() = 0;
    virtual Expr* to_expr() = 0;
    virtual bool is_true();

};


class NumVal : public Val {
public:
    int val;
    explicit NumVal(int val);
    bool equals(Val *e) override;
    Val* add_to(Val* other_val) override;
    Val* mult_to(Val* other_val) override;
    std::string to_string() override;
    Expr* to_expr() override;
    bool is_true() override;

};

class BoolVal : public Val{
public:
    bool TF;
    explicit BoolVal(bool TF);
    bool equals(Val *e) override;
    Val* add_to(Val* other_val) override;
    Val* mult_to(Val* other_val) override;
    std::string to_string() override;
    Expr* to_expr() override;
    bool is_true() override;
};

#endif //MSDSCRIPT2_VAL_H
