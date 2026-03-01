# Hafta 2, Gün 3-4: Thread Pool Notları

## Thread Pool Nedir?

Sabit sayıda thread sürekli hazır bekliyor. Görev gelince boştaki thread alıyor, bitirince yeni görev bekliyor. Thread oluşturma maliyetini ortadan kaldırır.

### Benzetme: Restoran Mutfağı

- **Thread Pool olmadan:** Her sipariş için yeni aşçı tut, bitince kov. Yavaş ve pahalı.
- **Thread Pool ile:** 5 aşçı sürekli hazır. Sipariş gelince boştaki alıyor. Hızlı ve verimli.

---

## Neden Lazım?

| Durum | Thread Pool olmadan | Thread Pool ile |
|-------|-------------------|-----------------|
| 100 görev | 100 thread oluştur (bilgisayar çöker) | 4 thread, 100 görevi sırayla yapar |
| Thread maliyeti | Her seferinde oluştur/yok et (yavaş) | Bir kere oluştur, sürekli kullan (hızlı) |
| Kaynak kullanımı | Kontrolsüz, RAM şişer | Sabit, öngörülebilir |

---

## Thread Pool Mimarisi

```
                    Görev Kuyruğu (Task Queue)
                   ┌──────────────────────┐
Görev ekle ──────> │ Görev1  Görev2  ...  │
                   └──────────┬───────────┘
                              │
                ┌─────────────┼─────────────┐
                │             │             │
           Worker-1      Worker-2      Worker-3
           (çalışıyor)  (boşta)       (çalışıyor)
```

1. Dışarıdan görevler kuyruğa eklenir
2. Worker thread'ler kuyruktan görev alır
3. Görevi yapar, bitince tekrar kuyruğa bakar
4. Kuyruk boşsa condition variable ile bekler (CPU harcamaz)

---

## Kullanılan Kavramlar

Thread pool şimdiye kadar öğrendiğin **her şeyi** kullanır:

| Kavram | Thread Pool'da nerede? |
|--------|----------------------|
| **std::thread** | Worker thread'ler |
| **std::queue** | Görev kuyruğu |
| **std::mutex** | Queue'ya güvenli erişim |
| **std::condition_variable** | "Yeni görev var!" sinyali |
| **std::packaged_task** | Görevi paketle, sonucu future ile al |
| **std::future** | Görevin sonucunu al |
| **std::function** | Herhangi bir fonksiyonu görev olarak ekle |

---

## Temel Kullanım (Bitince Böyle Kullanacağız)

```cpp
// Thread pool olustur (4 worker thread)
ThreadPool pool(4);

// Gorev ekle ve sonucunu al
auto sonuc1 = pool.ekle(hesapla, 5);
auto sonuc2 = pool.ekle(hesapla, 10);

// Sonuclari al
int s1 = sonuc1.get();  // 25
int s2 = sonuc2.get();  // 100
```

---

## Graceful Shutdown (Düzgün Kapanma)

Thread pool kapanırken:
1. Yeni görev kabul etmeyi durdur
2. Kuyruktaki mevcut görevlerin bitmesini bekle
3. Worker thread'lere "artık dur" sinyali gönder
4. Tüm thread'lerin bitmesini bekle (join)

```cpp
// Destructor'da otomatik kapanma
ThreadPool::~ThreadPool() {
    durdur = true;                    // "artık dur" flag'i
    sinyal.notify_all();              // tüm worker'ları uyandır
    for (auto& t : threadler) {
        t.join();                     // hepsinin bitmesini bekle
    }
}
```

---

## Önemli Kurallar

1. Worker thread sayısı genelde **CPU çekirdek sayısı** kadar olur
2. Queue'ya erişim **her zaman mutex ile** korunmalı
3. Boştaki worker'lar **condition variable** ile uyumalı (busy-wait yapma)
4. Shutdown sırasında **kuyruktaki görevler bitmeli** (veri kaybı olmasın)
5. `submit()` fonksiyonu **thread-safe** olmalı (herhangi bir thread'den çağrılabilmeli)