#include <iostream>
#include <thread>
#include <vector>

// Global sayac - tum thread'ler bunu paylasacak
int sayac = 0;

// Her thread bu fonksiyonu calistiracak
// 100.000 kere sayaci 1 arttir
void arttir(int threadId) {
    for (int i = 0; i < 100000; i++) {
        // BU SATIR TEHLIKELI!
        // sayac++ aslında 3 adim:
        // 1. sayac'in degerini oku (ornek: 42)
        // 2. 1 ekle (43)
        // 3. geri yaz              (sayac = 43)

        // Iki thread ayni anda okursa:
        //   Thread-1: oku -> 42
        //   Thread-2: oku -> 42 (ayni degeri okudu!)
        //   Thread-1: yaz -> 43
        //   Thread-2: yaz -> 43 (44 yazmasi gerekirken 43 yazdi!)
        //   Bir artis KAYBOLDU!
        sayac++;
    }

}

int main() {
    std::cout <<  " RACE CONDITION GOSTERIMI" << std::endl;

    int threadSayisi = 10;
    int beklenen = threadSayisi * 100000;   // 10 * 100000 = 1.000.000

    std::cout << "10 thread, her biri 100.000 kere sayaci artiracak" << std::endl;
    std::cout << "Beklenen sonuc: " << beklenen << "\n" << std::endl;

    // 10 thread olustur
    std::vector<std::thread> threadler;
    for (int i = 0; i < threadSayisi; i++) {
        threadler.push_back(std::thread(arttir, i));
    }

    // Hepsini bekle
    for (auto& t : threadler) {
        t.join();
    }

    // Sonucu goster 
    std::cout << " SONUC" << std::endl;
    std::cout << " Beklenen: " << beklenen << std::endl;
    std::cout << " Gercek: " << sayac << std::endl;

    if (sayac != beklenen) {
        std::cout << " KAYIP: " << beklenen - sayac << " artis kayboldu!" << std::endl;
        std::cout << "\n NEDEN? Race condition!" << std::endl;
        std::cout << " Thread'ler ayni anda sayac'a yazdi, " << std::endl;
        std::cout << " birbirlerinin degerini ezdi." << std::endl;
    }
    else {
        std::cout << " Dogru! (sansli gidis, ama garanti degil)" << std::endl;
    }

    return 0;
}
