#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <functional>     // std::function icin (herhangi bir fonksiyonu tutabilir)
#include <future>       // std::packaged_task ve std::future icin

class ThreadPool {
    public:
    // CONSTRUCTOR: Thread pool'u olustur
    // threadSayisi = kac worker thread olacak
    ThreadPool(int threadSayisi) : durdur(false) {
        std::cout << "[Pool] " << threadSayisi << " worker thread olusturuluyor..." << std::endl;

        // Worker thread'leri olustur
        for (int i = 0; i < threadSayisi; i++) {
            // Her worker thread bu lambda calistiracak
            // Lambda = isimsiz fonksiyon, [this] = class'in degiskenlerine erisebilir
            workers.push_back(std::thread([this, i]() {
                workerDongusu(i);
            }));
        }
    }

    // DESTRUCTOR: Thread pool'u kapat (graceful shutdown)
    ~ThreadPool() {
        std::cout << "\n[Pool] Kapatiliyor..." << std::endl;

        // "Artik dur" sinyali gonder
        {
            std::lock_guard<std::mutex> guard(kilit);
            durdur = true;
        }

        // Tum worker'lari uyandir (uyuyanlar varsa)
        sinyal.notify_all();

        // Tum thread'leri bitmesini bekle
        for (auto& t : workers) {
            t.join();
        }

        std::cout << "[Pool] Tum worker'lar durdu." << std::endl;
    }

    // GOREV EKLE: Herhangi bir fonksiyonu kuyruga ekle
    // Template kullaniyoruz cunku farkli turde fonksiyonlar eklenebilir
    // F = fonksiyon tipi, Args = paramete tipleri
    template<typename F, typename... Args>
    auto ekle(F&& fonksiyon, Args&&... args) -> std::future<decltype(fonksiyon(args...))> {
        // Fonksiyonun donus tipini bul
        using donusTipi = decltype(fonksiyon(args...));

        // Fonksiyonu paketle (packaged_task)
        // shared_ptr kullaniyoruz cunku queue'ya kopyalanamaz
        auto gorev = std::make_shared<std::packaged_task<donusTipi()>>(
            std::bind(std::forward<F>(fonksiyon), std::forward<Args>(args)...)
        );

        // Future'i al (sonucu buradan alacagiz)
        std::future<donusTipi> sonuc = gorev->get_future();

        // Gorevi kuyruga ekle
        {
            std::lock_guard<std::mutex> guard(kilit);

            // Pool kapatildiysa yeni gorev kabul etme
            if (durdur) {
                throw std::runtime_error("Pool kapatildi, yeni gorev eklenemez!");
            }

            // Gorevi kuyruga koy
            // Lambda ile packaged_task'i calistir
            gorevKuyrugu.push([gorev]() {
                (*gorev)();
            });
        }

        // Bir worker'i uyandir: "Yeni gorev var!"
        sinyal.notify_one();

        // Future'i dondur (cagiran kisma sonucu alabilsin)
        return sonuc;
    }

    private:
        // Her worker thread bu fonksiyonu calistiriyor
        void workerDongusu(int id) {
            std::cout << "  [Worker-" << id << "] Hazir, gorev bekliyorum." << std::endl;

            while (true) {
                std::function<void()> gorev;

                // Kuyruktan gorev al 
                {
                    std::unique_lock<std::mutex> ulock(kilit);

                    // Bekle: gorev gelene kadar VEYA pool kapanana kadar 
                    sinyal.wait(ulock, [this] () {
                        return !gorevKuyrugu.empty() || durdur;
                    });

                    // Pool kapaniyor ve kuyruk bos -> cik
                    if (durdur && gorevKuyrugu.empty()) {
                        std::cout << "  [Worker-" << id << "] Durduruluyor." << std::endl;
                        return;
                    }

                    // Kuyruktan gorevi al 
                    gorev = gorevKuyrugu.front();
                    gorevKuyrugu.pop();
                }

                // Gorevi calistir (mutex DISINDA, boylece diger worker'lar kuyruga erisabilir)
                gorev();
            }
        }

        // Worker thread'ler 
        std::vector<std::thread> workers;

        // Gorev kuyrugu - std::function<void()> herhangi bir fonksiyonu tutabilir
        std::queue<std::function<void()>> gorevKuyrugu;

        // Senkronizasyon
        std::mutex kilit;
        std::condition_variable sinyal;

        // Pool kapanma flag'i 
        bool durdur;
};


#endif