#include <iostream>
#include "catch.h"
#include "expr.h"
#include "parse.hpp"
#include "Val.h"

//
TEST_CASE("Nabil_Test_Cases"){
    CHECK((new VarExpr("x"))->equals(new VarExpr("x")) == true );
    CHECK((new VarExpr("x"))->equals(new VarExpr("y")) == false );
    CHECK((new NumExpr(1))->equals(new VarExpr("x")) == false );
    CHECK((new AddExpr(new NumExpr(2), new NumExpr(3)))->equals(new AddExpr(new NumExpr(3), new NumExpr(2))) == false );
    CHECK((new AddExpr(new NumExpr(2), new NumExpr(3)))->equals(new AddExpr(new NumExpr(2), new NumExpr(3))) == true );
}


TEST_CASE("Test_Mult"){
    CHECK((new MultExpr(new NumExpr (4), new NumExpr(10)))->equals(new MultExpr(new NumExpr (4), new NumExpr(10))) == true);
    CHECK((new MultExpr(new NumExpr(2), new NumExpr(3)))->equals(new MultExpr(new NumExpr(3), new NumExpr(2))) == false);
    //testing that casting the mult on the add will just be false
    CHECK((new MultExpr(new NumExpr(2), new NumExpr(3)))->equals(new AddExpr(new NumExpr(3), new NumExpr(2))) == false);
    //testing when plugging a Var in instead of a Num
    CHECK((new MultExpr(new VarExpr("2"), new VarExpr("3")))->equals(new MultExpr(new VarExpr("3"), new VarExpr("2"))) == false);
    CHECK((new MultExpr(new VarExpr("2"), new VarExpr("3")))->equals(new MultExpr(new VarExpr("2"), new VarExpr("3"))) == true);
}

TEST_CASE("Test_Num"){
    CHECK((new NumExpr(5))->equals((new NumExpr (5))) == true);
    CHECK((new NumExpr(6))->equals((new NumExpr (2))) == false);
    //testing casting var and num will be false
    CHECK((new NumExpr(6))->equals((new VarExpr ("6"))) == false);

}

TEST_CASE("Test_Var"){
    CHECK((new VarExpr("f"))->equals((new VarExpr ("f"))) == true);
    CHECK((new VarExpr("v"))->equals((new VarExpr ("r"))) == false);
    //testing casting Num and Var will be false
    CHECK((new VarExpr("6"))->equals((new NumExpr (6))) == false);
    CHECK((new VarExpr("6"))->equals((new VarExpr ("6"))) == true);
}

//
TEST_CASE("TEST_Interp_Nabil"){
    CHECK_THROWS_WITH((new VarExpr("x"))->interp(), "no value for variable" );
    CHECK((new MultExpr(new NumExpr(3), new NumExpr(2)))->interp()->equals(new NumVal(6)) );
    CHECK((new AddExpr(new AddExpr(new NumExpr(10), new NumExpr(15)), new AddExpr(new NumExpr(20), new NumExpr(20))))->interp()->equals(new NumVal(65)));
}

//
TEST_CASE("Test_subst_Nabil"){
    CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))->subst("x", new VarExpr("y"))->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))) );
    CHECK( (new VarExpr("x"))->subst("x", new AddExpr(new VarExpr("y"), new NumExpr(7)))->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))) );
}

TEST_CASE("Test_has_variable"){
    CHECK((new AddExpr(new VarExpr("x"), new NumExpr(3)))->has_variable() == true);
    CHECK((new AddExpr(new NumExpr(5), new NumExpr(5)))->has_variable() == false);
    CHECK((new AddExpr(new AddExpr(new NumExpr(10), new VarExpr("y")), new AddExpr(new NumExpr(20), new NumExpr(20))))->has_variable() == true);
}

TEST_CASE("Test_has_variable_2"){
    CHECK((new VarExpr("l"))->has_variable() == true);
    CHECK((new NumExpr (2))->has_variable() == false);

    CHECK((new AddExpr(new NumExpr(4), new NumExpr(10)))->has_variable() == false);
}

TEST_CASE("Test_Interp_Mult"){
    CHECK((new MultExpr(new MultExpr(new NumExpr(4), new NumExpr(3)), new NumExpr(6)))->interp()->equals(new NumVal(72)));
    CHECK((new MultExpr(new NumExpr (2), new NumExpr(20)))->interp()->equals(new NumVal(40)));
    CHECK((new MultExpr(new NumExpr (1), new NumExpr(20)))->interp()->equals(new NumVal(20)));
    CHECK((new MultExpr(new NumExpr (10), new NumExpr(10)))->interp()->equals(new NumVal(100)));
    CHECK((new MultExpr(new NumExpr (100), new NumExpr(20)))->interp()->equals(new NumVal(2000)));
}

TEST_CASE("Test_Interp_Add"){
    CHECK((new AddExpr(new AddExpr(new NumExpr(4), new NumExpr(3)), new NumExpr(6)))->interp()->equals(new NumVal(13)));
    CHECK((new AddExpr(new NumExpr (2), new NumExpr(20)))->interp()->equals(new NumVal(22)));
    CHECK((new AddExpr(new NumExpr(2), new NumExpr(2)))->interp()->equals(new NumVal(4)));
    CHECK((new AddExpr(new NumExpr(69), new NumExpr(20)))->interp()->equals(new NumVal(89)));
    CHECK((new AddExpr(new NumExpr(0), new NumExpr(0)))->interp()->equals(new NumVal(0)));
}

TEST_CASE("Test_Sub"){
    CHECK((new AddExpr(new VarExpr("x"), new NumExpr(7)))->subst("x", new VarExpr("y"))->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))) );
    CHECK((new VarExpr("x"))->subst("x", new AddExpr(new VarExpr("y"), new NumExpr(7)))->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))) );
    CHECK((new NumExpr(3))->subst("3", new VarExpr("r"))->equals(new NumExpr(3)));
    CHECK((new NumExpr(4))->subst("4", new VarExpr("o"))->equals(new NumExpr(4)));
}

TEST_CASE("Test_Sub_Mult"){
    CHECK((new MultExpr(new VarExpr("x"), new VarExpr("y")))->subst("x", new NumExpr(5))->equals(new MultExpr(new NumExpr(5), new VarExpr("y"))));
    CHECK((new MultExpr(new VarExpr("r"), new VarExpr("y")))->subst("x", new NumExpr(5))->equals(new MultExpr(new VarExpr("r"), new VarExpr("y"))));
    CHECK((new MultExpr(new VarExpr("x"), new VarExpr("y")))->subst("x", new NumExpr(200))->equals(new MultExpr(new NumExpr(200), new VarExpr("y"))));
}

TEST_CASE("Test_Negatives"){
    CHECK((new MultExpr(new NumExpr(-4), new NumExpr(-4)))->interp()->equals(new NumVal(16)));
    CHECK((new AddExpr(new NumExpr(5), new NumExpr(-3)))->interp()->equals(new NumVal(2)));
    CHECK((new NumExpr(-4))->equals(new NumExpr(-4)));
    CHECK((new MultExpr(new NumExpr(-4), new NumExpr(-10)))->interp()->equals(new NumVal(40)));
    CHECK((new MultExpr(new NumExpr(10), new NumExpr(-4)))->interp()->equals(new NumVal(-40)));
}

TEST_CASE("Test_More_Zeros"){
    CHECK((new AddExpr(new NumExpr(0), new NumExpr(0)))->interp()->equals(new NumVal(0)));
    CHECK((new MultExpr(new NumExpr(0), new NumExpr(4)))->interp()->equals(new NumVal(0)));
    CHECK((new MultExpr(new NumExpr(0), new NumExpr(-4)))->interp()->equals(new NumVal(0)));
}

TEST_CASE("Test_to_string"){
    CHECK((new NumExpr(10))->to_string() == "10" );
    CHECK((new AddExpr(new NumExpr(1), new NumExpr(3)))->to_string() == "(1+3)");
    CHECK((new MultExpr(new NumExpr(1), new NumExpr(50)))->to_string() == "(1*50)");
    CHECK((new AddExpr(new MultExpr(new NumExpr(4), new NumExpr(10)), new MultExpr(new NumExpr(19), new NumExpr(190))))->to_string() == "((4*10)+(19*190))");
    CHECK((new AddExpr(new MultExpr(new NumExpr(4), new NumExpr(90)), new AddExpr(new NumExpr(-10), new NumExpr(-200))))->to_string() == "((4*90)+(-10+-200))");

}

TEST_CASE("Test_to_pp_string"){
    CHECK((new NumExpr(10))->to_string() == "10" );
    CHECK((new MultExpr(new AddExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3)))->to_pp_string() == "(1 + 2) * 3" );
    CHECK((new MultExpr(new AddExpr(new NumExpr(2), new NumExpr(3)), new MultExpr(new NumExpr(4), new NumExpr(6))))->to_pp_string() == "(2 + 3) * 4 * 6");
    CHECK((new MultExpr(new MultExpr(new NumExpr(2), new NumExpr(3)), new NumExpr(4)))->to_pp_string() == "(2 * 3) * 4");
    CHECK((new AddExpr(new MultExpr(new NumExpr(4), new NumExpr(10)), new MultExpr(new NumExpr(19), new NumExpr(190))))->to_pp_string() == "4 * 10 + 19 * 190");
}

TEST_CASE("Test_to_pp_string1") {
    CHECK ((new MultExpr(new MultExpr(new NumExpr(8), new NumExpr(1)), new VarExpr("r")))->to_pp_string() == "(8 * 1) * r" );
    CHECK ((new MultExpr(new MultExpr(new NumExpr(7), new NumExpr(7)), new MultExpr(new NumExpr(9), new NumExpr(2))) )->to_pp_string() == "(7 * 7) * 9 * 2" );
    CHECK(( new AddExpr(new MultExpr(new NumExpr(3), new NumExpr(3)), new MultExpr(new NumExpr(2), new NumExpr(8))))->to_pp_string() == "3 * 3 + 2 * 8");
    CHECK(( new AddExpr(new MultExpr(new NumExpr(3), new NumExpr(3)), new MultExpr(new NumExpr(2), new AddExpr(new NumExpr(2), new NumExpr(10)))))->to_pp_string() == "3 * 3 + 2 * (2 + 10)");
    CHECK(( new AddExpr(new MultExpr(new NumExpr(3), new MultExpr(new NumExpr (20), new NumExpr (40))), new MultExpr(new NumExpr(2), new NumExpr(8))))->to_pp_string() == "3 * 20 * 40 + 2 * 8");
    CHECK(( new MultExpr(new MultExpr(new NumExpr(3), new MultExpr(new NumExpr (20), new MultExpr(new NumExpr(65), new NumExpr(10002)))), new MultExpr(new NumExpr(2), new NumExpr(8))))->to_pp_string() == "(3 * 20 * 65 * 10002) * 2 * 8");
}

TEST_CASE("Test_to_pp_string2"){
    CHECK((new NumExpr(8))->to_pp_string() == "8");
    CHECK((new NumExpr(0))->to_pp_string() == "0");
    CHECK((new NumExpr(-3))->to_pp_string() == "-3");
    CHECK((new NumExpr(-5000))->to_pp_string() == "-5000");
    CHECK((new NumExpr(0))->to_pp_string() == "0");
}

TEST_CASE("Test_to_pp_string3"){
    CHECK((new VarExpr("y"))->to_pp_string() == "y");
    CHECK((new VarExpr("g"))->to_pp_string() == "g");
    CHECK((new VarExpr("testVar"))->to_pp_string() == "testVar");
    CHECK((new VarExpr("10"))->to_pp_string() == "10");
    CHECK((new VarExpr(" "))->to_pp_string() == " ");
}

//
TEST_CASE("Test_Nabil_pretty_print"){
    CHECK ((new MultExpr(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3))))->to_pp_string() == "1 * (2 + 3)" );
    CHECK ((new MultExpr(new MultExpr(new NumExpr(8), new NumExpr(1)), new VarExpr("y")))->to_pp_string() == "(8 * 1) * y" );
    CHECK ((new MultExpr(new AddExpr(new NumExpr(3), new NumExpr(5)), new MultExpr(new NumExpr(6), new NumExpr(1))))->to_pp_string() == "(3 + 5) * 6 * 1" );
    CHECK ((new MultExpr(new MultExpr(new NumExpr(7), new NumExpr(7)), new AddExpr(new NumExpr(9), new NumExpr(2))) )->to_pp_string() == "(7 * 7) * (9 + 2)" );
}
TEST_CASE("Test_Let1"){
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(5))))->to_string() == "(_let x=5 _in (x+5))");
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(5))))->to_string() == "(_let x=5 _in (x+5))");
}

TEST_CASE("Test_Let_Sub"){
    CHECK( (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(5))))
    ->subst("y", new NumExpr(4))->equals(new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(5)))));
    CHECK((new LetExpr("z", new NumExpr(10), new MultExpr(new NumExpr(10), new VarExpr("z"))))->to_string() == "(_let z=10 _in (10*z))");
    CHECK((new LetExpr("z" , new NumExpr(10), new MultExpr(new VarExpr("z"), new NumExpr(10))))
    ->subst("z", new NumExpr(10))->to_string() == "(_let z=10 _in (z*10))");
    CHECK( (new LetExpr("z" , new NumExpr(10), new MultExpr(new VarExpr("z"), new NumExpr(10))))
    ->subst("z", new NumExpr(10))->equals(new LetExpr("z", new NumExpr(10), new MultExpr(new VarExpr("z"), new NumExpr(10)))) );
    CHECK( (new MultExpr(new VarExpr("z"), new NumExpr(10)))->subst("z", new NumExpr(10))->equals(new MultExpr(new NumExpr(10), new NumExpr(10))) );
    CHECK((new LetExpr("z" , new NumExpr(10), new MultExpr(new VarExpr("z"), new NumExpr(10))))->subst("z", new NumExpr(10))->interp()->equals(new NumVal(100)));
}
//
////todo is this right?
TEST_CASE("nested_sub_test"){
    CHECK( (new LetExpr("y", new VarExpr("x"), new VarExpr("x")))->subst("x", new NumExpr(2))->to_string()
                   == (new LetExpr("y", new NumExpr(2), new NumExpr(2)))->to_string());
    SECTION("Test LetBinding to_string")
    {
        LetExpr *expression1 = new LetExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2)));
        LetExpr *expression2 = new LetExpr("x", new NumExpr(5), new AddExpr(expression1, new VarExpr("x")));
        REQUIRE(expression1->to_string() == "(_let y=3 _in (y+2))");
        REQUIRE(expression2->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
    }
}
TEST_CASE("LET CLASS")
{
    SECTION("Equals") {

        //True check
        CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(6))))->equals
                (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(6)))) == true);
        //False check
        CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(6))))->equals
                (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("y"), new NumExpr(6)))) == false);
    }

    SECTION("Interp") {

        //Add expression
        CHECK((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))->interp()->equals(new NumVal(30)));
        //Multiplication expression
        CHECK((new AddExpr(new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new VarExpr("x"))), new NumExpr(1)))->interp()->equals(new NumVal(26)));
        //Nested in right argument of multiplication expression
        CHECK ((new MultExpr(new MultExpr(new NumExpr(2), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))),
                             new NumExpr(3)))->interp()->equals(new NumVal(36)));
        //Variable unchanged exception check
        CHECK_THROWS_WITH (
                (new AddExpr(new LetExpr("x", new NumExpr(3), new LetExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2)))),
                             new VarExpr("x")))->interp(), "no value for variable");
        //LetExpr in lhs of add
        CHECK ((new AddExpr(new LetExpr("x", new NumExpr(2), new AddExpr(new VarExpr("x"), new NumExpr(9))), new NumExpr(4)))->interp()->equals(new NumVal(15)));
    }

    SECTION("Has Variable ") {

        //Variable contained in rhs
        CHECK((new LetExpr("x", new VarExpr("x"), new AddExpr(new NumExpr(8), new NumExpr(9))))->has_variable() == true);
        //Variable contained in body
        CHECK((new LetExpr("x", new NumExpr(3), new AddExpr(new VarExpr("x"), new NumExpr(2))))->has_variable() == true);
        //Not contained in rhs
        CHECK((new LetExpr("y", new NumExpr(6), new AddExpr(new VarExpr("y"), new NumExpr(4))))->has_variable() == true);
        //Not contained in body
        CHECK((new LetExpr("z", new VarExpr("z"), new AddExpr(new NumExpr(7), new NumExpr(7))))->has_variable() == true);
        //Not contained except for lhs false check
        CHECK((new LetExpr("x", new NumExpr(2), new AddExpr(new NumExpr(8), new NumExpr(9))))->has_variable() == false);
    }

    SECTION("Substitution") {

        //Dont sub x because lhs = valToSub
        CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(5))))->subst("x", new NumExpr(4))
                      ->equals(new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(5)))));
        //Don't sub x because valToSub is not contained
        CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(5))))->subst("y", new NumExpr(4))
                      ->equals(new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(5)))));
        //Sub if lhs == valToSub in rhs
        CHECK((new LetExpr("x", new AddExpr(new VarExpr("x"), new NumExpr(8)), new AddExpr(new VarExpr("x"), new NumExpr(3))))->subst("x",
                                                                                                                                      new NumExpr(4))
                      ->equals(new LetExpr("x", new AddExpr(new NumExpr(4), new NumExpr(8)), new AddExpr(new VarExpr("x"), new NumExpr(3)))));
        //Sub if lhs != valToSub when valToSub is contained in rhs
        CHECK((new LetExpr("x", new AddExpr(new VarExpr("y"), new NumExpr(8)), new AddExpr(new VarExpr("x"), new NumExpr(3))))->subst("y",
                                                                                                                                      new NumExpr(4))
                      ->equals(new LetExpr("x", new AddExpr(new NumExpr(4), new NumExpr(8)), new AddExpr(new VarExpr("x"), new NumExpr(3)))));
        //Sub if lhs != valToSub when valToSub is contained in body
        CHECK((new LetExpr("x", new NumExpr(6), new AddExpr(new VarExpr("x"), new AddExpr(new VarExpr("y"), new NumExpr(7)))))->subst("y",
                                                                                                                                      new NumExpr(4))
                      ->equals(new LetExpr("x", new NumExpr(6), new AddExpr(new VarExpr("x"), new AddExpr(new NumExpr(4), new NumExpr(7))))));

    }

    SECTION("Print") {

        //Single let
        CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(7))))
                      ->to_string() == "(_let x=5 _in (x+7))");
        //LetExpr with multiplication expression
        CHECK((new LetExpr("x", new NumExpr(9), new MultExpr(new VarExpr("x"), new NumExpr(3))))
                      ->to_string() == "(_let x=9 _in (x*3))");
        //Nested let
        CHECK((new LetExpr("x", new NumExpr(5),
                           new AddExpr(new LetExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))), new VarExpr("x"))))
                      ->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
        //Triple nested let
        CHECK((new LetExpr("x", new NumExpr(5),
                           new AddExpr(new LetExpr("y", new NumExpr(3), new LetExpr("y", new NumExpr(2), new AddExpr(new VarExpr("y"), new NumExpr(5)))),
                                   new VarExpr("x"))))
                      ->to_string() == "(_let x=5 _in ((_let y=3 _in (_let y=2 _in (y+5)))+x))");
        //LetExpr nested with let in right hand side
        CHECK((new LetExpr("x", new LetExpr("y", new NumExpr(7), new AddExpr(new VarExpr("y"), new NumExpr(6))),
                           new AddExpr(new VarExpr("x"), new NumExpr(3))))
                      ->to_string() == "(_let x=(_let y=7 _in (y+6)) _in (x+3))");
    }
}

    TEST_CASE("Pretty Print NABIL 2??") {

//LetExpr nested as right argument of parenthesized multiplication expression
        CHECK ((new MultExpr(new MultExpr(new NumExpr(2), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))),
                             new NumExpr(3)))->to_pp_string() == "(2 * _let x = 5\n"
                                                         "      _in  x + 1) * 3");
//LetExpr nested to the left in add expression which is nested to the right within a multiplication expression
        CHECK((new MultExpr(new NumExpr(5), new AddExpr(new LetExpr("x", new NumExpr(5), new VarExpr("x")), new NumExpr(1))))->to_pp_string() ==
              "5 * ((_let x = 5\n"
              "       _in  x) + 1)");
//LetExpr in lhs of add
        CHECK ((new AddExpr(new LetExpr("x", new NumExpr(2), new AddExpr(new VarExpr("x"), new NumExpr(9))), new NumExpr(4)))->to_pp_string() ==
               "(_let x = 2\n"
               "  _in  x + 9) + 4");
//LetExpr in lhs of multiplication expression
        CHECK((new MultExpr(new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(8))), new NumExpr(3)))->to_pp_string() ==
              "(_let x = 5\n"
              "  _in  x + 8) * 3");
//LetExpr nest as right argument of un-parenthesized multiplication expression
        CHECK((new AddExpr(new MultExpr(new NumExpr(4), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))),
                           new NumExpr(9)))->to_pp_string() == "4 * (_let x = 5\n"
                                                       "      _in  x + 1) + 9");
//LetExpr nested to the left within let that is nested to the left within add
        CHECK ((new AddExpr(new LetExpr("x", new NumExpr(3), new LetExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2)))),
                            new VarExpr("x")))->to_pp_string() == "(_let x = 3\n"
                                                          "  _in  _let y = 3\n"
                                                          "       _in  y + 2) + x");
//LetExpr nested in lhs of Add expression nested within body of let expression
        CHECK((new LetExpr("x", new NumExpr(5),
                           new AddExpr(new LetExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))), new VarExpr("x"))))
                      ->to_pp_string() == "_let x = 5\n"
                                          " _in  (_let y = 3\n"
                                          "       _in  y + 2) + x");
//Triple nested let
//todo figure out indent?
        CHECK((new LetExpr("x", new NumExpr(5),
                           new AddExpr(new LetExpr("y", new NumExpr(3),
                                                   new AddExpr(new VarExpr("y"), new LetExpr("z", new NumExpr(6),
                                                                                             new AddExpr(new VarExpr("a"), new NumExpr(8))))),
                                   new VarExpr("x"))))
                      ->to_pp_string() == "_let x = 5\n"
                                          " _in  (_let y = 3\n"
                                          "       _in  y + _let z = 6\n"
                                          "                _in  a + 8) + x");

    }

TEST_CASE("Nabil_parse") {
    CHECK_THROWS_WITH( parse_str("()"), "invalid input" );

    CHECK( parse_str("(1)")->equals(new NumExpr(1)) );
    CHECK( parse_str("(((1)))")->equals(new NumExpr(1)) );

    CHECK_THROWS_WITH( parse_str("(1"), "invalid input" );

    CHECK( parse_str("1")->equals(new NumExpr(1)) );
    CHECK( parse_str("10")->equals(new NumExpr(10)) );
    CHECK( parse_str("-3")->equals(new NumExpr(-3)) );
    CHECK( parse_str("  \n 5  ")->equals(new NumExpr(5)) );
    CHECK_THROWS_WITH( parse_str("-"), "invalid input" );


    CHECK_THROWS_WITH( parse_str(" -   5  "), "invalid input" );

    CHECK( parse_str("x")->equals(new VarExpr("x")) );
    CHECK( parse_str("xyz")->equals(new VarExpr("xyz")) );
    CHECK( parse_str("xYz")->equals(new VarExpr("xYz")) );
    CHECK_THROWS_WITH( parse_str("x_z"), "invalid input" );

    CHECK( parse_str("x + y")->equals(new AddExpr(new VarExpr("x"), new VarExpr("y"))) );

    CHECK( parse_str("x * y")->equals(new MultExpr(new VarExpr("x"), new VarExpr("y"))) );

    CHECK( parse_str("z * x + y")
                   ->equals(new AddExpr(new MultExpr(new VarExpr("z"), new VarExpr("x")),
                                        new VarExpr("y"))) );

    CHECK( parse_str("z * (x + y)")
                   ->equals(new MultExpr(new VarExpr("z"),
                                         new AddExpr(new VarExpr("x"), new VarExpr("y"))) ));
}

TEST_CASE("testing_let_parse"){
    CHECK(parse_str(("_let x=5 _in (x+7)"))->equals(new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(7)))));
    CHECK(parse_str(("_let x=10 _in (y+10)"))->equals(new LetExpr("x", new NumExpr(10), new AddExpr(new VarExpr("y"), new NumExpr(10)))));
    CHECK(parse_str(("_let i=100 _in (i+10)"))->equals(new LetExpr("i", new NumExpr(100), new AddExpr(new VarExpr("i"), new NumExpr(10)))));
    CHECK(parse_str(("(_let x=5 _in ((_let y=3 _in (y+2))+x))"))
    ->equals(new LetExpr("x", new NumExpr(5),
                         new AddExpr(new LetExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))), new VarExpr("x")))));
    CHECK(parse_str(("(_let x=5 _in (x+7))"))->equals((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(7))))));
}

TEST_CASE("Val_Classes"){
    CHECK(((new NumVal(4))->to_string()) == "4");
}

TEST_CASE("testing_refactoring_Val"){
    SECTION("SetUpBoolVal"){
        CHECK((new BoolVal(true))->is_true());
        CHECK_FALSE((new BoolVal(false))->is_true());
        CHECK_THROWS_WITH((new BoolVal(true))->add_to(new NumVal(5)), "Cannot add a boolean arguments");

    }
}

TEST_CASE("testing_refactoring_expr"){
    SECTION("EqExpr"){
        CHECK((new EqExpr(new NumExpr(4), new NumExpr(4)))->interp()->equals(new BoolVal(true)));
        CHECK((new EqExpr(new NumExpr(1), new NumExpr(2)))->interp()->equals(new BoolVal(false)) );
        CHECK((new EqExpr(new NumExpr(3), new NumExpr(3)))->interp()->equals(new BoolVal(true)));
        CHECK((new EqExpr(new AddExpr(new NumExpr(3), new NumExpr(3)), new NumExpr(6)))->interp()->equals(new BoolVal(true)));
    }
}

TEST_CASE("testing-refactoring"){
    CHECK( (new AddExpr(new NumExpr(1), new NumExpr(2)))->interp()
                   ->equals(new NumVal(3)) );
    CHECK( (new AddExpr(new NumExpr(10), new NumExpr(20)))->interp()
                   ->equals(new NumVal(30)) );
}

TEST_CASE("HW9 Conditionals"){
    SECTION("Nabil slide 40"){
        //1 + 2 -> 3
        CHECK( (new AddExpr(new NumExpr(1), new NumExpr(2)))->interp()->equals(new NumVal(3)) );
    }
    SECTION("Nabil slide 49"){
        //_let x = 2+3
        //_in x*x
        CHECK( (new LetExpr("x",
                         new AddExpr(new NumExpr(2), new NumExpr(3)),
                         new MultExpr(new VarExpr("x"), new VarExpr("x"))))
                       ->interp()
                       ->equals(new NumVal(25)) );
    }
    SECTION("Nabil slide 57"){
        //_if _true
        //_then 1
        //_else 2
        CHECK( (new IfExpr(new BoolExpr(true),
                           new NumExpr(1),
                           new NumExpr(2)))->interp()
                       ->equals(new NumVal(1)) );
    }
    SECTION("(1 == 2) + 3 throws an exception", "[Add]") {
        expr* testExpr = new AddExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3));
        CHECK_THROWS_WITH(testExpr->interp(), "Cannot add a boolean arguments");
    }

    SECTION("1 == 2 + 3 evaluates to _false", "[EqExpr]") {
        expr* testExpr = new EqExpr(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3)));
        Val* result = testExpr->interp();
        CHECK((result)->to_string() == "_false");
        //std::cout << "testBool: " << (new BoolExpr(true))->interp()->to_string() << std::endl;

    }

    SECTION("1 + 1 == 2 + 0 evaluates to _true", "[EqExpr]") {
        expr* testExpr = new EqExpr(new AddExpr(new NumExpr(1), new NumExpr(1)), new AddExpr(new NumExpr(2), new NumExpr(0)));
        Val* result = testExpr->interp();
        CHECK((result)->to_string() == "_true");
    }
}


TEST_CASE("parse1") {
    SECTION("parse") {
        CHECK(parse_str("1 == 2")->interp()->equals(new BoolVal(false)));
        CHECK((((parse_str("_if 1 == 2 _then 5 _else 6"))->interp())->to_string()) == "6");
        CHECK((parse_str("(1 + 2) == (3 + 0)"))->interp()->equals(new BoolVal(true)));
        CHECK((parse_str("(1 + 2) == (3 + 0)"))->interp()->to_string()== "_true");
    }
    SECTION("parse2"){
        CHECK( parse_str("_if x==3 _then 42+x _else 84*x")
            ->equals( new IfExpr( new EqExpr( new VarExpr("x"), new NumExpr(3) ),
                     new AddExpr( new NumExpr(42), new VarExpr("x")),
                     new MultExpr( new NumExpr(84), new VarExpr("x")))));

    CHECK( parse_str(" \n\t _if \n\t x \n\t == \n\t 3 \n\t _then \n\t 42 \n\t + \n\t x "
             "\n\t _else \n\t 84 \n\t * \n\t x \n\t ")
            ->equals( new IfExpr( new EqExpr( new VarExpr("x"), new NumExpr(3) ),
                     new AddExpr( new NumExpr(42), new VarExpr("x") ),
                     new MultExpr( new NumExpr(84), new VarExpr("x") ) ) ));
    }
    SECTION("parse3WithBools"){
        CHECK(parse_str("_if _true _then 5 _else 4")->equals(new IfExpr(new BoolExpr(true), new NumExpr(5), new NumExpr(4))));
        CHECK(parse_str("_true")->equals(new BoolExpr(true)));
        CHECK(parse_str("_if 2 == 2 _then _true _else _false")
        ->equals(new IfExpr(new EqExpr(new NumExpr(2), new NumExpr(2)), new BoolExpr(true), new BoolExpr(false))));
    }
}




