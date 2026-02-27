#include <iostream>
#include <thread>
#include <fstream>    // Dosya okumak icin
#include <string>
#include <vector>

// Bu fonksiyon her thread'de calisacak
// dosyaAdi: Hangi dosyayi okuyacak
// sonuc: satir sayisini buraya yazacak (referans! thread'den sonuc almak icin)
void satirSayisi(std::string dosyaAdi, int& sonuc) {

    // Dosyayi ac
    std::ifstream dosya(dosyaAdi);

    // Dosya acilmadiysa hata ver
    if (!dosya.is_open()) {
        std::cout << " [HATA] " << dosyaAdi << " acilmadi!" << std::endl;
        sonuc = -1;
        return; // Fonksiyondan cik
    }

    int sayac = 0;          // Satir sayacimiz
    std::string satir;      // Okunan her satiri buraya koyacak

    // Dosyayi satir satir oku, her satirda sayaci 1 arttir
    while (std::getline(dosya,satir)) {
        sayac++;
    }

    dosya.close();  // Dosyayi kapat
    sonuc = sayac;  // Sonucu referans ile geri dondur

    std::cout << " [Thread] " << dosyaAdi << " -> " << sayac << " satir" << std::endl;
}

int main() {
    std::cout << " PARALEL DOSYA OKUYUCU" << std::endl;

    // 4 dosyanin sonuclarini tutacak vector (hepsi 0 ile baslar)
    std::vector<int> sonuclar(4, 0);

    // 4 dosyanin isimleri
    std::vector<std::string> dosyalar = {
        "dosya1.txt", "dosya2.txt", "dosya3.txt", "dosya4.txt"
    };

    // Thread'leri tutacak vector
    std::vector<std::thread> threadler;

    std::cout << "\n4 thread baslatiliyor...\n" << std::endl;

    // 4 thread olustur 
    // Her thread: satirSay fonksiyonunu calistır
    //             dosyalar[i] -> hangi dosya
    //             std::ref(sonuclar[i]) -> sonucu buraya yaz (referans)
    for (int i = 0; i < 4; i++) {
        threadler.push_back(
            std::thread(satirSayisi, dosyalar[i], std::ref(sonuclar[i]))
        );
    }

    // 4 thread'in bitmesini bekle
    for (auto& t : threadler) {
        t.join();
    }

    // Sonuclari goster
    std::cout << " SONUCLAR" << std::endl;

    int toplam = 0;
    for (int i = 0; i < 4; i++) {
        std::cout << " " << dosyalar[i] << " -> " << sonuclar[i] << " satir" << std::endl;
        toplam += sonuclar[i];
    }

    std::cout << "\n Toplam: " << toplam << " satir" << std::endl;
    std::cout << " (4 dosya, 4 thread ile paralel okundu)" << std::endl;
}
