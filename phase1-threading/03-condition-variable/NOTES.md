# Gün 5-7: Condition Variable Notları

## Condition Variable Nedir?

Thread'ler arası **sinyal gönderme** mekanizması. Bir thread diğerine "hey, iş hazır, gel al" diyor.

### Benzetme: Restoranda Aşçı ve Garson

- Aşçı yemek hazırlıyor
- Garson bekliyor
- Yemek hazır olunca aşçı bağırıyor: **"HAZIR!"**
- Garson alıp masaya götürüyor

Garson sürekli "hazır mı? hazır mı?" diye sormak yerine, aşçının sinyal vermesini bekliyor. Daha verimli.

---

## Neden Lazım?

Mutex sadece "bir seferde bir thread girsin" diyor. Ama bazen thread'lerin **birbirini beklemesi** lazım.

| Durum | Mutex yeterli mi? | Condition Variable lazım mı? |
|-------|-------------------|------------------------------|
| Aynı değişkene erişim | ✅ Evet | Hayır |
| Bir thread diğerini bekliyor | ❌ Hayır | **✅ Evet** |
| Veri hazır olunca devam et | ❌ Hayır | **✅ Evet** |

---

## Temel Kullanım

```cpp
#include <condition_variable>

std::mutex kilit;
std::condition_variable sinyal;
bool veriHazir = false;

// Bekleyen thread (garson):
std::unique_lock<std::mutex> ulock(kilit);
sinyal.wait(ulock, []{ return veriHazir; });
// veriHazir true olana kadar uyuyor

// Sinyal gönderen thread (aşçı):
{
    std::lock_guard<std::mutex> guard(kilit);
    veriHazir = true;
}
sinyal.notify_one();  // bekleyeni uyandır
```

---

## wait() Nasıl Çalışıyor?

`sinyal.wait(ulock, []{ return veriHazir; });` şunu yapıyor:

| Adım | Ne oluyor |
|------|-----------|
| 1 | `veriHazir` kontrol et |
| 2 | `false` ise → kilidi bırak ve uyu |
| 3 | Sinyal gelince uyan |
| 4 | Kilidi tekrar al |
| 5 | `veriHazir` tekrar kontrol et |
| 6 | `true` ise → devam et, `false` ise → tekrar uyu |

### Neden unique_lock kullanılıyor?

`wait()` kilidi **bırakıp tekrar alması** lazım. `lock_guard` bunu yapamaz çünkü sadece scope bitince açılır. `unique_lock` esnek, istediğin zaman aç/kapa yapabilir.

| | lock_guard | unique_lock |
|---|---|---|
| Ortada aç/kapa | ❌ Yapamaz | ✅ Yapabilir |
| Condition variable ile | ❌ Kullanılamaz | ✅ Kullanılır |
| Ne zaman kullan | Basit mutex koruma | Condition variable ile |

---

## notify_one() vs notify_all()

| Komut | Ne yapar |
|-------|----------|
| `notify_one()` | Bekleyen thread'lerden **birini** uyandır |
| `notify_all()` | Bekleyen **tüm** thread'leri uyandır |

```cpp
sinyal.notify_one();   // 1 thread uyanır
sinyal.notify_all();   // hepsi uyanır
```

---

## Producer-Consumer Pattern

Condition variable'ın en yaygın kullanımı. Bir thread veri **üretir** (producer), diğer thread veri **tüketir** (consumer). Aralarında bir **queue** (kuyruk) var.

```
[Producer] → veri üret → queue'ya koy → sinyal gönder
                              ↓
[Consumer] ← sinyal al ← queue'dan al ← veriyi işle
```

### Benzetme: Fırın

- Fırıncı (producer) ekmek pişiriyor, rafa koyuyor
- Müşteri (consumer) raftan ekmek alıyor
- Raf boşsa müşteri bekliyor
- Fırıncı ekmek koyunca müşteriye haber veriyor

### Kod Yapısı

```cpp
std::queue<int> kuyruk;
std::mutex kilit;
std::condition_variable sinyal;

// Producer: veri üret, queue'ya koy
void producer() {
    int veri = 42;
    {
        std::lock_guard<std::mutex> guard(kilit);
        kuyruk.push(veri);
    }
    sinyal.notify_one();  // consumer'a haber ver
}

// Consumer: sinyal bekle, queue'dan al
void consumer() {
    std::unique_lock<std::mutex> ulock(kilit);
    sinyal.wait(ulock, []{ return !kuyruk.empty(); });
    int veri = kuyruk.front();
    kuyruk.pop();
}
```

---

## Özet Tablosu

| Kavram | Ne İşe Yarar |
|--------|-------------|
| **condition_variable** | Thread'ler arası sinyal gönderme |
| **wait()** | Sinyal gelene kadar bekle (uyur, CPU harcamaz) |
| **notify_one()** | Bekleyen 1 thread'i uyandır |
| **notify_all()** | Bekleyen tüm thread'leri uyandır |
| **unique_lock** | wait() ile kullanılması zorunlu (lock_guard olmaz) |
| **Producer-Consumer** | Bir üretir, diğeri tüketir (queue ile) |

---

## Önemli Kurallar

1. `wait()` ile **her zaman `unique_lock`** kullan, `lock_guard` olmaz
2. `wait()` içine **koşul fonksiyonu** yaz: `wait(ulock, []{ return kosul; })`
3. Koşul olmadan wait kullanma, **spurious wakeup** olabilir (yanlışlıkla uyanma)
4. `notify_one()` veya `notify_all()` çağrısını **kilit dışında** yapmak daha performanslı
5. Producer-Consumer pattern'de **queue doluluk kontrolü** unutma