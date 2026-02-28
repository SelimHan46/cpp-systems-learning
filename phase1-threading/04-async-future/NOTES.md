# Hafta 2, Gün 1-2: std::async ve std::future Notları

## std::async ve std::future Nedir?

Thread'den sonuç almanın **kolay yolu**. Thread + std::ref yerine async + future kullanırsın.

### Benzetme: Kargo Siparişi

- **std::async** = Kargo siparişi ver. "Bu işi yap, sonucu bana gönder."
- **std::future** = Kargo takip numarası. "Sonuç hazır olunca buradan alacağım."
- **future.get()** = Kargoyu teslim al. "Sonuç gelene kadar bekle, gelince ver."

```
Sen: "Hesaplamayı yap" (async)
Sistem: "Tamam, işte takip numaran" (future)
Sen: başka işler yapıyorsun...
Sen: "Sonucu ver" (future.get())
Sistem: "İşte sonuç: 42"
```

---

## Neden Lazım?

| Yöntem | Kod | Sonuç alma |
|--------|-----|------------|
| Thread + std::ref (eski) | `std::thread t(func, std::ref(sonuc));` | Karmaşık, ref ile |
| Async + future (yeni) | `auto f = std::async(func);` | Kolay, `f.get()` ile |

Async daha kısa, daha temiz, daha güvenli.

---

## Temel Kullanım

```cpp
#include <future>

// Sonuc donduren fonksiyon
int hesapla(int sayi) {
    return sayi * sayi;
}

// Async ile baslat
std::future<int> sonuc = std::async(std::launch::async, hesapla, 5);

// Baska isler yap...

// Sonucu al (hazir degilse bekler)
int deger = sonuc.get();  // deger = 25
```

---

## std::launch Modları

| Mod | Ne yapar |
|-----|----------|
| `std::launch::async` | Hemen yeni thread'de başlat |
| `std::launch::deferred` | `get()` çağrılana kadar başlatma (tembel) |
| Hiçbir şey yazmazsan | Sistem karar verir (async veya deferred) |

```cpp
// Hemen baslat (yeni thread)
auto f1 = std::async(std::launch::async, hesapla, 5);

// get() cagirilinca baslat (ayni thread)
auto f2 = std::async(std::launch::deferred, hesapla, 5);
```

**Kural:** Paralel çalışmasını istiyorsan her zaman `std::launch::async` yaz.

---

## Birden Fazla Async (Paralel Hesaplama)

```cpp
// 3 hesaplamayi AYNI ANDA baslat
auto f1 = std::async(std::launch::async, hesapla, 3);
auto f2 = std::async(std::launch::async, hesapla, 7);
auto f3 = std::async(std::launch::async, hesapla, 10);

// Sonuclari al
int s1 = f1.get();  // 9
int s2 = f2.get();  // 49
int s3 = f3.get();  // 100
```

Her biri 2 saniye sürüyorsa:
- Sırayla: 2 + 2 + 2 = **6 saniye**
- Paralel: hepsi aynı anda = **2 saniye**

---

## std::promise Nedir?

`async` otomatik çalışır. Ama bazen sonucu **elle** göndermek istersin. `promise` bunu sağlar.

- **promise** = "Söz veriyorum, sonucu göndereceğim"
- **future** = "Söz aldım, sonucu bekleyeceğim"

```cpp
std::promise<int> soz;
std::future<int> sonuc = soz.get_future();

// Baska thread'de:
soz.set_value(42);  // sonucu gonder

// Ana thread'de:
int deger = sonuc.get();  // 42
```

### async vs promise

| | async | promise |
|---|---|---|
| Kullanım | Otomatik, kolay | Elle kontrol |
| Thread | Otomatik oluşturur | Kendin oluşturursun |
| Ne zaman | Basit işler | Karmaşık kontrol lazımsa |

---

## std::packaged_task Nedir?

Bir fonksiyonu paketleyip sonra çalıştırmak istersen kullanılır. Thread pool'da kullanacağız.

```cpp
// Fonksiyonu paketle
std::packaged_task<int(int)> paket(hesapla);

// Future'ını al
std::future<int> sonuc = paket.get_future();

// Paketi calistir (istedigin zaman)
paket(5);

// Sonucu al
int deger = sonuc.get();  // 25
```

### async vs packaged_task

| | async | packaged_task |
|---|---|---|
| Ne zaman çalışır | Hemen | Sen çalıştırınca |
| Thread | Otomatik | Kendin oluşturursun |
| Kullanım | Basit paralel iş | Thread pool, görev kuyruğu |

---

## future.get() Kuralları

1. `get()` **sadece bir kere** çağrılabilir. İkinci çağrı hata verir.
2. `get()` sonuç hazır değilse **bekler** (blocking).
3. Fonksiyon exception fırlatırsa, `get()` aynı exception'ı fırlatır.

```cpp
auto f = std::async(std::launch::async, hesapla, 5);

int s = f.get();   // OK, sonuc geldi
int s2 = f.get();  // HATA! get() sadece bir kere cagrilabilir
```

Sonucun hazır olup olmadığını kontrol etmek için:

```cpp
// Beklemeden kontrol et
if (f.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
    int s = f.get();  // hazir, al
} else {
    // henuz hazir degil
}
```

---

## Özet Tablosu

| Kavram | Ne İşe Yarar |
|--------|-------------|
| **std::async** | Fonksiyonu arka planda çalıştır |
| **std::future** | Sonucu bekle ve al |
| **future.get()** | Sonucu teslim al (bir kere!) |
| **std::launch::async** | Hemen yeni thread'de başlat |
| **std::launch::deferred** | get() çağrılınca başlat |
| **std::promise** | Sonucu elle gönder |
| **std::packaged_task** | Fonksiyonu paketle, sonra çalıştır |

---

## Önemli Kurallar

1. Paralel çalışmasını istiyorsan `std::launch::async` yaz
2. `get()` sadece **bir kere** çağrılır
3. `get()` sonuç hazır değilse **bekler**
4. Basit işler için `async` kullan, karmaşık kontrol için `promise` kullan
5. Thread pool yazarken `packaged_task` kullanacağız