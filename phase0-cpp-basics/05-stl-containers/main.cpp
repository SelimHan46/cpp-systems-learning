#include <iostream>
#include <string>
#include <vector>       // dinamik dizi 
#include <map>          // anahtar-deger 
#include <queue>        // kuyruk (FIFO)

int main() {
    std::cout << "=== VECTOR ===" << std::endl;

    // boyut belirtmene gerek yok, otomatik buyur
    std::vector<int> notlar;

    // Eleman ekleme 
    notlar.push_back(90);
    notlar.push_back(75);
    notlar.push_back(85);
    notlar.push_back(60);
    notlar.push_back(95);

    std::cout << "Not sayisi: " << notlar.size() << std::endl;

    // Elemanlara erişim
    std::cout << "Ilk not: " << notlar[0] << std::endl;
    std::cout << "Son not: " << notlar.back() << std::endl; 

    // Dongu ile yazdirma (ozel range-based for)
    std::cout << "Tum notlar: ";
    for (int not_ : notlar) {
        std::cout << not_ << " ";
    }

    std::cout << std::endl;

    // Eleman silme (sondan)
    notlar.pop_back();
    std::cout << "Son not silindi, kalan: " << notlar.size() << " not" << std::endl;

    // 2. MAP - Anahtar-Deger yapisi
    std::cout << "\n=== MAP ===" << std::endl;

    // C++'da: isim -> not eslemesi 
    std::map<std::string, int> ogrenciler;

    ogrenciler["Selim"] = 90;
    ogrenciler["Ahmet"] = 75;
    ogrenciler["Ayse"] = 95;
    ogrenciler["Fatma"] = 85;

    //Erisim
    std::cout << "Selim'in notu: " << ogrenciler["Selim"] << std::endl;

    // Dongu ile yazdirma 
    std::cout << "\nTum ogrenciler:" << std::endl;
    for (auto& [isim, not_] : ogrenciler) {
        std::cout << " " << isim << " -> " << not_ << std::endl;
    }

    // Eleman var mi kontrolu
    if (ogrenciler.count("Selim") > 0) {
        std::cout << "\nSelim listede var!" << std::endl;
    }

    std::cout << "\n=== QUEUE ===" << std::endl;

    // Ileride thread'lerde cok kullancagiz!
    std::queue<std::string> siralama;

    siralama.push("Selim");
    siralama.push("Ahmet");
    siralama.push("Ayse");

    std::cout << "Siradaki: " << siralama.front() << std::endl;
    std::cout << "Sira sayisi: " << siralama.size() << std::endl;

    // Sirayla cikar (ilk giren ilk cikar)
    std::cout << "\nSira isleniyor" << std::endl;
    while(!siralama.empty()) {
        std::cout << " -> " << siralama.front() << " islendi" << std::endl;
        siralama.pop(); // ilk elemani cikar
    }

    std::cout << "Sira bos!" << std::endl;

    return 0;
}
