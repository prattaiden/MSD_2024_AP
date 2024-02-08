#include <iostream>
#include "catch.h"
#include "expr.h"

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

//TODO negative numbers and zeros
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




//TEST_CASE("Test_Print") {
//  Mult *multTest3 = new Mult(new Num(2), new Mult(new Num(3), new Num(4)));
//  Mult *multTest1 = new Mult(new Num(2), new Add(new Num(3), new Num(4)));
//  Mult *multTest2 = new Mult(new Mult(new Num(2), new Num(2)), new Add(new Num(2), new Num(2)));
//  Mult *multTest4 = new Mult(new Add(new Num(2), new Num(3)), new Mult(new Num(4), new Num(6)));
//  Mult *multTest5 = new Mult(new Add( new Num(1), new Num(2)), new Num(3));
//
//  Add *addTest1 = new Add(new Mult(new Num(2), new Num(3)), new Num(1));
//    Add *addTest2 = new Add(new Num(1), new Mult(new Num(2), new Num(3)));
//
//    std::cout << "\n";
//    multTest3->pretty_print_at(std::cout);
//    std::cout << "\n";
//    multTest1->pretty_print_at(std::cout);
//    std::cout << "\n";
//    multTest2->pretty_print_at(std::cout);
//    std::cout << "\n";
//    multTest4->pretty_print_at(std::cout);
//    std::cout << "\n";
//    addTest1->pretty_print_at(std::cout);
//    std::cout << "\n";
//    multTest5->pretty_print_at(std::cout);
//    std::cout << "\n";
//    addTest2->pretty_print_at(std::cout);
//
//
//}
