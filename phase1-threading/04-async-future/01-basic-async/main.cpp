#include <iostream>
#include <future>       // async ve future icin
#include <chrono>
#include <string>
#include <thread>

// Bu fonksiyon bir deger DONDURUYOR (return ile)
// Thread'de std::ref kullanmak zorundaydik
// Async ile direkt return kullanabiliyoruz
int uzunHesaplama(int sayi) {
    std::cout << " [Hesaplama] " << sayi << " icin basliyor..." << std::endl;

    // 2 saniye hesaplama simulasyonu
    std::this_thread::sleep_for(std::chrono::seconds(2));

    int sonuc = sayi * sayi; // karesi 
    std::cout << " [Hesaplama] " << sayi << " icin bitti! Sonuc: " << sonuc << std::endl;
    return sonuc;  // sonuc dondur
}

int main() {
    std::cout << "  STD::ASYNC VE STD::FUTURE" << std::endl;

    std::cout << "--- ORNEK 1: Tek async ---\n" << std::endl;

    // asyc ile gorevi baslat
    // future = "sonuc buradan gelecek" sözü

    std::future<int> sonuc1 = std::async(std::launch::async, uzunHesaplama, 5);

    // Ana thread baska isler yapabilir (beklemek zorunda degil)
    std::cout << "[Ana] Hesaplama arka planda calisiyor..." << std::endl;
    std::cout << "[Ana] Ben baska isler yapiyorum...\n" << std::endl;

    // Sonucu al (hazir degilse bekler)
    int deger = sonuc1.get();
    std::cout << "\n[Ana] Sonuc geldi: " << deger << "\n" << std::endl;

    std::cout << "--- ORNEK 2: 3 async paralel ---\n" << std::endl;

    auto baslangic = std::chrono::steady_clock::now();

    // 3 hesaplamayi AYNI ANDA baslat
    std::future<int> f1 = std::async(std::launch::async, uzunHesaplama, 3);
    std::future<int> f2 = std::async(std::launch::async, uzunHesaplama, 7);
    std::future<int> f3 = std::async(std::launch::async, uzunHesaplama, 10);

    std::cout << "\n[Ana] 3 hesaplama paralel calisiyor...\n" << std::endl;

    // Sonuclari al
    int s1 = f1.get();
    int s2 = f2.get();
    int s3 = f3.get();

    auto bitis = std::chrono::steady_clock::now();
    auto sure = std::chrono::duration_cast<std::chrono::seconds>(bitis - baslangic).count();

    std::cout << "  SONUCLAR" << std::endl;

    std::cout << " 3 * 3 = " << s1 << std::endl;
    std::cout << " 7 * 7 = " << s2 << std::endl;
    std::cout << " 10 * 10 = " << s3 << std::endl;
    std::cout << "\n  Toplam sure: " << sure << " saniye" << std::endl;
    std::cout << "  (Sirayla olsaydi 6 saniye sururdu)" << std::endl;

    return 0;
}