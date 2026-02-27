#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex kilit_A;
std::mutex kilit_B;

// Thread-1: once A sonra B kilitliyor
void thread1_gorevi() {
    std::cout << "[Thread-1] kilit_A'yi almaya calisiyorum..." << std::endl;
    kilit_A.lock();
    std::cout << "[Thread-1] kilit_A'yi ALDIM!" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "[Thread-1] kilit_B'yi almaya calisiyorum..." << std::endl;
    kilit_B.lock();
    std::cout << "[Thread-1] kilit_B'yi ALDIM" << std::endl;

    kilit_B.unlock();
    kilit_A.unlock();
}

// Thread-2: once B sonra A kilitliyor (Ters sıra!)
void thread2_gorevi() {
    std::cout << "[Thread-2] kilit_B'yi almaya calisiyorum..." << std::endl;
    kilit_B.lock();
    std::cout << "[Thread-2] kilit_B'yi ALDIM!" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "[Thread-2] kilit_A'yi almaya calisiyorum..." << std::endl;
    kilit_A.lock();
    std::cout << "[Thread-2] kilit_A'yi ALDIM!" << std::endl;

    kilit_A.unlock();
    kilit_B.unlock();
}

int main() {
    std::cout << " DEADLOCK GOSTERIMI" << std::endl;

    std::cout << "Thread-1: A sonra B kilitleyecek" << std::endl;
    std::cout << "Thread-2: B sonra A kilitleyecek" << std::endl;
    std::cout << "Program donacak, Ctrl+C ile kapat. \n" << std::endl;

    std::thread t1(thread1_gorevi);
    std::thread t2(thread2_gorevi);

    t1.join();
    t2.join();

    std::cout << "BU mesaji gormeyeceksin" << std::endl;

    return 0;
}
