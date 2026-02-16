# Phase 1 - Threading Notları

## Thread Nedir?

Thread, bir programın içinde aynı anda çalışan farklı iş parçacıklarıdır.

### Benzetme: Restoran
- **Process** = restoranın kendisi (programın kendisi)
- **Thread** = restorandaki aşçılar (programın içindeki iş parçacıkları)
- Tek aşçı (tek thread) = her şeyi sırayla yapar, yavaş
- 4 aşçı (4 thread) = her şeyi aynı anda yapar, hızlı

### Neden Thread Lazım?

Thread'siz (sırayla):

Görev 1: 3 saniye
Görev 2: 3 saniye
Görev 3: 3 saniye
Toplam: 9 saniye


Thread'li (paralel):

Görev 1: 3 saniye ─┐
Görev 2: 3 saniye ─┼─ Aynı anda çalışır
Görev 3: 3 saniye ─┘
Toplam: 3 saniye


## std::thread Kullanımı

### Thread Oluşturma-cpp
#include <thread>

void gorev(std::string isim) {
    std::cout << isim << " calisiyor" << std::endl;
}

std::thread t1(gorev, "Thread-1");  // thread olustur ve baslat
t1.join();                           // bitmesini bekle


### join() vs detach()
- **join()** = "Bu thread bitene kadar bekle." Ana program thread bitmeden kapanmaz.
- **detach()** = "Thread'i serbest bırak, kendi halinde çalışsın." Ana program beklemez.
- Her thread'de ya join() ya detach() ZORUNLU çağrılmalı, yoksa program çöker.

### Thread'e Parametre Geçme-cpp
void selamla(std::string isim, int yas) {
    std::cout << isim << " " << yas << " yasinda" << std::endl;
}

std::thread t(selamla, "Selim", 25);  // parametre virgülle geçilir
t.join();
```

### Derleme
Thread kullanırken -pthread bayrağı ZORUNLU: - bash
g++ -std=c++17 -pthread -o main main.cpp


### Zamanlama (sleep)-cpp
#include <chrono>

// 2 saniye bekle
std::this_thread::sleep_for(std::chrono::seconds(2));

// 500 milisaniye bekle
std::this_thread::sleep_for(std::chrono::milliseconds(500));


## Önemli Kurallar
1. Thread oluşturunca ya join() ya detach() çağır
2. join() çağrılmazsa program ÇÖKER
3. Derleme sırasında -pthread EKLE
4. Thread'ler aynı anda çalışır, sıra GARANTİ DEĞİL