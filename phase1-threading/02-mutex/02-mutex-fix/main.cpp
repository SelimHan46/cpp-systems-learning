#include <iostream>
#include <thread>
#include <vector>
#include <mutex>   // Eklendi: mutex kutuphanesi

int sayac = 0;
std::mutex kilit; // Eklendi: sayaci koruyacak kilit

void arttir(int threadId) {
    for (int i = 0; i < 100000; i++) {
        // Eklendi: lock_guard ile kilitle
        // Bu satir diyor ki: "Bu scope icinde sadece 1 thread girebilir"
        // Scope bitince otomatik acilir
        std::lock_guard<std::mutex> guard(kilit);
        sayac++;
    }
}

int main() {
    std::cout << " MUTEX ILE COZUM" << std::endl;

    int threadSayisi = 10;
    int beklenen = threadSayisi * 100000;

    std::cout << " 10 thread, her biri 100.000 kere sayaci artiracak" << std::endl;
    std::cout << " Bu sefer MUTEX ile korunuyor!\n" << std::endl;

    std::vector<std::thread> threadler;
    for (int i = 0; i < threadSayisi; i++) {
        threadler.push_back(std::thread(arttir, i));
    }

    for (auto& t : threadler) {
        t.join();
    }

    std::cout << " SONUC" << std::endl;
    std::cout << " Beklenen: " << beklenen << std::endl;
    std::cout << " Gercek: " << sayac << std::endl;

    if (sayac == beklenen) {
        std::cout << " DOGRU! Mutex sayesinde kayip yok." << std::endl;
    }

    return 0;
}
