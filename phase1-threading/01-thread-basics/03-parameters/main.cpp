#include <iostream>
#include <thread>
#include <string>

// 1. BY VALUE (deger ile gecme)
// Fonksiyon degiskenin KOPYASINI alir
// Orjinal degismez
void byValue(int sayi) {
    sayi = 999; // kopyayi degistirdik
    std::cout << " [byValue] icerde sayi = " << sayi << std::endl;
}

// 2. BY REFERENCE (referans ile gecme)
// Fonksiyon degiskenin KENDISINI alir
// Orjinal DEGISIR
void byReference(int& sayi) {
    sayi = 999; // orjinali degistirdik
    std::cout << " [byRef] icerde sayi = " << sayi << std::endl;
}

// 3. BIRDEN FAZLA PARAMETRE
void oyuncuBilgi(std::string isim, int can, double hasar) {
    std::cout << " [Oyuncu] " << isim
              << " | Can: "  << can
              << " | Hasar: " << hasar << std::endl;
}

int main() {
    // TEST 1: BY VALUE
    std::cout << "================================" << std::endl;
    std::cout << "  BY VALUE (kopya)" << std::endl;
    std::cout << "================================" << std::endl;

    int a = 42;
    std::cout << "Oncesi: a = " << a << std::endl;

    std::thread t1(byValue, a); // a'nin KOPYASI gider
    t1.join();

    std::cout << "Sonrasi: a = " << a << std::endl; // hala 42
    std::cout << "-> Thread degistirdi ama orjinal ayni kaldi\n" << std::endl;

    // TEST 2: BY REFERENCE
    std::cout << "================================" << std::endl;
    std::cout << "  BY REFERENCE (referans)" << std::endl;
    std::cout << "================================" << std::endl;

    int b = 42;
    std::cout << "Oncesi: b = " << b << std::endl;

    // std::ref() ZORUNLU! Thread'e referans gecerken
    // normal & calismaz, std::ref() kullanman lazim
    std::thread t2(byReference, std::ref(b));
    t2.join();

    std::cout << "Sonrasi: b = " << b << std::endl; // 999 oldu!
    std::cout << "-> Thread orjinali degistirdi\n" << std::endl;

    // TEST 3: BIRDEN FAZLA PARAMETRE
    std::cout << "================================" << std::endl;
    std::cout << "  BIRDEN FAZLA PARAMETRE" << std::endl;
    std::cout << "================================" << std::endl;

    std::thread t3(oyuncuBilgi, "Selim", 100, 25.5);
    std::thread t4(oyuncuBilgi, "Boss", 200, 50.0);
    std::thread t5(oyuncuBilgi, "Mage", 80, 75.3);

    t3.join();
    t4.join();
    t5.join();

    // ==========================================
    //  OZET
    // ==========================================
    //
    // By Value:     std::thread t(func, degisken);
    //               Kopya gider, orjinal degismez
    //
    // By Reference: std::thread t(func, std::ref(degisken));
    //               Orjinal gider, degisebilir
    //               DIKKAT: std::ref() zorunlu!
    //
    // Birden fazla: std::thread t(func, param1, param2, param3);
    //               Virgullerle sirala

    return 0;
}