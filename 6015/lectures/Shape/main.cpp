#include "shape.hpp"

int main (int argc, char * argv[]) {
    
//    Rectangle *r1 = new Rectangle (2,4);
//    Circle *c1 = new Circle (5);
    Rectangle r1 (2,4);
    Circle c1 (5);
    

    printArea(r1);
    printArea(c1);
}
