# Gün 3-4: Mutex ve Senkronizasyon Notları

## Race Condition (Yarış Durumu) Nedir?

Birden fazla thread **aynı değişkene aynı anda** yazarsa sonuç **YANLIŞ** olur.
Buna "race condition" denir.

### Neden Oluyor?

`sayac++` komutu basit görünür ama aslında 3 adım:

| Adım | İşlem | Örnek |
|------|-------|-------|
| 1 | Sayacın değerini oku | 42 |
| 2 | 1 ekle | 43 |
| 3 | Geri yaz | sayac = 43 |

İki thread aynı anda yaparsa:

| Sıra | Thread-1 | Thread-2 | sayac |
|------|----------|----------|-------|
| 1 | oku → 42 | | 42 |
| 2 | | oku → 42 (aynı değeri okudu!) | 42 |
| 3 | yaz → 43 | | 43 |
| 4 | | yaz → 43 (44 yazması gerekirken!) | 43 |

**Bir artış KAYBOLDU!**

### Benzetme: Banka Hesabı

Hesapta 1000 TL var. İki kişi aynı anda 100 TL çekiyor:

**Doğru (sıralı):**

| Sıra | İşlem | Bakiye |
|------|-------|--------|
| 1 | Kişi-1: oku 1000, çek 100 | 900 |
| 2 | Kişi-2: oku 900, çek 100 | 800 ✅ |

**Race condition (aynı anda):**

| Sıra | İşlem | Bakiye |
|------|-------|--------|
| 1 | Kişi-1: oku 1000 | 1000 |
| 2 | Kişi-2: oku 1000 (aynı anda!) | 1000 |
| 3 | Kişi-1: çek 100, yaz 900 | 900 |
| 4 | Kişi-2: çek 100, yaz 900 | 900 ❌ |

100 TL havadan geldi! Sonuç 800 olmalıydı.

---

## Mutex Nedir?

**Mutex** = Mutual Exclusion = Karşılıklı Dışlama

Bir seferde **sadece 1 thread** erişsin diye kilit mekanizması.

### Benzetme: Tuvalet Kapısı

```
Thread-1 geldi → kapıyı KİLİTLE (lock) → işini yap → kapıyı AÇ (unlock)
                                                          ↓
Thread-2 bekliyordu → kapıyı KİLİTLE → işini yap → kapıyı AÇ
```

Aynı anda iki kişi giremez. Dışarıdakiler **bekler**.

### Temel Kullanım

```cpp
#include <mutex>

std::mutex kilit;      // kilit olustur
int sayac = 0;

void arttir() {
    kilit.lock();      // kapıyı kilitle (sadece ben girebilirim)
    sayac++;           // güvenle değiştir
    kilit.unlock();    // kapıyı aç (başkası girebilir)
}
```

---

## lock_guard Nedir?

`lock()` ve `unlock()` elle yazmak **tehlikeli**.
`unlock()` yazmayı unutursan program **KİTLENİR** (deadlock).

`lock_guard` otomatik: oluşturulunca kilitler, **scope bitince otomatik açar**.

### Kullanım

```cpp
#include <mutex>

std::mutex kilit;
int sayac = 0;

void arttir() {
    std::lock_guard<std::mutex> guard(kilit);  // otomatik kilitle
    sayac++;
    // scope bitince guard otomatik unlock yapar
    // unutma riski YOK
}
```

### lock() vs lock_guard Karşılaştırma

| Özellik | lock()/unlock() | lock_guard |
|---------|-----------------|------------|
| Kilitleme | Elle yaparsın | Otomatik |
| Açma | Elle yaparsın (unutabilirsin!) | Otomatik (scope bitince) |
| Deadlock riski | **VAR** | **YOK** |
| Kullanım | Basit durumlar | **Her yerde (önerilen)** |

---

## unique_lock Nedir?

`lock_guard` gibi ama **daha esnek**. İstediğin zaman kilitleyip açabilirsin.
Condition variable ile birlikte kullanılır (Gün 5-7'de göreceğiz).

### Kullanım

```cpp
std::mutex kilit;

void fonksiyon() {
    std::unique_lock<std::mutex> ulock(kilit);  // kilitle

    // bir seyler yap...

    ulock.unlock();    // elle açabilirsin
    // baska seyler yap (kilitsiz)
    ulock.lock();      // tekrar kilitleyebilirsin
}
```

### lock_guard vs unique_lock

| Özellik | lock_guard | unique_lock |
|---------|------------|-------------|
| Esneklik | Sadece scope boyunca | İstediğin zaman aç/kapat |
| Performans | Biraz daha hızlı | Biraz daha yavaş |
| Condition variable | Kullanamaz | **Kullanabilir** |
| Ne zaman kullan | Basit koruma | Esnek kontrol lazımsa |

---

## Deadlock Nedir?

İki thread **birbirini beklerse** ikisi de sonsuza kadar takılır. Buna **deadlock** denir.

### Benzetme: Dar Koridor

```
Kişi-1 →  ← Kişi-2
"Sen geç"   "Hayır sen geç"
İkisi de bekliyor, kimse geçemiyor → DEADLOCK
```

### Nasıl Oluyor?

```
Thread-1: kilit_A'yı aldım ✅ → kilit_B'yi bekliyorum ⏳
Thread-2: kilit_B'yi aldım ✅ → kilit_A'yı bekliyorum ⏳

Thread-1, B'yi bekliyor ama B Thread-2'de
Thread-2, A'yı bekliyor ama A Thread-1'de
→ İkisi de SONSUZA KADAR bekleyecek!
```

### Kod Örneği

```cpp
std::mutex kilit_A;
std::mutex kilit_B;

// Thread-1:
kilit_A.lock();    // A'yı aldım
kilit_B.lock();    // B'yi bekliyorum... (Thread-2'de!)

// Thread-2:
kilit_B.lock();    // B'yi aldım
kilit_A.lock();    // A'yı bekliyorum... (Thread-1'de!)

// DEADLOCK!
```

### Deadlock Nasıl Önlenir?

| Yöntem | Açıklama |
|--------|----------|
| Aynı sırayla kilitle | Tüm thread'ler önce A, sonra B kilitlesin |
| `std::lock()` kullan | Birden fazla mutex'i aynı anda kilitle |
| `lock_guard` kullan | Otomatik unlock, unutma riski yok |
| Kısa tut | Sadece gerekli yerde kilitle, hemen aç |

```cpp
// DOGRU: std::lock ile iki mutex'i ayni anda kilitle
std::lock(kilit_A, kilit_B);
std::lock_guard<std::mutex> guardA(kilit_A, std::adopt_lock);
std::lock_guard<std::mutex> guardB(kilit_B, std::adopt_lock);
```

---

## Özet Tablosu

| Kavram | Ne İşe Yarar |
|--------|-------------|
| **Race Condition** | Birden fazla thread aynı veriye yazınca sonuç yanlış olur |
| **Mutex** | Bir seferde 1 thread erişsin diye kilit mekanizması |
| **lock()/unlock()** | Elle kilitle/aç (tehlikeli, unutabilirsin) |
| **lock_guard** | Otomatik kilitle/aç (güvenli, önerilen) |
| **unique_lock** | Esnek kilitle/aç (condition variable için lazım) |
| **Deadlock** | İki thread birbirini beklerse ikisi de takılır |

---

## Önemli Kurallar

1. Paylaşılan değişkene erişirken **HER ZAMAN** mutex kullan
2. `lock_guard` kullan, `lock()/unlock()` kullanma
3. Kilitleme süresini olabildiğince **KISA** tut
4. Birden fazla mutex varsa **HEP AYNI SIRADA** kilitle
5. Race condition'ı görmek zor, **her çalıştırmada farklı sonuç** verir