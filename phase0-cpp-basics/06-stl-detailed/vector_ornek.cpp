#include <iostream>
#include <vector>

int main() {
    std::vector<int> sayilar;

    sayilar.push_back(10);
    sayilar.push_back(20);
    sayilar.push_back(30);
    sayilar.push_back(40);
    sayilar.push_back(50);

    std::cout << "=== VECTOR ===" << std::endl;
    std::cout << "Boyut: " << sayilar.size() << std::endl;
    std::cout << "Ilk: " << sayilar.front() << std::endl;
    std::cout << "Son: " << sayilar.back() << std::endl;

    std::cout << "\nTum elemanlar: ";
    for (int s : sayilar) {
        std::cout << s << " ";
    }
    std::cout << std::endl;

    sayilar[2] = 999;
    std::cout << "\nsayilar[2] = 999 yapildi: ";
    for (int s : sayilar) {
        std::cout << s << " ";
    }
    std::cout << std::endl;

    sayilar.pop_back();
    std::cout << "\npop_back sonrasi: ";
    for (int s : sayilar) {
        std::cout << s << " ";
    }
    std::cout << std::endl;

    std::cout << "\n=== NOT ORTALAMASI ===" << std::endl;
    std::vector<int> notlar = {70, 85, 90, 60, 95};
    int toplam = 0;
    for (int n : notlar) {
        toplam += n;
    }
    std::cout << "Ortalama: " << (double)toplam / notlar.size() << std::endl;

    return 0;
}