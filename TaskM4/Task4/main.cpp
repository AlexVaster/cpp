//  Ex.17 Квадраты цифр числа

#include <iostream>
#include <fstream>
#include "Quad.hpp"

int main(int argc, const char * argv[]) {
    Quad obj1;
    std::ifstream fin("QUAD.IN");
    std::ofstream fout("QUAD.OUT");
    fin >> obj1;
    fout << obj1;
    fout.close();
    
    Quad test(5, 3);
    std::cout << test << std::endl;
    return 0;
}
