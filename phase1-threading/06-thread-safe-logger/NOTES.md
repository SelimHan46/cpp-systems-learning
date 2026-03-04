# Pekiştirme Projesi: Thread-Safe Logger

## Bu Proje Nedir?

Birden fazla thread aynı anda log yazmak istiyor. Ama aynı anda dosyaya yazarlarsa satırlar karışır (race condition). Bu proje bunu çözüyor.

**Sektörde neden önemli:** Her yazılımda log sistemi var. Birden fazla thread çalışan her programda log'un thread-safe olması şart. Mülakatta "thread-safe bir yapı yazdın mı?" sorusuna somut cevap.

---

## Mimari

```
Thread-1 ─┐
Thread-2 ──┤──→ log() ──→ [Kuyruk] ──→ [Yazıcı Thread] ──→ dosya.log
Thread-3 ──┤               mutex ile      condition variable
Thread-4 ──┤               korunuyor      ile uyandırılıyor
Thread-5 ─┘
```

Birden fazla thread mesaj üretiyor (producer), tek bir yazıcı thread kuyruktan alıp dosyaya yazıyor (consumer).

---

## Dosya Yapısı

```
06-thread-safe-logger/
├── src/
│   ├── log_mesaji.hpp    → Log mesajı struct'ı
│   └── logger.hpp        → Logger sınıfı
├── main.cpp              → Test dosyası
├── uygulama.log          → Çıktı (program oluşturur)
└── NOTES.md              → Bu dosya
```

---

## Kullanılan Kavramlar

| Kavram | Projede Nerede? |
|--------|----------------|
| **struct** | LogMesaji — 4 özelliği bir arada tutuyor (zaman, seviye, mesaj, threadId) |
| **std::queue** | Mesaj kuyruğu — thread'ler mesajı buraya koyuyor |
| **std::mutex** | Kuyruğu koruyor — aynı anda iki thread dokunmasın |
| **std::condition_variable** | "Yeni mesaj var!" sinyali — yazıcı thread'i uyandırıyor |
| **Producer-Consumer** | log() = producer (mesaj üret), yaziciDongusu() = consumer (dosyaya yaz) |
| **std::thread** | 5 iş thread'i + 1 yazıcı thread |
| **std::ref** | Thread'ler aynı logger'ı paylaşıyor |

---

## Düşünce Süreci (Nasıl Tasarladık?)

### 1. Log mesajında ne olmalı?
Bir log satırı: `[14:30:05] [INFO] [Thread-1] Dosya açıldı`
4 bilgi var → birden fazla özellik → **struct** kullan

### 2. Mesajları nerede biriktireceğiz?
Birden fazla thread mesaj üretiyor → sırayla işlenmeli → **queue** kullan

### 3. Kuyruğa aynı anda iki thread yazarsa?
Race condition olur → **mutex** ile koru

### 4. Kuyruktaki mesajları kim dosyaya yazacak?
Arka planda çalışan bir **thread** (yazıcı thread)

### 5. Yazıcı thread kuyruk boşken ne yapacak?
Sürekli kontrol etse CPU yanar → **condition_variable** ile uyusun, mesaj gelince uyansın

---

## Akış Detayı

### Mesaj yazma (Producer tarafı):

```
1. Thread logger.info("basladi") çağırır
2. LogMesaji struct'ı oluşur (zaman ve threadId otomatik)
3. mutex kilitleniyor
4. Mesaj kuyruğa ekleniyor
5. mutex açılıyor
6. sinyal.notify_one() → yazıcı thread'e "mesaj var!" haberi
```

### Dosyaya yazma (Consumer tarafı):

```
1. Yazıcı thread uyuyor (wait)
2. Sinyal geldi → uyanıyor
3. mutex kilitleniyor
4. Kuyruktaki TÜM mesajları alıyor
5. Her mesajı dosyaya yazıyor
6. mutex açılıyor
7. Başa dönüyor, tekrar uyuyor
```

### Kapanış:

```
1. durdur = true yapılıyor
2. Yazıcı thread uyandırılıyor (notify_one)
3. Kuyruktaki kalan mesajlar yazılıyor
4. Yazıcı thread çıkıyor (return)
5. join() ile bekleniyor
6. Dosya kapatılıyor
```

---

## Örnek Çıktı

```
[14:30:05] [INFO] [Thread-1001] Program basladi
[14:30:05] [INFO] [Thread-1002] Thread-1 basladi
[14:30:05] [INFO] [Thread-1003] Thread-2 basladi
[14:30:05] [INFO] [Thread-1004] Thread-3 basladi
[14:30:05] [WARNING] [Thread-1003] Thread-2 yavas calisiyor
[14:30:06] [INFO] [Thread-1002] Thread-1 bitti
[14:30:06] [INFO] [Thread-1004] Thread-3 bitti
```

Tüm satırlar düzgün, hiçbiri karışmamış. Çünkü mutex kuyruğu koruyor.

---

## Öğrenilen Dersler

1. **Producer-Consumer pattern** gerçek dünyada en çok kullanılan thread pattern'i
2. **Struct** birden fazla veriyi bir arada tutmak için ideal
3. **Background thread** ağır işleri (dosya yazma) ana thread'i yavaşlatmadan yapıyor
4. **Graceful shutdown** önemli — kapanırken kuyruktaki mesajlar kaybolmamalı