#include <iostream>
#include <thread>
#include <chrono>

void uzunGorev(std::string isim, int sure) {
    for (int i = 1; i <= sure; i++) {
        std::cout << " [" << isim << "] Calisiyorum... " << i << "/" << sure << " saniye" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
    }
    std::cout << " [" << isim << "] BITTIM!" << std::endl;
}

int main() {
    // 1. JOIN - BEKLE

    std::cout << "========================================" << std::endl;
    std::cout << "  ORNEK 1: JOIN (thread bitmesini bekle)" << std::endl;
    std::cout << "========================================" << std::endl;

    std::cout << "[Ana] Thread baslatiyorum..." << std::endl;
    std::thread t1(uzunGorev, "Thread-1", 3);

    std::cout << "[Ana] Thread'in bitmesini bekliyorum (join)..." << std::endl;
    t1.join();  // 3 saniye bekleyecek

    std::cout << "[Ana] Thread bitti, devam ediyorum!\n" << std::endl;

    std::cout << "==================================" << std::endl;
    std::cout << "   ORNEK 2: DETACH (serbest birak)" << std::endl;
    std::cout << "==================================" << std::endl;

    std::cout << "[Ana] Thread baslatiyorum..." << std::endl;
    std::thread t2(uzunGorev, "Thread-2", 5);

    t2.detach(); // serbest birak, bekleme 

    std::cout << "[Ana] Thread'i serbest biraktim (detach)" << std::endl;
    std::cout << "[Ana] Ben 2 saniye caliscam, thread hala devam ediypr..." << std::endl;

    // Ana program 2 saniye calissin
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "[Ana] Ben bittim! Ama thread hala calisiyor olabilir..." << std::endl;
    std::cout << "[Ana] Program kapaninca detach edilmis thread de olur.\n" << std::endl;

    // 3. JOIN OLMADAN NE OLUR?
    std::cout << "========================================" << std::endl;
    std::cout << "  ORNEK 3: BIRDEN FAZLA THREAD + JOIN" << std::endl;
    std::cout << "========================================" << std::endl;

    std::thread t3(uzunGorev, "Asci-1", 2);
    std::thread t4(uzunGorev, "Asci-2", 3);
    std::thread t5(uzunGorev, "Asci-3", 1);

    std::cout << "[Ana] 3 thread basladi, hepsini bekliyorum...\n" << std::endl;

    t3.join();
    std::cout << "[Ana] Asci-1 bitti!" << std::endl;

    t4.join();
    std::cout << "[Ana] Asci-2 bitti!" << std::endl;

    t5.join();
    std::cout << "[Ana] Asci-3 bitti!" << std::endl;

    std::cout << "\n[Ana] Herkes bitti, program kapaniyor." << std::endl;

    // ==========================================
    //  OZET
    // ==========================================
    //
    // join()   = "Bekle, thread bitsin sonra devam et"
    //            Kullanim: Thread'in sonucuna ihtiyacin varsa
    //            Ornek: Dosya indirme bitsin, sonra ac
    //
    // detach() = "Serbest birak, beklemeden devam et"
    //            Kullanim: Thread'in sonucunu umursamiyorsan
    //            Ornek: Log yazma, arka plan temizligi
    //
    // KURAL: Her thread icin ya join() ya detach() ZORUNLU!
    //        Ikisini de yazmazsan program COKER (terminate)

    return 0;
}
