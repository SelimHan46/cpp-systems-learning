#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::mutex kilit;
std::condition_variable sinyal;
bool yemekHazir = false;

// Asci: Yemek hazırlayıp sinyal gonderecek
void asci() {
    std::cout << "[Asci] Yemek hazirliyorum..." << std::endl;

    // 3 saniye yemek hazırlama
    std::this_thread::sleep_for(std::chrono::seconds(3));

    // Yemek hazir, sinyal gonder
    {
        std::lock_guard<std::mutex> guard(kilit);
        yemekHazir = true;
        std::cout << "[Asci] Yemek HAZIR! Garsona haber veriyorum." << std::endl;
    }

    sinyal.notify_one();  // Garsonu uyandir
}

// Garson: Sinyal gelene kadar bekleyecek
void garson() {
    std::cout << "[Garson] Yemek hazir olana kadar bekliyorum..." << std::endl;

    std::unique_lock<std::mutex> ulock(kilit);
    sinyal.wait(ulock, []{ return yemekHazir; });

    std::cout << "[Garson] Yemek geldi! Masaya goturuyorum." << std::endl;
}

int main() {
    std::cout << "  CONDITION VARIABLE - TEMEL SINYAL" << std::endl;

    std::cout << "Asci yemek hazirlayacak (3 saniye)" << std::endl;
    std::cout << "Garson sinyal gelene kadar bekleyecek\n" << std::endl;

    std::thread t1(garson);
    std::thread t2(asci);

    t1.join();
    t2.join();

    std::cout << "\nSiparis tamamlandi!" << std::endl;
    
    return 0;
}