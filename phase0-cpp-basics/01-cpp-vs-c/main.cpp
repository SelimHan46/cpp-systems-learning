#include <iostream>
#include <string>

int main () 
{
    std::cout << "Merhaba, C++ dunyasina hosgeldin!" << std::endl;

    std::string isim;
    std::cout << "Ismin ne? ";
    std::cin >> isim;
    std::cout << "Selam " << isim << "!" << std::endl;

    int *p = new int(42);

    std::cout << "Deger: " << *p << std::endl;
    delete p;

    return 0;
}