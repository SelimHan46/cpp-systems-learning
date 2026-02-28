#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>   // sinyal mekanizması
#include <queue>                // producer-consumer arasindaki kuyruk
#include <chrono>           
#include <random>               

// Paylasilan degiskenler - iki thread de bunlari kullanacak
std::queue<int> kuyruk;         // veriler burada bekliyor
std::mutex kilit;               // kuyruga ayni anda iki thread dokunmasin
std::condition_variable sinyal; // "veri geldi!" haberi
bool uretimBitti = false;       // producer isi bitirdi mi?

// PRODUCER: Sayi uretip kuyruga koyuyor
void producer() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < 10; i++) {
        int sayi = dis(gen);    // rastgele sayi uret

        // Mutex ile kilitle, kuyruga koy
        {
            std::lock_guard<std::mutex> guard(kilit);
            kuyruk.push(sayi);
            std::cout << "[Producer] Uretti: " << sayi 
                      << " (kuyrukta: " << kuyruk.size() << ")" << std::endl;
        }

        // Consumer'a haber ver: "kuyrukta veri var!"
        sinyal.notify_one();

        // 300ms bekle (uretim simülasyonu)
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

     // 10 sayi uretildi, consumer'a "artik bekleme" de
    {
        std::lock_guard<std::mutex> guard(kilit);
        uretimBitti = true;
        std::cout << "\n[Producer] 10 sayi uretti, urettim BITTI." << std::endl;
    }

    sinyal.notify_one();    // son sinyal: "is bitti"
}

// CONSUMER: Kuyruktan sayi alip isliyor
void consumer() {
    while (true) {
        std::unique_lock<std::mutex> ulock(kilit);

        // Bekle: kuyrukta veri olana kadar VEYA uretim bitene kadar uyu
        // CPU harcamadan bekliyor, sinyal gelince uyaniyor
        sinyal.wait(ulock, [] {return !kuyruk.empty() || uretimBitti;
        
        });

        // Uyandi! Kuyruktaki tum verileri al ve isle
        while (!kuyruk.empty()) {
            int sayi = kuyruk.front();  // kuyrugun onundekini oku
            kuyruk.pop();               // kuyruktan cikar
            std::cout << " [Consumer] Tuketti: " << sayi << std::endl;
        }

        // Uretim bittiyse be kuyruk bossa artik cik
        if (uretimBitti && kuyruk.empty()) {
            std::cout << "[Consumer] Uretim bitti, cikiyorum." << std::endl;
            break;  // donguden cik
        }
    }
}

int main() {
    std::cout << "  PRODUCER - CONSUMER PATTERN" << std::endl;

    // Iki thread baslat: biri uretecek, digeri tuketecek
    std::thread t1(producer);
    std::thread t2(consumer);

    // Ikisinin de bitmesini bekle
    t1.join();
    t2.join();

    std::cout << "\nProgram bitti!" << std::endl;

    return 0;
}