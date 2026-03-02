#include <iostream>
#include <chrono>
#include "src/thread_pool.hpp"      // bizim thread pool'umuz

// Test fonksiyonu: bir sayi alip karesini dondur
int kareHesapla(int sayi) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return sayi * sayi;
}

// Test fonksiyonu: iki sayi toplayip dondur
int topla(int a, int b) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return a + b;
}

int main() {
    std::cout << "  THREAD POOL TESTI" << std::endl;

    // 3 worker thread ile pool olustur
    ThreadPool pool(3);

    std::cout << "\n5 gorev ekleniyor (3 worker var)...\n" << std::endl;

     auto baslangic = std::chrono::steady_clock::now();

    // 5 gorev ekle (3 worker var, ilk 3 hemen baslar, digerleri bekler)
    auto f1 = pool.ekle(kareHesapla, 5);
    auto f2 = pool.ekle(kareHesapla, 10);
    auto f3 = pool.ekle(kareHesapla, 7);
    auto f4 = pool.ekle(topla, 3, 4);
    auto f5 = pool.ekle(topla, 100, 200);

    // Sonuclari al
    int s1 = f1.get();
    int s2 = f2.get();
    int s3 = f3.get();
    int s4 = f4.get();
    int s5 = f5.get();

    auto bitis = std::chrono::steady_clock::now();
    auto sure = std::chrono::duration_cast<std::chrono::milliseconds>(bitis - baslangic).count();

    // Temiz sonuclar
    std::cout << "  SONUCLAR" << std::endl;
    std::cout << "  5 * 5     = " << s1 << std::endl;
    std::cout << "  10 * 10   = " << s2 << std::endl;
    std::cout << "  7 * 7     = " << s3 << std::endl;
    std::cout << "  3 + 4     = " << s4 << std::endl;
    std::cout << "  100 + 200 = " << s5 << std::endl;

    std::cout << "\n  Toplam sure: " << sure << " ms" << std::endl;
    std::cout << "  (Sirayla olsaydi ~3500 ms sururdu)" << std::endl;
    std::cout << "  (3 worker ile paralel: ~" << sure << " ms)\n" << std::endl;

    return 0;
}