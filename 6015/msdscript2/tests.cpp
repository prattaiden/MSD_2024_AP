#include <iostream>
#include "catch.h"
#include "expr.h"
#include "parse.hpp"
#include "Val.h"
#include "pointer.h"

//
TEST_CASE("Nabil_Test_Cases"){
    CHECK((NEW(VarExpr)("x"))->equals(NEW(VarExpr)("x")) == true );
    CHECK((NEW (VarExpr)("x"))->equals(NEW (VarExpr)("y")) == false );
    CHECK((NEW (NumExpr)(1))->equals(NEW (VarExpr)("x")) == false );
    CHECK((NEW (AddExpr)(NEW (NumExpr)(2), NEW (NumExpr)(3)))->equals(NEW (AddExpr)(NEW (NumExpr)(3), NEW (NumExpr)(2))) == false );
    CHECK((NEW (AddExpr)(NEW(NumExpr)(2), NEW (NumExpr)(3)))->equals(NEW (AddExpr)(NEW (NumExpr)(2), NEW (NumExpr)(3))) == true );
}


TEST_CASE("Test_Mult"){
    CHECK((NEW (MultExpr)(NEW (NumExpr)(4), NEW (NumExpr)(10)))->equals(NEW (MultExpr)(NEW (NumExpr) (4), NEW(NumExpr)(10))) == true);
    CHECK((NEW (MultExpr)(NEW (NumExpr)(2), NEW(NumExpr)(3)))->equals(NEW (MultExpr)(NEW (NumExpr)(3), NEW(NumExpr)(2))) == false);
    //testing that casting the mult on the add will just be false
    CHECK((NEW(MultExpr) (NEW(NumExpr)(2), NEW(NumExpr) (3)))->equals(NEW(AddExpr) (NEW(NumExpr)(3), NEW(NumExpr)(2))) == false);
    //testing when plugging a Var in instead of a Num
    CHECK((NEW(MultExpr)(NEW(VarExpr)("2"), NEW (VarExpr)("3")))->equals(NEW(MultExpr) (NEW(VarExpr)("3"), NEW(VarExpr)("2"))) == false);
    CHECK((NEW(MultExpr) (NEW(VarExpr)("2"), NEW(VarExpr)("3")))->equals(NEW(MultExpr) (NEW(VarExpr)("2"), NEW(VarExpr)("3"))) == true);
}

TEST_CASE("Test_Num"){
    CHECK((NEW(NumExpr)(5))->equals((NEW(NumExpr) (5))) == true);
    CHECK((NEW(NumExpr)(6))->equals((NEW(NumExpr) (2))) == false);
    //testing casting var and num will be false
    CHECK((NEW(NumExpr)(6))->equals((NEW(VarExpr) ("6"))) == false);

}

TEST_CASE("Test_Var"){
    CHECK((NEW(VarExpr)("f"))->equals((NEW (VarExpr) ("f"))) == true);
    CHECK((NEW(VarExpr)("v"))->equals((NEW(VarExpr) ("r"))) == false);
    //testing casting Num and Var will be false
    CHECK((NEW(VarExpr)("6"))->equals((NEW(NumExpr) (6))) == false);
    CHECK((NEW( VarExpr)("6"))->equals((NEW(VarExpr) ("6"))) == true);
}

//
TEST_CASE("TEST_Interp_Nabil"){
    CHECK_THROWS_WITH((NEW(VarExpr)("x"))->interp(Env::empty), "no value for variable" );
    CHECK((NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(2)))->interp(Env::empty)->equals(NEW(NumVal)(6)) );
    CHECK((NEW(AddExpr) (NEW(AddExpr) (NEW(NumExpr) (10), NEW(NumExpr) (15)), NEW(AddExpr) (NEW(NumExpr) (20), NEW(NumExpr)(20))))->interp(Env::empty)->equals(NEW(NumVal)(65)));
}

//
TEST_CASE("Test_subst_Nabil"){
    CHECK( (NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))->subst("x", NEW(VarExpr)("y"))->equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))) );
    CHECK( (NEW(VarExpr)("x"))->subst("x", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7)))->equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))) );
}

TEST_CASE("Test_has_variable"){
    CHECK((NEW(AddExpr) (NEW(VarExpr)("x"), NEW(NumExpr)(3)))->has_variable() == true);
    CHECK((NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(5)))->has_variable() == false);
    CHECK((NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(10), NEW(VarExpr)("y")), NEW(AddExpr)(NEW(NumExpr)(20), NEW(NumExpr)(20))))->has_variable() == true);
}

TEST_CASE("Test_has_variable_2"){
    CHECK((NEW(VarExpr)("l"))->has_variable() == true);
    CHECK((NEW(NumExpr)(2))->has_variable() == false);

    CHECK((NEW(AddExpr)(NEW(NumExpr)(4), NEW(NumExpr)(10)))->has_variable() == false);
}

TEST_CASE("Test_Interp_Mult"){
    CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(4), NEW(NumExpr)(3)), NEW(NumExpr)(6)))->interp(Env::empty)->equals(NEW(NumVal)(72)));
    CHECK((NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(20)))->interp(Env::empty)->equals(NEW(NumVal)(40)));
    CHECK((NEW(MultExpr)(NEW(NumExpr)(1), NEW(NumExpr)(20)))->interp(Env::empty)->equals(NEW(NumVal)(20)));
    CHECK((NEW(MultExpr)(NEW(NumExpr) (10), NEW(NumExpr)(10)))->interp(Env::empty)->equals(NEW(NumVal)(100)));
    CHECK((NEW(MultExpr)(NEW(NumExpr)(100), NEW(NumExpr)(20)))->interp(Env::empty)->equals(NEW(NumVal)(2000)));
}

TEST_CASE("Test_Interp_Add"){
    CHECK((NEW(AddExpr)(NEW(AddExpr)(NEW(NumExpr)(4), NEW(NumExpr)(3)), NEW(NumExpr)(6)))->interp(Env::empty)->equals(NEW(NumVal)(13)));
    CHECK((NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(20)))->interp(Env::empty)->equals(NEW(NumVal)(22)));
    CHECK((NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2)))->interp(Env::empty)->equals(NEW(NumVal)(4)));
    CHECK((NEW(AddExpr)(NEW(NumExpr)(69), NEW(NumExpr)(20)))->interp(Env::empty)->equals(NEW(NumVal)(89)));
    CHECK((NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)))->interp(Env::empty)->equals(NEW(NumVal)(0)));
}

TEST_CASE("Test_Sub"){
    CHECK((NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))->subst("x", NEW(VarExpr) ("y"))->equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))));
    CHECK((NEW(VarExpr)("x"))->subst("x",  NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7)))->equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))) );
    CHECK((NEW(NumExpr)(3))->subst("3", NEW(VarExpr)("r"))->equals(NEW(NumExpr)(3)));
    CHECK((NEW(NumExpr)(4))->subst("4", NEW(VarExpr)("o"))->equals(NEW(NumExpr)(4)));
}

TEST_CASE("Test_Sub_Mult"){
    CHECK((NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")))->subst("x", NEW(NumExpr)(5))->equals(NEW(MultExpr)(NEW(NumExpr)(5), NEW(VarExpr)("y"))));
    CHECK((NEW(MultExpr)(NEW(VarExpr)("r"), NEW(VarExpr)("y")))->subst("x", NEW(NumExpr)(5))->equals(NEW(MultExpr)(NEW(VarExpr)("r"), NEW(VarExpr)("y"))));
    CHECK((NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")))->subst("x", NEW(NumExpr)(200))->equals(NEW(MultExpr)(NEW(NumExpr)(200), NEW(VarExpr)("y"))));
}

TEST_CASE("Test_Negatives"){
    CHECK((NEW(MultExpr)(NEW(NumExpr)(-4), NEW(NumExpr)(-4)))->interp(Env::empty)->equals(NEW(NumVal)(16)));
    CHECK((NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(-3)))->interp(Env::empty)->equals(NEW(NumVal)(2)));
    CHECK((NEW(NumExpr)(-4))->equals(NEW(NumExpr)(-4)));
    CHECK((NEW(MultExpr)(NEW(NumExpr)(-4), NEW(NumExpr)(-10)))->interp(Env::empty)->equals(NEW(NumVal)(40)));
    CHECK((NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(-4)))->interp(Env::empty)->equals(NEW(NumVal)(-40)));
}

TEST_CASE("Test_More_Zeros"){
    CHECK((NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)))->interp(Env::empty)->equals(NEW(NumVal)(0)));
    CHECK((NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(4)))->interp(Env::empty)->equals(NEW(NumVal)(0)));
    CHECK((NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(-4)))->interp(Env::empty)->equals(NEW(NumVal)(0)));
}

TEST_CASE("Test_to_string"){
    CHECK((NEW(NumExpr)(10))->to_string() == "10" );
    CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(3)))->to_string() == "(1+3)");
    CHECK((NEW(MultExpr)(NEW(NumExpr)(1),NEW(NumExpr)(50)))->to_string() == "(1*50)");
    CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(4), NEW(NumExpr)(10)), NEW(MultExpr)(NEW(NumExpr)(19), NEW(NumExpr)(190))))->to_string() == "((4*10)+(19*190))");
    CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(4), NEW(NumExpr)(90)), NEW(AddExpr)(NEW(NumExpr)(-10), NEW(NumExpr)(-200))))->to_string() == "((4*90)+(-10+-200))");

}

TEST_CASE("Test_to_pp_string"){
    CHECK((NEW(NumExpr)(10))->to_string() == "10" );
    CHECK((NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))->to_pp_string() == "(1 + 2) * 3" );
    CHECK((NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)), NEW(MultExpr)(NEW(NumExpr)(4), NEW(NumExpr)(6))))->to_pp_string() == "(2 + 3) * 4 * 6");
    CHECK((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)), NEW(NumExpr)(4)))->to_pp_string() == "(2 * 3) * 4");
    CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(4), NEW(NumExpr)(10)), NEW(MultExpr)(NEW(NumExpr)(19), NEW(NumExpr)(190))))->to_pp_string() == "4 * 10 + 19 * 190");
}

TEST_CASE("Test_to_pp_string1") {
    CHECK ((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(8), NEW(NumExpr)(1)), NEW(VarExpr)("r")))->to_pp_string() == "(8 * 1) * r" );
    CHECK ((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(7), NEW(NumExpr)(7)), NEW(MultExpr)(NEW(NumExpr)(9), NEW(NumExpr)(2))) )->to_pp_string() == "(7 * 7) * 9 * 2" );
    CHECK(( NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(3)), NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(8))))->to_pp_string() == "3 * 3 + 2 * 8");
    CHECK(( NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(3)), NEW(MultExpr)(NEW(NumExpr)(2), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(10)))))->to_pp_string() == "3 * 3 + 2 * (2 + 10)");
    CHECK(( NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(3), NEW(MultExpr)(NEW(NumExpr)(20), NEW(NumExpr)(40))), NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(8))))->to_pp_string() == "3 * 20 * 40 + 2 * 8");
    CHECK(( NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(3), NEW(MultExpr)(NEW(NumExpr)(20), NEW(MultExpr)(NEW(NumExpr)(65), NEW(NumExpr)(10002)))), NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(8))))->to_pp_string() == "(3 * 20 * 65 * 10002) * 2 * 8");
}

TEST_CASE("Test_to_pp_string2"){
    CHECK((NEW(NumExpr)(8))->to_pp_string() == "8");
    CHECK((NEW(NumExpr)(0))->to_pp_string() == "0");
    CHECK((NEW(NumExpr)(-3))->to_pp_string() == "-3");
    CHECK((NEW(NumExpr)(-5000))->to_pp_string() == "-5000");
    CHECK((NEW(NumExpr)(0))->to_pp_string() == "0");
}

TEST_CASE("Test_to_pp_string3"){
    CHECK((NEW(VarExpr) ("y"))->to_pp_string() == "y");
    CHECK((NEW(VarExpr)("g"))->to_pp_string() == "g");
    CHECK((NEW(VarExpr)("testVar"))->to_pp_string() == "testVar");
    CHECK((NEW(VarExpr)("10"))->to_pp_string() == "10");
    CHECK((NEW(VarExpr)(" "))->to_pp_string() == " ");
}

//
TEST_CASE("Test_Nabil_pretty_print"){
    CHECK ((NEW(MultExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr) (3))))->to_pp_string() == "1 * (2 + 3)" );
    CHECK ((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(8), NEW(NumExpr)(1)), NEW(VarExpr)("y")))->to_pp_string() == "(8 * 1) * y" );
    CHECK ((NEW(MultExpr)(NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(5)), NEW(MultExpr)(NEW(NumExpr)(6), NEW(NumExpr)(1))))->to_pp_string() == "(3 + 5) * 6 * 1" );
    CHECK ((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(7), NEW(NumExpr)(7)), NEW(AddExpr)(NEW(NumExpr)(9), NEW(NumExpr)(2))) )->to_pp_string() == "(7 * 7) * (9 + 2)" );
}
TEST_CASE("Test_Let1"){
    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5))))->to_string() == "(_let x=5 _in (x+5))");
    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5))))->to_string() == "(_let x=5 _in (x+5))");
}

TEST_CASE("Test_Let_Sub"){
    CHECK( (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5))))
    ->subst("y", NEW(NumExpr)(4))->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)))));
    CHECK((NEW(LetExpr)("z", NEW(NumExpr)(10), NEW(MultExpr)(NEW(NumExpr)(10), NEW(VarExpr)("z"))))->to_string() == "(_let z=10 _in (10*z))");
    CHECK((NEW(LetExpr)("z" , NEW(NumExpr)(10), NEW(MultExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(10))))
    ->subst("z", NEW(NumExpr)(10))->to_string() == "(_let z=10 _in (z*10))");
    CHECK( (NEW(LetExpr)("z" , NEW(NumExpr)(10), NEW(MultExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(10))))
    ->subst("z", NEW(NumExpr)(10))->equals(NEW(LetExpr)("z", NEW(NumExpr)(10), NEW(MultExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(10)))) );
    CHECK( (NEW(MultExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(10)))->subst("z", NEW(NumExpr)(10))->equals(NEW(MultExpr)(NEW(NumExpr)(10), NEW(NumExpr)(10))) );
   // CHECK((NEW(LetExpr)("z" , NEW(NumExpr)(10), NEW(MultExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(10))))->subst("z", NEW(NumExpr)(10))->interp(Env::empty)->equals(NEW(NumVal)(100)));
}
//
////todo is this right?
TEST_CASE("nested_sub_test"){
    CHECK( (NEW(LetExpr)("y", NEW(VarExpr)("x"), NEW(VarExpr)("x")))->subst("x", NEW(NumExpr)(2))->to_string()
                   == (NEW(LetExpr)("y", NEW(NumExpr)(2), NEW(NumExpr)(2)))->to_string());
    SECTION("Test LetBinding to_string")
    {
        PTR(LetExpr)expression1 = NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2)));
        PTR(LetExpr) expression2 = NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(expression1, NEW(VarExpr)("x")));
        REQUIRE(expression1->to_string() == "(_let y=3 _in (y+2))");
        REQUIRE(expression2->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
    }
}
TEST_CASE("LET CLASS")
{
    SECTION("Equals") {

        //True check
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(6))))->equals
                (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(6)))) == true);
        //False check
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(6))))->equals
                (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(6)))) == false);
    }

    SECTION("Interp") {

        //Add expression
        //CHECK((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))->interp(Env::empty)->equals(NEW(NumVal)(30)));
        //Multiplication expression
       // CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x"))), NEW(NumExpr)(1)))->interp(Env::empty)->equals(NEW(NumVal)(26)));
        //Nested in right argument of multiplication expression
       // CHECK ((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))),
       //                      NEW(NumExpr)(3)))->interp(Env::empty)->equals(NEW(NumVal)(36)));
        //Variable unchanged exception check
        CHECK_THROWS_WITH (
                (NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(3), NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2)))),
                             NEW(VarExpr)("x")))->interp(Env::empty), "no value for variable");
        //LetExpr in lhs of add
       // CHECK ((NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(2), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(9))), NEW(NumExpr)(4)))->interp(Env::empty)->equals(NEW(NumVal)(15)));
    }

    SECTION("Has Variable ") {

        //Variable contained in rhs
        CHECK((NEW(LetExpr)("x", NEW(VarExpr)("x"), NEW(AddExpr)(NEW(NumExpr)(8), NEW(NumExpr)(9))))->has_variable() == true);
        //Variable contained in body
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2))))->has_variable() == true);
        //Not contained in rhs
        CHECK((NEW(LetExpr)("y", NEW(NumExpr)(6), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(4))))->has_variable() == true);
        //Not contained in body
        CHECK((NEW(LetExpr)("z", NEW(VarExpr)("z"), NEW(AddExpr)(NEW(NumExpr)(7), NEW(NumExpr)(7))))->has_variable() == true);
        //Not contained except for lhs false check
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(2), NEW(AddExpr)(NEW(NumExpr)(8), NEW(NumExpr)(9))))->has_variable() == false);
    }

    SECTION("Substitution") {

        //Dont sub x because lhs = valToSub
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5))))->subst("x", NEW(NumExpr)(4))
                      ->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)))));
        //Don't sub x because valToSub is not contained
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5))))->subst("y", NEW(NumExpr)(4))
                      ->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(5)))));
        //Sub if lhs == valToSub in rhs
        CHECK((NEW(LetExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(8)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3))))->subst("x",
                                                                                                                                      NEW(NumExpr)(4))
                      ->equals(NEW(LetExpr)("x", NEW(AddExpr)(NEW(NumExpr)(4), NEW(NumExpr)(8)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)))));
        //Sub if lhs != valToSub when valToSub is contained in rhs
        CHECK((NEW(LetExpr)("x", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(8)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3))))->subst("y",
                                                                                                                                      NEW(NumExpr)(4))
                      ->equals(NEW(LetExpr)("x", NEW(AddExpr)(NEW(NumExpr)(4), NEW(NumExpr)(8)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3)))));
        //Sub if lhs != valToSub when valToSub is contained in body
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(6), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7)))))->subst("y",
                                                                                                                                      NEW(NumExpr)(4))
                      ->equals(NEW(LetExpr)("x", NEW(NumExpr)(6), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(NumExpr)(4), NEW(NumExpr)(7))))));

    }

    SECTION("Print") {

        //Single let
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7))))
                      ->to_string() == "(_let x=5 _in (x+7))");
        //LetExpr with multiplication expression
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(9), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3))))
                      ->to_string() == "(_let x=9 _in (x*3))");
        //Nested let
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5),
                           NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x"))))
                      ->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
        //Triple nested let
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5),
                           NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(LetExpr)("y", NEW(NumExpr)(2), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(5)))),
                                   NEW(VarExpr)("x"))))
                      ->to_string() == "(_let x=5 _in ((_let y=3 _in (_let y=2 _in (y+5)))+x))");
        //LetExpr nested with let in right hand side
        CHECK((NEW(LetExpr)("x", NEW(LetExpr)("y", NEW(NumExpr)(7), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(6))),
                           NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(3))))
                      ->to_string() == "(_let x=(_let y=7 _in (y+6)) _in (x+3))");
    }
}

    TEST_CASE("Pretty Print NABIL 2??") {

//LetExpr nested as right argument of parenthesized multiplication expression
        CHECK ((NEW(MultExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))),
                             NEW(NumExpr)(3)))->to_pp_string() == "(2 * _let x = 5\n"
                                                         "      _in  x + 1) * 3");
//LetExpr nested to the left in add expression which is nested to the right within a multiplication expression
        CHECK((NEW(MultExpr)(NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(1))))->to_pp_string() ==
              "5 * ((_let x = 5\n"
              "       _in  x) + 1)");
//LetExpr in lhs of add
        CHECK ((NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(2), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(9))), NEW(NumExpr)(4)))->to_pp_string() ==
               "(_let x = 2\n"
               "  _in  x + 9) + 4");
//LetExpr in lhs of multiplication expression
        CHECK((NEW(MultExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(8))), NEW(NumExpr)(3)))->to_pp_string() ==
              "(_let x = 5\n"
              "  _in  x + 8) * 3");
//LetExpr nest as right argument of un-parenthesized multiplication expression
        CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(4), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))),
                           NEW(NumExpr)(9)))->to_pp_string() == "4 * (_let x = 5\n"
                                                       "      _in  x + 1) + 9");
//LetExpr nested to the left within let that is nested to the left within add
        CHECK ((NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(3), NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2)))),
                            NEW(VarExpr)("x")))->to_pp_string() == "(_let x = 3\n"
                                                          "  _in  _let y = 3\n"
                                                          "       _in  y + 2) + x");
//LetExpr nested in lhs of Add expression nested within body of let expression
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5),
                           NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x"))))
                      ->to_pp_string() == "_let x = 5\n"
                                          " _in  (_let y = 3\n"
                                          "       _in  y + 2) + x");
//Triple nested let
//todo figure out indent?
        CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5),
                           NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3),
                                                   NEW(AddExpr)(NEW(VarExpr)("y"), NEW(LetExpr)("z", NEW(NumExpr)(6),
                                                                                             NEW(AddExpr)(NEW(VarExpr)("a"), NEW(NumExpr)(8))))),
                                   NEW(VarExpr)("x"))))
                      ->to_pp_string() == "_let x = 5\n"
                                          " _in  (_let y = 3\n"
                                          "       _in  y + _let z = 6\n"
                                          "                _in  a + 8) + x");

    }

TEST_CASE("Nabil_parse") {
    CHECK_THROWS_WITH( parse_str("()"), "invalid input " );

    CHECK( parse_str("(1)")->equals(NEW(NumExpr)(1)) );
    CHECK( parse_str("(((1)))")->equals(NEW(NumExpr)(1)) );

    CHECK_THROWS_WITH( parse_str("(1"), "invalid input ')' " );

    CHECK( parse_str("1")->equals(NEW(NumExpr)(1)) );
    CHECK( parse_str("10")->equals(NEW(NumExpr)(10)) );
    CHECK( parse_str("-3")->equals(NEW(NumExpr) (-3)) );
    CHECK( parse_str("  \n 5  ")->equals(NEW(NumExpr)(5)) );
    CHECK_THROWS_WITH( parse_str("-"), "invalid input digit" );


    CHECK_THROWS_WITH( parse_str(" -   5  "), "invalid input digit" );

    CHECK( parse_str("x")->equals(NEW(VarExpr)("x")) );
    CHECK( parse_str("xyz")->equals(NEW(VarExpr)("xyz")) );
    CHECK( parse_str("xYz")->equals(NEW(VarExpr)("xYz")) );
    CHECK_THROWS_WITH( parse_str("x_z"), "invalid input eof" );

    CHECK( parse_str("x + y")->equals(NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))) );

    CHECK( parse_str("x * y")->equals(NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))) );

    CHECK( parse_str("z * x + y")
                   ->equals(NEW(AddExpr)(NEW(MultExpr)(NEW(VarExpr)("z"), NEW(VarExpr)("x")),
                                        NEW(VarExpr)("y"))) );

    CHECK( parse_str("z * (x + y)")
                   ->equals(NEW(MultExpr)(NEW(VarExpr)("z"),
                                         NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))) ));
}

TEST_CASE("testing_let_parse"){
    CHECK(parse_str(("_let x=5 _in (x+7)"))->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))));
    CHECK(parse_str(("_let x=10 _in (y+10)"))->equals(NEW(LetExpr)("x", NEW(NumExpr)(10), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(10)))));
    CHECK(parse_str(("_let i=100 _in (i+10)"))->equals(NEW(LetExpr)("i", NEW(NumExpr)(100), NEW(AddExpr)(NEW(VarExpr)("i"), NEW(NumExpr)(10)))));
    CHECK(parse_str(("(_let x=5 _in ((_let y=3 _in (y+2))+x))"))
    ->equals(NEW(LetExpr)("x", NEW(NumExpr)(5),
                         NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(2))), NEW(VarExpr)("x")))));
    CHECK(parse_str(("(_let x=5 _in (x+7))"))->equals((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7))))));
}

TEST_CASE("Val_Classes"){
    CHECK(((NEW(NumVal)(4))->to_string()) == "4");
    CHECK(((NEW(NumVal)(0))->to_string()) == "0");
    CHECK(((NEW(NumVal)(-5))->to_string()) == "-5");
    CHECK(((NEW(NumVal)(10000))->to_string()) == "10000");
    CHECK(((NEW(NumVal)(3000000))->to_string()) == "3000000");
}

TEST_CASE("testing_refactoring_Val"){
    SECTION("SetUpBoolVal"){
        CHECK((NEW(BoolVal)(true))->is_true());
        CHECK_FALSE((NEW(BoolVal)(false))->is_true());
        CHECK_THROWS_WITH((NEW(BoolVal)(true))->add_to(NEW(NumVal)(5)), "Cannot add a boolean arguments");

    }
}

TEST_CASE("testing_refactoring_expr"){
    SECTION("EqExpr"){
        CHECK((NEW(EqExpr)(NEW(NumExpr)(4), NEW(NumExpr)(4)))->interp(Env::empty)->equals(NEW(BoolVal)(true)));
        CHECK((NEW(EqExpr)(NEW(NumExpr) (1), NEW(NumExpr)(2)))->interp(Env::empty)->equals(NEW(BoolVal)(false)) );
        CHECK((NEW(EqExpr)(NEW(NumExpr)(3), NEW(NumExpr)(3)))->interp(Env::empty)->equals(NEW(BoolVal)(true)));
        CHECK((NEW(EqExpr)(NEW(AddExpr)(NEW(NumExpr)(3), NEW(NumExpr)(3)), NEW(NumExpr)(6)))->interp(Env::empty)->equals(NEW(BoolVal)(true)));
    }
}

TEST_CASE("testing-refactoring"){
    CHECK( (NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))->interp(Env::empty)
                   ->equals(NEW(NumVal)(3)) );
    CHECK( (NEW(AddExpr)(NEW(NumExpr)(10), NEW(NumExpr)(20)))->interp(Env::empty)
                   ->equals(NEW(NumVal)(30)) );
    CHECK((NEW(MultExpr)(NEW(NumExpr)(3), NEW(NumExpr)(10)))->interp(Env::empty)->equals(NEW(NumVal)(30)));
    CHECK((NEW(MultExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)))->interp(Env::empty)->equals(NEW(NumVal)(0)));
    CHECK((NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(0)))->interp(Env::empty)->equals(NEW(NumVal)(0)));
    CHECK((NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(-10)))->interp(Env::empty)->equals(NEW(NumVal)(-10)));

}

TEST_CASE("HW9 Conditionals"){
    SECTION("Nabil slide 40"){
        //1 + 2 -> 3
        CHECK( (NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)))->interp(Env::empty)->equals(NEW(NumVal)(3)) );
    }
    SECTION("Nabil slide 49"){
        //_let x = 2+3
        //_in x*x
//        CHECK( (NEW(LetExpr)("x",
//                            NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)),
//                         NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x"))))
//                       ->interp(Env::empty)
//                       ->equals(NEW(NumVal)(25)) );
    }
    SECTION("Nabil slide 57"){
        //_if _true
        //_then 1
        //_else 2
        CHECK( (NEW(IfExpr)(NEW(BoolExpr)(true),
                           NEW(NumExpr)(1),
                           NEW(NumExpr)(2)))->interp(Env::empty)
                       ->equals(NEW(NumVal)(1)) );
    }
    SECTION("(1 == 2) + 3 throws an exception", "[Add]") {
        PTR(expr) testExpr = NEW(AddExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3));
        CHECK_THROWS_WITH(testExpr->interp(Env::empty), "Cannot add a boolean arguments");
    }

    SECTION("1 == 2 + 3 evaluates to _false", "[EqExpr]") {
        PTR(expr) testExpr = NEW(EqExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)));
        PTR(Val) result = testExpr->interp(Env::empty);
        CHECK((result)->to_string() == "_false");
        //std::cout << "testBool: " << (new BoolExpr(true))->interp(Env::empty)->to_string() << std::endl;

    }

    SECTION("1 + 1 == 2 + 0 evaluates to _true", "[EqExpr]") {
        PTR(expr) testExpr = NEW(EqExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(1)), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(0)));
        PTR(Val) result = testExpr->interp(Env::empty);
        CHECK((result)->to_string() == "_true");
    }
}


TEST_CASE("parse") {
    SECTION("parse1") {
        CHECK(parse_str("1 == 2")->interp(Env::empty)->equals(NEW(BoolVal)(false)));
        CHECK((((parse_str("_if 1 == 2 _then 5 _else 6"))->interp(Env::empty))->to_string()) == "6");
        CHECK((parse_str("(1 + 2) == (3 + 0)"))->interp(Env::empty)->equals(NEW(BoolVal)(true)));
        CHECK((parse_str("(1 + 2) == (3 + 0)"))->interp(Env::empty)->to_string()== "_true");
    }
    SECTION("parse2"){
        CHECK( parse_str("_if x==3 _then 42+x _else 84*x")
            ->equals( NEW(IfExpr)(NEW(EqExpr)( NEW(VarExpr)("x"), NEW(NumExpr)(3) ),
                     NEW(AddExpr)( NEW(NumExpr)(42), NEW(VarExpr)("x")),
                     NEW(MultExpr)( NEW(NumExpr)(84), NEW(VarExpr)("x")))));

        CHECK( parse_str(" \n\t _if \n\t x \n\t == \n\t 3 \n\t _then \n\t 42 \n\t + \n\t x "
                "\n\t _else \n\t 84 \n\t * \n\t x \n\t ")->equals( NEW(IfExpr)( NEW(EqExpr)( NEW(VarExpr)("x"), NEW(NumExpr)(3) ),
                     NEW(AddExpr)( NEW(NumExpr)(42), NEW(VarExpr)("x") ),
                     NEW(MultExpr)( NEW(NumExpr)(84), NEW(VarExpr)("x") ) ) ));

        CHECK((parse_str(" _if   3 == 3 _then _true _else _false")
        ->equals(NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(3), NEW(NumExpr)(3)), NEW(BoolExpr)(true), NEW(BoolExpr)(false)))));
    }

    SECTION("parse3WithBools"){
        CHECK(parse_str("_if _true _then 5 _else 4")->equals(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(5), NEW(NumExpr)(4))));
        CHECK(parse_str("_true")->equals(NEW(BoolExpr)(true)));
        CHECK(parse_str("_if 2 == 2 _then _true _else _false")
        ->equals(NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2)), NEW(BoolExpr)(true), NEW(BoolExpr)(false))));
    }
    SECTION("parseMore"){
        CHECK( (parse_str ( "_if 1 == 2 _then 5 _else 6")) -> equals(NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(5), NEW(NumExpr)(6))));
        CHECK( (parse_str ("_if (_if 1 == 2 _then _true _else _false) _then 5 _else 6")) ->interp(Env::empty) ->equals(NEW(NumVal)(6)));
        CHECK( (((parse_str ( "_if 1 == 2 _then 5 _else 6")) -> interp(Env::empty)) -> to_string()) == "6");
        CHECK( (((parse_str ( "_if (_if 1 == 2\n"
                              "     _then _false\n"
                              "     _else _true)\n"
                              "_then 5\n"
                              "_else 6")) -> interp(Env::empty)) -> to_string()) == "5");
    }
}

TEST_CASE("test_sub_if"){
    CHECK((NEW(IfExpr)(NEW(BoolExpr)(true), NEW(VarExpr)("x"), NEW(NumExpr)(10)))->subst("x", NEW(NumExpr)(8))
    ->equals(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(8), NEW(NumExpr)(10))));
    CHECK((NEW(IfExpr)(NEW(BoolExpr)(false), NEW(VarExpr)("x"), NEW(NumExpr)(10)))->subst("x", NEW(NumExpr)(8))
                  ->equals(NEW(IfExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(8), NEW(NumExpr)(10))));
    CHECK((NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(6), NEW(VarExpr)("6")))->subst("6", NEW(NumExpr)(8))
                  ->equals(NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(6), NEW(NumExpr)(8))));
}

TEST_CASE("aiden_fun_expr"){
   // std::cout << (FunExpr("x", new AddExpr(new NumExpr(5), new NumExpr(7))).to_string());
//    CHECK((parse_str("(_fun (x) (5+7))"))->interp(Env::empty)->equals(FunExpr("x", new AddExpr(new NumExpr(5), new NumExpr(7))))));
    CHECK((parse_str("(_fun (x) x+1 (10))"))->interp(Env::empty)->to_string() == "11");
}

TEST_CASE("FunExpr"){
//    SECTION("Parsing FunExpr"){
//        CHECK((parse_str("_let f = _fun (x) x+1"
//                         "_in f(10)"))->interp(Env::empty)->to_string() == "11");
//    }

    SECTION("Parsing FunExpr"){
        CHECK((parse_str("_let f = _fun (x) x+1"
                         "_in f(10)"))->interp(Env::empty)->to_string() == "11");
        CHECK((parse_str("_let f = _fun (x) x*x"
                         "_in f(2)"))->interp(Env::empty)->to_string() == "4");
        CHECK((parse_str("_let y = 8"
                         "_in _let f = _fun (x) x*y"
                         "_in f(2)"))->interp(Env::empty)->to_string() == "16");
        CHECK((parse_str("_let x = 8"
                         "_in _let f = _fun (x) x*x"
                         "_in f(2)"))->interp(Env::empty)->to_string() == "4");
        CHECK((parse_str("_let factrl = _fun (factrl)"
                         "_fun (x)"
                         "_if x ==1"
                         "_then 1"
                         "_else x * factrl(factrl)(x + -1)"
                         "_in factrl(factrl)(10)"))->interp(Env::empty)->to_string() == "3628800");
    }
    SECTION("Parsing CallExpr"){
        CHECK((parse_str("(_fun (x) x+1 (10))"))->interp(Env::empty)->to_string() == "11");
        CHECK((parse_str("(_fun (x) x+x (1))"))->interp(Env::empty)->to_string() == "2");
    }

}





