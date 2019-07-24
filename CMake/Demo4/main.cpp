#include<iostream>
#include<string>
#include"config.h"

#ifdef USE_MYMATH
    #include "mymath/mymath.h"
    using namespace mymath;
#else
    using namespace std;
#endif

int main(int argc, char *argv[]){
    
    std::string info;
#ifdef USE_MYMATH
    info = "mymath::min";
#else
    info = "std::min";
#endif
    std::cout << "Using "<< info << std::endl;

    if(argc > 2)
        std::cout << "minimum ASCII char between " << argv[1][0] << " and " << argv[2][0] << " is " << (char)min(argv[1][0], argv[2][0]) << "." << std::endl;
    else
        std::cout << "bad input." << std::endl;
    return 0;
}
