#include <iostream>
#include <string>

class Hayvan {
public:
    virtual void sesCikar() {
        std::cout << "Hayvan: ..." << std::endl;
    }
};

class Kedi : public Hayvan {
public:
    void sesCikar() override {
        std::cout << "Kedi: Miyav!" << std::endl;
    }
};

class Kopek : public Hayvan {
public:
    void sesCikar() override {
        std::cout << "Kopek: Hav hav!" << std::endl;
    }
};

int main() {
    std::cout << "=== NORMAL CAGIRMA ===" << std::endl;
    Kedi kedi;
    Kopek kopek;
    kedi.sesCikar();
    kopek.sesCikar();

    std::cout << "\n=== POINTER ILE CAGIRMA (VIRTUAL SAYESINDE DOGRU CALISIYOR) ===" << std::endl;
    Hayvan* h1 = &kedi;
    Hayvan* h2 = &kopek;
    h1->sesCikar();
    h2->sesCikar();

    return 0;
}