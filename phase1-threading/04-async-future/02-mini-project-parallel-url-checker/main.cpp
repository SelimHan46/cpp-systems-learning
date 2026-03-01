#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <random>

// Bir URL'nin boyutunu hesapla (SIMULASYON)
// Gercekte internete baglanmiyoruz, rastgele sure bekleyip
// Rastgele boyut donduruyoruz, Ama mantik ayni:
// Bir istek gonder, cevap bekle, boyutu dondur.
int urlBoyutuHesapla(std::string url) {
    std::cout << " [Basliyor] " << url << std::endl;

    // Rastgele 1-3 saniye bekle (internet gecikmesi simülasyonu)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> sureDis(1,3);
    int bekleme = sureDis(gen);
    std::this_thread::sleep_for(std::chrono::seconds(bekleme));

    // Rastgele boyut uret (1000-50000 byte arasi)
    std::uniform_int_distribution<> boyutDis(1000, 50000);
    int boyut = boyutDis(gen);

    std::cout << " [Bitti] " << url << " -> " << boyut << " byte (" << bekleme << " sn)" << std::endl;
    return boyut;
}

int main() {
    std::cout << "  PARALEL URL BOYUT HESAPLAYICI" << std::endl;

    // 5 tane URL (gercek degil, simulasyon)
    std::vector<std::string> urls = {
        "https://google.com",
        "https://github.com",
        "https://stackoverflow.com",
        "https://cppreference.com",
        "https://youtube.com"
    };

    // SIRAYLA (YAVAS)
     std::cout << "--- SIRAYLA ---\n" << std::endl;
     auto baslangic1 = std::chrono::steady_clock::now();

     std::vector<int> sonuclarSirali;
     for (auto& url : urls) {
        sonuclarSirali.push_back(urlBoyutuHesapla(url));
     }

     auto bitis1 = std::chrono::steady_clock::now();
     auto sure1 = std::chrono::duration_cast<std::chrono::seconds>(bitis1 - baslangic1).count();

     //  PARALEL (HIZLI) - ASYNC ILE
     std::cout << "\n--- PARALEL (ASYNC) ---\n" << std::endl;
     auto baslangic2 = std::chrono::steady_clock::now();

     // 5 async gorevi ayni anda baslat
     std::vector<std::future<int>> futures;
     for (auto& url : urls) {
        futures.push_back(
            std::async(std::launch::async, urlBoyutuHesapla, url)
        );
     }

     // Sonuclari topla 
     std::vector<int> sonuclarParalel;
     for (auto& f : futures) {
        int boyut = f.get();
        sonuclarParalel.push_back(boyut);
     }

     auto bitis2 =  std::chrono::steady_clock::now();
     auto sure2 = std::chrono::duration_cast<std::chrono::seconds>(bitis2 - baslangic2).count();

     std::cout << "  SONUCLAR" << std::endl;
     std::cout << "\nParalel sonuclar:" << std::endl;
     int toplam = 0;
     for (int i = 0; i < urls.size(); i++) {
        std::cout << " " << urls[i] << " -> " << sonuclarParalel[i] << " byte" << std::endl;
        toplam += sonuclarParalel[i];
     }
     
    std::cout << "\n  Toplam: " << toplam << " byte" << std::endl;
    std::cout << "\n  Sirayla sure:  " << sure1 << " saniye" << std::endl;
    std::cout << "  Paralel sure:  " << sure2 << " saniye" << std::endl;
    std::cout << "  Kazanc:        " << sure1 - sure2 << " saniye daha hizli!" << std::endl;

    return 0;

}