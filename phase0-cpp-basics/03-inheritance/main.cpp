#include <iostream>
#include <string>

//Base class
class Player {
    protected: 
        std::string name;
        int health;
        int damage;

    public:
        Player(std::string n, int h, int d) {
            name = n;
            health = h;
            damage = d;
        }

        void receiveDamage(int dmg) {
            health -= dmg;
            if (health < 0) {
                health = 0;
            }
        }

        virtual void attack(Player& target) {
            std::cout << " " << name << " --> " << target.name << " | " << damage << " hasar" << std::endl; 
            target.receiveDamage(damage);
        }

        void printInfo() {
            std::cout << " " << name << " | Can: " << health << " | Hasar: " << damage << std::endl;
        }

        bool isAlive() {
            return health > 0;
        }

        std::string getName() {
            return name;
        }
};

class Warrior : public Player {
    private:
        int armor;

    public:
        Warrior(std::string n, int h, int d, int a)
            : Player(n, h, d) { // once Player constructor cagrilir
            armor = a;
            std::cout << "[+] Savasci " << name << " olusturuldu! (Zirh: " << armor << ")" << std::endl;
            }

        // attack fonksiyonunu override et (farkli davransin)    
        void attack(Player& target) override {
             int totalDamage = damage + armor / 2;
             std::cout << " " << name << " --> " << target.getName() << " | " << totalDamage << " hasar (bonus zirhtan)" << std::endl; 
             target.receiveDamage(totalDamage);
        }
};

// Mage class'i Player'dan turetildi
class Mage : public Player {
    private:
        int mana;

    public:
        Mage(std::string n, int h, int d, int m)
            : Player(n, h, d) {
                mana = m;
                std::cout << "[+] Buyucu " << name << " olusturuldu! (Mana: " << mana << ")" << std::endl;
            }

        void attack(Player& target) override {
            if (mana >= 20) {
                int spellDamage = damage *2; // buyu ile 2x hasar
                mana -= 20;
                std::cout << " " <<  name << " --> " << target.getName() << " | " << spellDamage << " hasar (buyu!) | Kalan mana: " << mana << std::endl;
                target.receiveDamage(spellDamage);
            }
            else {
                std::cout << " " << name << " --> " << target.getName() << " | " << damage << " hasar (mana yok, normal saldiri)" << std::endl;
                target.receiveDamage(damage);
            }
          
        }
};


int main() {
    std::cout << "=======================" << std::endl;
    std::cout << "      KARAKTERLER" << std::endl;
    std::cout << "=======================" << std::endl;

    Warrior Savasci("Selim", 150, 25,40);
    Mage buyucu("Merlin", 80, 30, 100);

    std::cout << "\n======================" << std::endl;
    std::cout << "     BASLANGIC DURUMU" << std::endl;
    std::cout << "\n======================" << std::endl;
    Savasci.printInfo();
    buyucu.printInfo();

    std::cout << "\n=====================" << std::endl;
    std::cout << "          SAVAS!" << std::endl;
    std::cout << "\n=====================" << std::endl;

    // Round 1 
    std::cout << "\n-- Round 1 --" << std::endl;
    Savasci.attack(buyucu);
    buyucu.attack(Savasci);

    // Round 2
    std::cout << "\n-- Round 2 --" << std::endl;
    Savasci.attack(buyucu);
    buyucu.attack(Savasci);

    // Round 3
    std::cout << "\n-- Round 3 --" << std::endl;
    Savasci.attack(buyucu);
    buyucu.attack(Savasci);

    std::cout << "\n======================" << std::endl;
    std::cout << "           SONUC" << std::endl;
    std::cout << "========================" << std::endl;
    Savasci.printInfo();
    buyucu.printInfo();

    if (Savasci.isAlive() && !buyucu.isAlive()) {
        std::cout << "\n" << Savasci.getName() << " kazandi!" << std::endl;
    }
    else if (buyucu.isAlive() && !Savasci.isAlive()) {
        std::cout << "\n" << buyucu.getName() << " kazandi!" << std::endl;
    }
    else {
        std::cout << "\n Ikisi de hayatta!" << std::endl;
    }


    return 0;
}