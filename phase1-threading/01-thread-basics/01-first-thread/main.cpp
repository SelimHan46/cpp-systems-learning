#include <iostream>
#include <thread>   // thread kutuphanesi
#include <chrono>   // zamanlama icin (sleep)

// Bu fonksiyon AYRI bir thread'de calisacak
void asciGorevi(std::string yemek, int sure) {
    std::cout << "[BASLADI] " << yemek << " hazirlaniyor..." << std::endl;

    // "sure" saniye bekle (yemek pisirme simulasyonu)
    std::this_thread::sleep_for(std::chrono::seconds(sure));

    std::cout << "[BITTI] " << yemek << " hazir! (" << sure << " saniye)" << std::endl; 
}

int main() {
    // ONCE THREAD'SIZ (SIRAYLA) DENEYELIM
    std::cout << "================================" << std::endl;
    std::cout << " THREAD'SIZ - TEK ASCI (SIRAYLA)" << std::endl;
    std::cout << "================================" << std::endl;

    auto baslangic1 = std::chrono::steady_clock::now();

    // Tek asci her seyi sirayla yapıyor
    asciGorevi("Corba", 2);
    asciGorevi("Et", 3);
    asciGorevi("Tatli", 2);

    auto bitis1 = std::chrono::steady_clock::now();
    auto sure1 = std::chrono::duration_cast<std::chrono::seconds>(bitis1 - baslangic1).count();
    std::cout << "Toplam sure: " << sure1 << " saniye\n" << std::endl;

    std::cout << "=============================" << std::endl;
    std::cout << "   THREAD'LI 3 ASCI (PARALEL)" << std::endl;
    std::cout << "=============================" << std::endl;

    auto baslangic2 = std::chrono::steady_clock::now();

    // 3 farkli thread olustur (3 asci)
    // Her thread ayni fonksioynu farklı parametrelerle calistiriyor
    std::thread asci1(asciGorevi, "Corba", 2);
    std::thread asci2(asciGorevi, "Et", 3);
    std::thread asci3(asciGorevi, "Tatli", 2);

    // join() = "bu thread bitene kadar bekle"
    // join() yazmazsan program thread bitmeden kapanir -> hata
    asci1.join();   // corba bitene kadar bekle
    asci2.join();   // et bitene kadar bekle
    asci3.join();   // tatli bitene kadar bekle
    
    auto bitis2 = std::chrono::steady_clock::now();
    auto sure2 = std::chrono::duration_cast<std::chrono::seconds>(bitis2 - baslangic2).count();
    std::cout << "Toplam sure: " << sure2 << " saniye" << std::endl;

    // KARSILASTIRMA
    std::cout << "\n=================================" << std::endl;
    std::cout << "  SONUC" << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Thread'siz: " << sure1 << " saniye (sirayla)" << std::endl;
    std::cout << "Thread'li: " << sure2 << " saniye (paralel)" << std::endl;
    std::cout << "Kazanc:    " << sure1 - sure2 << " saniye daha hizli!" << std::endl;

    return 0;
}


