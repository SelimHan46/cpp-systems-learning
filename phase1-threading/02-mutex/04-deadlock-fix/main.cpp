#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex kilit_A;
std::mutex kilit_B;

// Thread-1: once A sonra B
void thread1_gorevi() {
    std::cout << "[Thread-1] kilit_A'yi almaya calisiyorum..." << std::endl;
    kilit_A.lock();
    std::cout << "[Thread-1] kilit_A'yi ALDIM!" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "[Thread-1] kilit_B'yi almaya calisiyorum..." << std::endl;
    kilit_B.lock();
    std::cout << "[Thread-1] kilit_B'yi ALDIM!" << std::endl;

    kilit_B.unlock();
    kilit_A.unlock();
    std::cout << "[Thread-1] Isim bitti, kilitleri biraktim." << std::endl;
}

// Thread-2: AYNI SIRA! once A sonra B
// Deadlock kodunda burasi once B sonra A idi, o yuzden takiliyordu
// Simdi ayni sira -> deadlock yok
void thread2_gorevi() {
    std::cout << "[Thread-2] kilit_A'yi almaya calisiyorum..." << std::endl;
    kilit_A.lock();
    std::cout << "[Thread-2] kilit_A'yi ALDIM!" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "[Thread-2] kilit_B'yi almaya calisiyorum..." << std::endl;
    kilit_B.lock();
    std::cout << "[Thread-2] kilit_B'yi ALDIM!" << std::endl;

    kilit_B.unlock();
    kilit_A.unlock();
    std::cout << "[Thread-2] Isim bitti, kilitleri biraktim." << std::endl;
}

int main() {
    std::cout << "DEADLOCK COZUMU" << std::endl;

    std::cout << "Cozum: Ikisi de AYNI SIRADA kilitliyor" << std::endl;
    std::cout << "Thread-1: once A, sonra B" << std::endl;
    std::cout << "Thread-2: once A, sonra B (ayni sira!)\n" << std::endl;

    std::thread t1(thread1_gorevi);
    std::thread t2(thread2_gorevi);

    t1.join();
    t2.join();

    std::cout << "\nProgram bitti! Deadlock olmadi." << std::endl;

    return 0;
}