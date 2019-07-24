#include <iostream>
#include "calc.h"

int main(){
    Calc c;
    int a = 1;
    int b = 2;
    std::cout << a << "+" << b << "=" << c.add(a, b) << std::endl;
    return 0;
}
