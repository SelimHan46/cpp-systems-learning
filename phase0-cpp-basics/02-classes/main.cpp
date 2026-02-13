#include <iostream>
#include <string>

class Player {
private:
    std::string name;
    int health;

public:
    Player(std::string playerName, int playerHealth) {
        name = playerName;
        health = playerHealth;
        std::cout << "[+] " << name << " olusturuldu! (Can: " << health << ")" << std::endl;
    }

    ~Player() {
        std::cout << "[-] " << name << " silindi." << std::endl;
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
        std::cout << "  " << name << " --> -" << damage << " hasar | Kalan can: " << health << std::endl;
    }

    void heal(int amount) {
        health += amount;
        std::cout << "  " << name << " --> +" << amount << " can   | Toplam can: " << health << std::endl;
    }

    void printInfo() {
        std::cout << "  " << name << " | Can: " << health << std::endl;
    }
};

int main() {
    std::cout << "=============================" << std::endl;
    std::cout << "     OYUNCU OLUSTURMA" << std::endl;
    std::cout << "=============================" << std::endl;

    Player p1("Selim", 100);
    Player p2("Boss", 200);

    std::cout << "\n=============================" << std::endl;
    std::cout << "        SAVAS BASLADI" << std::endl;
    std::cout << "=============================" << std::endl;

    p1.takeDamage(30);
    p2.takeDamage(50);
    p1.heal(20);

    std::cout << "\n=============================" << std::endl;
    std::cout << "          SONUC" << std::endl;
    std::cout << "=============================" << std::endl;

    p1.printInfo();
    p2.printInfo();

    std::cout << "\n=============================" << std::endl;
    std::cout << "       PROGRAM BITTI" << std::endl;
    std::cout << "=============================" << std::endl;

    return 0;
}