#include <iostream>
#include <string>
#include <memory>       // smart pointer'lar burada

int main() {
    std::cout << "=== UNIQUE_PTR ===" << std::endl;

    std::unique_ptr<int> p1 = std::make_unique<int>(42);
    std::cout << "p1 degeri: " << *p1 << std::endl;

    // Scope (suslü parantez) bitince otomatik silinir
    // delete yazmana gerek yok!

    // unique_ptr'nin kurali: TEK SAHIP
    // Baska bir pointer'a veremezsin:
    // std::unique_ptr<int> p2 = p1; HATA! kopyalanamaz

    // Ama tasinabilir (sahiplik devri):
    std::unique_ptr<int> p2 = std::move(p1);
    std::cout << "p2 degeri (tasinmis): " << *p2 << std::endl;
    // Artik p1 bos, p2 sahip

    // p1 kullanirsan PATLAT! (nullptr)
    if (p1 == nullptr) {
        std::cout << "p1 artik bos (sahiplik p2'ye gecti)" << std::endl;
    }

    std::cout << "\n=== SHARED_PTR ===" << std::endl;

    // Birden fazla pointer ayni seyi gosterebilir
    std::shared_ptr<int> s1 = std::make_shared<int>(100);
    std::cout << "s1 degeri: " << *s1 << std::endl;
    std::cout << "Sahip sayisi: " << s1.use_count() << std::endl;
    // Sahip sayisi: 1

    // Kopyalanabilir! (unique_ptr'den farki bu)
    std::shared_ptr<int> s2 = s1; // ikisi de ayni yeri gosteriyor
    std::cout << "\ns2 = s1 yapildi" << std::endl;
    std::cout << "s1 degeri: " << *s1 << std::endl;
    std::cout << "s2 degeri: " << *s2 << std::endl;
    std::cout << "Sahip sayisi: " << s1.use_count() << std::endl;
    // Sahip sayisi: 2

    // Birinden degistirince digeri de degisir (ayni yer!)
    *s2 = 999;
    std::cout << "\ns2 uzerinden 999 yapildi" << std::endl;
    std::cout << "s1 degeri: " << *s1 << std::endl; // 999
    std::cout << "s2 degeri: " << *s2 << std::endl; // 999
    
    // Son sahip scope'dan cikinca otomatik silinir

    std::cout << "\n=== GERCEK ORNEK ===" << std::endl;

    struct Oyuncu {
        std::string isim;
        int can;

        Oyuncu(std::string i, int c) : isim(i), can(c) {
            std::cout << " [+] " << isim << " olusturuldu (can: " << can << ")" << std::endl;
        }
        ~Oyuncu() {
            std::cout << " [-] " << isim << " otomatik silindi!" << std::endl;
        }
    };

    { // Bu suslu parantez bir SCOPE
      std::cout << "\nScope basladi:" << std::endl;
      auto oyuncu1 = std::make_unique<Oyuncu>("Selim", 100);
      auto oyuncu2 = std::make_unique<Oyuncu>("Boss", 200);
      std::cout << "Scope icindeyiz, oyuncular yasiyor" << std::endl;  
    } // Scope bitti -> destructor otomatik cagrildi!

    std::cout << "\nScope bitti, oyuncular otomatik silindi" << std::endl;

    // OZET
    // new/delete -> KULLANMA (unutabilirsin)
    // unique_ptr -> tek sahip, kopyalanamaz, tasinabilir
    // shared_ptr -> ortak sahip, kopyalanabilir
    // make_unique -> unique_ptr olusturmanin dogru yolu
    // make_shared -> shared_ptr olusturmanin dogru yolu

    // KURAL: MODERN C++'da new ve delete yazma,
    //        smart pointer kullan!

    return 0;
}