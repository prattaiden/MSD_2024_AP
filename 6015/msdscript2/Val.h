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

};

class NumVal : public Val {
public:
    int val;
    explicit NumVal(int val);

    bool equals(Val *e) override;
    Val* add_to(Val* other_val) override;
    Val* mult_to(Val* other_val) override;
    std::string to_string() override;
};

#endif //MSDSCRIPT2_VAL_H
