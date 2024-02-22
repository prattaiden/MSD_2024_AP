#include <iostream>
#include "catch.h"
#include "expr.h"
#include "parse.hpp"

//
TEST_CASE("Nabil_Test_Cases"){
    CHECK( (new Var("x"))->equals(new Var("x")) == true );
    CHECK( (new Var("x"))->equals(new Var("y")) == false );
    CHECK( (new Num(1))->equals(new Var("x")) == false );
    CHECK( (new Add(new Num(2),new Num(3)))->equals(new Add(new Num(3),new Num(2)))==false );
    CHECK( (new Add(new Num(2),new Num(3)))->equals(new Add(new Num(2),new Num(3)))==true );
}


TEST_CASE("Test_Mult"){
    CHECK((new Mult(new Num (4), new Num(10)))->equals(new Mult(new Num (4), new Num(10))) == true);
    CHECK( (new Mult(new Num(2),new Num(3)))->equals(new Mult(new Num(3),new Num(2)))==false);
    //testing that casting the mult on the add will just be false
    CHECK((new Mult(new Num(2),new Num(3)))->equals(new Add(new Num(3),new Num(2)))==false);
    //testing when plugging a Var in instead of a Num
    CHECK((new Mult(new Var("2"),new Var("3")))->equals(new Mult(new Var("3"),new Var("2")))==false);
    CHECK((new Mult(new Var("2"),new Var("3")))->equals(new Mult(new Var("2"),new Var("3")))==true);
}

TEST_CASE("Test_Num"){
    CHECK((new Num(5))->equals((new Num (5)))== true);
    CHECK((new Num(6))->equals((new Num (2)))== false);
    //testing casting var and num will be false
    CHECK((new Num(6))->equals((new Var ("6")))== false);

}

TEST_CASE("Test_Var"){
    CHECK((new Var("f"))->equals((new Var ("f")))== true);
    CHECK((new Var("v"))->equals((new Var ("r")))== false);
    //testing casting Num and Var will be false
    CHECK((new Var("6"))->equals((new Num (6)))== false);
    CHECK((new Var("6"))->equals((new Var ("6")))== true);
}

//
TEST_CASE("TEST_Interp_Nabil"){
    CHECK_THROWS_WITH( (new Var("x"))->interp(), "no value for variable" );
    CHECK( (new Mult(new Num(3), new Num(2)))->interp()==6 );
    CHECK( (new Add(new Add(new Num(10), new Num(15)),new Add(new Num(20),new Num(20))))->interp()==65);
}

//
TEST_CASE("Test_subst_Nabil"){
    CHECK( (new Add(new Var("x"), new Num(7)))->subst("x", new Var("y"))->equals(new Add(new Var("y"), new Num(7))) );
    CHECK( (new Var("x"))->subst("x", new Add(new Var("y"),new Num(7)))->equals(new Add(new Var("y"),new Num(7))) );
}

TEST_CASE("Test_has_variable"){
    CHECK((new Add(new Var("x"), new Num(3)))->has_variable() == true);
    CHECK((new Add(new Num(5), new Num(5)))->has_variable() == false);
    CHECK( (new Add(new Add(new Num(10), new Var("y")),new Add(new Num(20),new Num(20))))->has_variable()==true);
}

TEST_CASE("Test_has_variable_2"){
    CHECK((new Var("l"))->has_variable()== true);
    CHECK((new Num (2))->has_variable()==false);

    CHECK((new Add(new Num(4), new Num(10)))->has_variable()==false);
}

TEST_CASE("Test_Interp_Mult"){
    CHECK((new Mult(new Mult(new Num(4), new Num(3)), new Num(6)))->interp()==72);
    CHECK((new Mult(new Num (2), new Num(20)))->interp()==40);
    CHECK((new Mult(new Num (1), new Num(20)))->interp()==20);
    CHECK((new Mult(new Num (10), new Num(10)))->interp()==100);
    CHECK((new Mult(new Num (100), new Num(20)))->interp()==2000);
}

TEST_CASE("Test_Interp_Add"){
    CHECK((new Add(new Add(new Num(4), new Num(3)), new Num(6)))->interp()==13);
    CHECK((new Add(new Num (2), new Num(20)))->interp()==22);
    CHECK((new Add(new Num(2), new Num(2)))->interp()==4);
    CHECK((new Add(new Num(69), new Num(20)))->interp()==89);
    CHECK((new Add(new Num(0), new Num(0)))->interp()==0);
}

TEST_CASE("Test_Sub"){
    CHECK((new Add(new Var("x"), new Num(7)))->subst("x", new Var("y"))->equals(new Add(new Var("y"), new Num(7))) );
    CHECK((new Var("x"))->subst("x", new Add(new Var("y"),new Num(7)))->equals(new Add(new Var("y"),new Num(7))) );
    CHECK((new Num(3))->subst("3", new Var("r"))->equals(new Num(3)));
    CHECK((new Num(4))->subst("4", new Var("o"))->equals(new Num(4)));
}

TEST_CASE("Test_Sub_Mult"){
    CHECK((new Mult(new Var("x"),new Var("y")))->subst("x", new Num(5))->equals(new Mult(new Num(5),new Var("y"))));
    CHECK((new Mult(new Var("r"),new Var("y")))->subst("x", new Num(5))->equals(new Mult(new Var("r"),new Var("y"))));
    CHECK((new Mult(new Var("x"),new Var("y")))->subst("x", new Num(200))->equals(new Mult(new Num(200),new Var("y"))));
}

TEST_CASE("Test_Negatives"){
    CHECK((new Mult(new Num(-4), new Num(-4)))->interp()==16);
    CHECK((new Add(new Num(5), new Num(-3)))->interp()==2);
    CHECK((new Num(-4))->equals(new Num(-4)));
    CHECK((new Mult(new Num(-4), new Num(-10)))->interp()==40);
    CHECK((new Mult(new Num(10), new Num(-4)))->interp()==-40);
}

TEST_CASE("Test_More_Zeros"){
    CHECK((new Add(new Num(0), new Num(0)))->interp()==0);
    CHECK((new Mult(new Num(0), new Num(4)))->interp()==0);
    CHECK((new Mult(new Num(0), new Num(-4)))->interp()==0);
}

TEST_CASE("Test_to_string"){
    CHECK( (new Num(10))->to_string() == "10" );
    CHECK((new Add(new Num(1), new Num(3)))->to_string() == "(1+3)");
    CHECK((new Mult(new Num(1), new Num(50)))->to_string() == "(1*50)");
    CHECK((new Add(new Mult(new Num(4), new Num(10)), new Mult(new Num(19), new Num(190))))->to_string() == "((4*10)+(19*190))");
    CHECK((new Add(new Mult(new Num(4), new Num(90)), new Add(new Num(-10), new Num(-200))))->to_string() == "((4*90)+(-10+-200))");

}

TEST_CASE("Test_to_pp_string"){
    CHECK( (new Num(10))->to_string() == "10" );
    CHECK( (new Mult(new Add( new Num(1), new Num(2)), new Num(3)))->to_pp_string() == "(1 + 2) * 3" );
    CHECK((new Mult(new Add(new Num(2), new Num(3)), new Mult(new Num(4), new Num(6))))->to_pp_string() == "(2 + 3) * 4 * 6");
    CHECK((new Mult(new Mult(new Num(2), new Num(3)), new Num(4)))->to_pp_string() == "(2 * 3) * 4");
    CHECK((new Add(new Mult(new Num(4), new Num(10)), new Mult(new Num(19), new Num(190))))->to_pp_string() == "4 * 10 + 19 * 190");
}

TEST_CASE("Test_to_pp_string1") {
    CHECK ( (new Mult(new Mult(new Num(8), new Num(1)), new Var("r")))->to_pp_string() ==  "(8 * 1) * r" );
    CHECK ( (new Mult(new Mult(new Num(7), new Num(7)), new Mult(new Num(9), new Num(2))) )->to_pp_string() ==  "(7 * 7) * 9 * 2" );
    CHECK(( new Add(new Mult(new Num(3), new Num(3)), new Mult(new Num(2), new Num(8))))->to_pp_string() == "3 * 3 + 2 * 8");
    CHECK(( new Add(new Mult(new Num(3), new Num(3)), new Mult(new Num(2), new Add(new Num(2), new Num(10)))))->to_pp_string() == "3 * 3 + 2 * (2 + 10)");
    CHECK(( new Add(new Mult(new Num(3), new Mult(new Num (20), new Num (40))), new Mult(new Num(2), new Num(8))))->to_pp_string() == "3 * 20 * 40 + 2 * 8");
    CHECK(( new Mult(new Mult(new Num(3), new Mult(new Num (20), new Mult(new Num(65), new Num(10002)))), new Mult(new Num(2), new Num(8))))->to_pp_string() == "(3 * 20 * 65 * 10002) * 2 * 8");
}

TEST_CASE("Test_to_pp_string2"){
    CHECK((new Num(8))->to_pp_string() == "8");
    CHECK((new Num(0))->to_pp_string() == "0");
    CHECK((new Num(-3))->to_pp_string() == "-3");
    CHECK((new Num(-5000))->to_pp_string() == "-5000");
    CHECK((new Num(0))->to_pp_string() == "0");
}

TEST_CASE("Test_to_pp_string3"){
    CHECK((new Var("y"))->to_pp_string() == "y");
    CHECK((new Var("g"))->to_pp_string() == "g");
    CHECK((new Var("testVar"))->to_pp_string() == "testVar");
    CHECK((new Var("10"))->to_pp_string() == "10");
    CHECK((new Var(" "))->to_pp_string() == " ");
}

//
TEST_CASE("Test_Nabil_pretty_print"){
    CHECK ( (new Mult(new Num(1), new Add(new Num(2), new Num(3))))->to_pp_string() ==  "1 * (2 + 3)" );
    CHECK ( (new Mult(new Mult(new Num(8), new Num(1)), new Var("y")))->to_pp_string() ==  "(8 * 1) * y" );
    CHECK ( (new Mult(new Add(new Num(3), new Num(5)), new Mult(new Num(6), new Num(1))))->to_pp_string() ==  "(3 + 5) * 6 * 1" );
    CHECK ( (new Mult(new Mult(new Num(7), new Num(7)), new Add(new Num(9), new Num(2))) )->to_pp_string() ==  "(7 * 7) * (9 + 2)" );
}
TEST_CASE("Test_Let1"){
    CHECK((new Let("x", new Num(5), new Add(new Var("x"), new Num(5))))->to_string() == "(_let x=5 _in (x+5))");
    CHECK((new Let("x", new Num(5), new Add(new Var("x"), new Num(5))))->to_string() == "(_let x=5 _in (x+5))");
}

TEST_CASE("Test_Let_Sub"){
    CHECK( (new Let("x", new Num(5), new Add(new Var("x"), new Num(5))))
    ->subst("y", new Num(4))->equals( new Let("x", new Num(5), new Add(new Var("x"), new Num(5)))));
    CHECK((new Let("z", new Num(10), new Mult(new Num(10), new Var("z"))))->to_string() == "(_let z=10 _in (10*z))");
    CHECK((new Let("z" , new Num(10),new Mult(new Var("z"), new Num(10))))
    ->subst("z", new Num(10))->to_string() == "(_let z=10 _in (z*10))");
    CHECK( (new Let("z" , new Num(10),new Mult(new Var("z"), new Num(10))))
    ->subst("z", new Num(10))->equals(new Let("z", new Num(10),new Mult(new Var("z"), new Num(10)))) );
    CHECK( (new Mult(new Var("z"), new Num(10)))->subst("z", new Num(10))->equals(new Mult(new Num(10),new Num(10))) );
    CHECK( (new Let("z" , new Num(10),new Mult(new Var("z"), new Num(10))))->subst("z", new Num(10))->interp() == 100 );
}
//
////todo is this right?
TEST_CASE("nested_sub_test"){
    CHECK( (new Let("y", new Var("x"), new Var("x")))->subst("x", new Num(2))->to_string()
                   == (new Let("y", new Num(2), new Num(2)))->to_string());
    SECTION("Test LetBinding to_string")
    {
        Let *expression1 = new Let("y", new Num(3), new Add(new Var("y"), new Num(2)));
        Let *expression2 = new Let("x", new Num(5),new Add(expression1, new Var("x")));
        REQUIRE(expression1->to_string() == "(_let y=3 _in (y+2))");
        REQUIRE(expression2->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
    }
}
TEST_CASE("LET CLASS")
{
    SECTION("Equals") {

        //True check
        CHECK((new Let("x", new Num(5), new Add(new Var("x"), new Num(6))))->equals
                (new Let("x", new Num(5), new Add(new Var("x"), new Num(6)))) == true);
        //False check
        CHECK((new Let("x", new Num(5), new Add(new Var("x"), new Num(6))))->equals
                (new Let("x", new Num(5), new Add(new Var("y"), new Num(6)))) == false);
    }

    SECTION("Interp") {

        //Add expression
        CHECK((new Mult(new Num(5), new Let("x", new Num(5), new Add(new Var("x"), new Num(1)))))->interp() == 30);
        //Multiplication expression
        CHECK((new Add(new Mult(new Num(5), new Let("x", new Num(5), new Var("x"))), new Num(1)))->interp() == 26);
        //Nested in right argument of multiplication expression
        CHECK ((new Mult(new Mult(new Num(2), new Let("x", new Num(5), new Add(new Var("x"), new Num(1)))),
                         new Num(3)))->interp() == 36);
        //Variable unchanged exception check
        CHECK_THROWS_WITH (
                (new Add(new Let("x", new Num(3), new Let("y", new Num(3), new Add(new Var("y"), new Num(2)))),
                         new Var("x")))->interp(), "no value for variable");
        //Let in lhs of add
        CHECK ((new Add(new Let("x", new Num(2), new Add(new Var("x"), new Num(9))), new Num(4)))->interp() == 15);
    }

    SECTION("Has Variable ") {

        //Variable contained in rhs
        CHECK((new Let("x", new Var("x"), new Add(new Num(8), new Num(9))))->has_variable() == true);
        //Variable contained in body
        CHECK((new Let("x", new Num(3), new Add(new Var("x"), new Num(2))))->has_variable() == true);
        //Not contained in rhs
        CHECK((new Let("y", new Num(6), new Add(new Var("y"), new Num(4))))->has_variable() == true);
        //Not contained in body
        CHECK((new Let("z", new Var("z"), new Add(new Num(7), new Num(7))))->has_variable() == true);
        //Not contained except for lhs false check
        CHECK((new Let("x", new Num(2), new Add(new Num(8), new Num(9))))->has_variable() == false);
    }

    SECTION("Substitution") {

        //Dont sub x because lhs = valToSub
        CHECK((new Let("x", new Num(5), new Add(new Var("x"), new Num(5))))->subst("x", new Num(4))
                      ->equals(new Let("x", new Num(5), new Add(new Var("x"), new Num(5)))));
        //Don't sub x because valToSub is not contained
        CHECK((new Let("x", new Num(5), new Add(new Var("x"), new Num(5))))->subst("y", new Num(4))
                      ->equals(new Let("x", new Num(5), new Add(new Var("x"), new Num(5)))));
        //Sub if lhs == valToSub in rhs
        CHECK((new Let("x", new Add(new Var("x"), new Num(8)), new Add(new Var("x"), new Num(3))))->subst("x",
                                                                                                          new Num(4))
                      ->equals(new Let("x", new Add(new Num(4), new Num(8)), new Add(new Var("x"), new Num(3)))));
        //Sub if lhs != valToSub when valToSub is contained in rhs
        CHECK((new Let("x", new Add(new Var("y"), new Num(8)), new Add(new Var("x"), new Num(3))))->subst("y",
                                                                                                          new Num(4))
                      ->equals(new Let("x", new Add(new Num(4), new Num(8)), new Add(new Var("x"), new Num(3)))));
        //Sub if lhs != valToSub when valToSub is contained in body
        CHECK((new Let("x", new Num(6), new Add(new Var("x"), new Add(new Var("y"), new Num(7)))))->subst("y",
                                                                                                          new Num(4))
                      ->equals(new Let("x", new Num(6), new Add(new Var("x"), new Add(new Num(4), new Num(7))))));

    }

    SECTION("Print") {

        //Single let
        CHECK((new Let("x", new Num(5), new Add(new Var("x"), new Num(7))))
                      ->to_string() == "(_let x=5 _in (x+7))");
        //Let with multiplication expression
        CHECK((new Let("x", new Num(9), new Mult(new Var("x"), new Num(3))))
                      ->to_string() == "(_let x=9 _in (x*3))");
        //Nested let
        CHECK((new Let("x", new Num(5),
                       new Add(new Let("y", new Num(3), new Add(new Var("y"), new Num(2))), new Var("x"))))
                      ->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
        //Triple nested let
        CHECK((new Let("x", new Num(5),
                       new Add(new Let("y", new Num(3), new Let("y", new Num(2), new Add(new Var("y"), new Num(5)))),
                               new Var("x"))))
                      ->to_string() == "(_let x=5 _in ((_let y=3 _in (_let y=2 _in (y+5)))+x))");
        //Let nested with let in right hand side
        CHECK((new Let("x", new Let("y", new Num(7), new Add(new Var("y"), new Num(6))),
                       new Add(new Var("x"), new Num(3))))
                      ->to_string() == "(_let x=(_let y=7 _in (y+6)) _in (x+3))");
    }
}

    TEST_CASE("Pretty Print NABIL 2??") {

//Let nested as right argument of parenthesized multiplication expression
        CHECK ((new Mult(new Mult(new Num(2), new Let("x", new Num(5), new Add(new Var("x"), new Num(1)))),
                         new Num(3)))->to_pp_string() == "(2 * _let x = 5\n"
                                                         "      _in  x + 1) * 3");
//Let nested to the left in add expression which is nested to the right within a multiplication expression
        CHECK((new Mult(new Num(5), new Add(new Let("x", new Num(5), new Var("x")), new Num(1))))->to_pp_string() ==
              "5 * ((_let x = 5\n"
              "       _in  x) + 1)");
//Let in lhs of add
        CHECK ((new Add(new Let("x", new Num(2), new Add(new Var("x"), new Num(9))), new Num(4)))->to_pp_string() ==
               "(_let x = 2\n"
               "  _in  x + 9) + 4");
//Let in lhs of multiplication expression
        CHECK((new Mult(new Let("x", new Num(5), new Add(new Var("x"), new Num(8))), new Num(3)))->to_pp_string() ==
              "(_let x = 5\n"
              "  _in  x + 8) * 3");
//Let nest as right argument of un-parenthesized multiplication expression
        CHECK((new Add(new Mult(new Num(4), new Let("x", new Num(5), new Add(new Var("x"), new Num(1)))),
                       new Num(9)))->to_pp_string() == "4 * (_let x = 5\n"
                                                       "      _in  x + 1) + 9");
//Let nested to the left within let that is nested to the left within add
        CHECK ((new Add(new Let("x", new Num(3), new Let("y", new Num(3), new Add(new Var("y"), new Num(2)))),
                        new Var("x")))->to_pp_string() == "(_let x = 3\n"
                                                          "  _in  _let y = 3\n"
                                                          "       _in  y + 2) + x");
//Let nested in lhs of Add expression nested within body of let expression
        CHECK((new Let("x", new Num(5),
                       new Add(new Let("y", new Num(3), new Add(new Var("y"), new Num(2))), new Var("x"))))
                      ->to_pp_string() == "_let x = 5\n"
                                          " _in  (_let y = 3\n"
                                          "       _in  y + 2) + x");
//Triple nested let
//todo figure out indent?
        CHECK((new Let("x", new Num(5),
                       new Add(new Let("y", new Num(3),
                                       new Add(new Var("y"), new Let("z", new Num(6),
                                                                     new Add(new Var("a"), new Num(8))))),
                               new Var("x"))))
                      ->to_pp_string() == "_let x = 5\n"
                                          " _in  (_let y = 3\n"
                                          "       _in  y + _let z = 6\n"
                                          "                _in  a + 8) + x");

    }

TEST_CASE("Nabil_parse") {
    CHECK_THROWS_WITH( parse_str("()"), "invalid input" );

    CHECK( parse_str("(1)")->equals(new Num(1)) );
    CHECK( parse_str("(((1)))")->equals(new Num(1)) );

    CHECK_THROWS_WITH( parse_str("(1"), "invalid input" );

    CHECK( parse_str("1")->equals(new Num(1)) );
    CHECK( parse_str("10")->equals(new Num(10)) );
    CHECK( parse_str("-3")->equals(new Num(-3)) );
    CHECK( parse_str("  \n 5  ")->equals(new Num(5)) );
    CHECK_THROWS_WITH( parse_str("-"), "invalid input" );


    CHECK_THROWS_WITH( parse_str(" -   5  "), "invalid input" );

    CHECK( parse_str("x")->equals(new Var("x")) );
    CHECK( parse_str("xyz")->equals(new Var("xyz")) );
    CHECK( parse_str("xYz")->equals(new Var("xYz")) );
    CHECK_THROWS_WITH( parse_str("x_z"), "invalid input" );

    CHECK( parse_str("x + y")->equals(new Add(new Var("x"), new Var("y"))) );

    CHECK( parse_str("x * y")->equals(new Mult(new Var("x"), new Var("y"))) );

    CHECK( parse_str("z * x + y")
                   ->equals(new Add(new Mult(new Var("z"), new Var("x")),
                                    new Var("y"))) );

    CHECK( parse_str("z * (x + y)")
                   ->equals(new Mult(new Var("z"),
                                     new Add(new Var("x"), new Var("y"))) ));
}

TEST_CASE("testing_let_parse"){
    CHECK(parse_str(("_let x=5 _in (x+7)"))->equals(new Let("x", new Num(5), new Add(new Var("x"), new Num(7)))));
    CHECK(parse_str(("_let x=10 _in (y+10)"))->equals(new Let("x", new Num(10), new Add(new Var("y"), new Num(10)))));
    CHECK(parse_str(("_let i=100 _in (i+10)"))->equals(new Let("i", new Num(100), new Add(new Var("i"), new Num(10)))));
    CHECK(parse_str(("(_let x=5 _in ((_let y=3 _in (y+2))+x))"))
    ->equals(new Let("x", new Num(5),
                     new Add(new Let("y", new Num(3), new Add(new Var("y"), new Num(2))), new Var("x")))));
    CHECK(parse_str(("(_let x=5 _in (x+7))"))->equals((new Let("x", new Num(5), new Add(new Var("x"), new Num(7))))));
}


