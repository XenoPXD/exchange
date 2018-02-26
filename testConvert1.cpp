#include <iostream>
#include <cstdio>


using namespace std;

// g++ -std=c++11 paper-folding-curve.cpp -o paper-folding-curve



int main(int argc, char *argv[]) {

    std::cout << argc << endl;
    string str = "22.3";
    string a1 = argv[1];
    //std::cout << str << endl;
    //int i = std::istringstream ss(t);
    int i;
    float f;
    double d;
    long l;

    // string -> integer
    if(sscanf(str.c_str(), "%d", &i) != 1){}
    // error management

    // string -> float
    if(sscanf(str.c_str(), "%f", &f) != 1){}
        // error management

    // string -> double
    if(sscanf(str.c_str(), "%lf", &d) != 1){}
    // error management

    // string -> long
    if(sscanf(str.c_str(), "%ld", &l) != 1){}
    // error management

    std::cout << "int:" << i << endl;
    std::cout << "float:" << f << endl;
    std::cout << "double:" << d << endl;
    std::cout << "long:" << l << endl;

    return 0;
}
