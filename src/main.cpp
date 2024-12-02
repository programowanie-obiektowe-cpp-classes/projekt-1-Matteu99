#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>


constexpr int CI = 100; 
constexpr int CMag = 50; 
constexpr int CMkt = 10; 
constexpr int CR = 20; 
constexpr int N = 3; 
constexpr int MAX_CREDIT_TERM = 24; 
constexpr float MAX_DEBT_RATIO = 3.0; 
constexpr double ODSETKI_KREDYT = 0.05; 


class Pracownik {
public:
    Pracownik(std::string imie, double wynagrodzenie) : imie(imie), wynagrodzenie(wynagrodzenie) {}
    virtual ~Pracownik() = default;

    virtual void wyswietlInformacje() const = 0;
    double getWynagrodzenie() const { return wynagrodzenie; }

protected:
    std::string imie;
    double wynagrodzenie;
};


class Inzynier : public Pracownik {
public:
    Inzynier(std::string imie, std::string wydzial) : Pracownik(imie, 5000), wydzial(wydzial) {}

    void wyswietlInformacje() const override {
        std::cout << "Inzynier: " << imie << ", Wynagrodzenie: " << wynagrodzenie << ", Wydzial: " << wydzial << "\n";
    }

private:
    std::string wydzial;
};


class Magazynier : public Pracownik {
public:
    Magazynier(std::string imie, bool uprawnieniaNaWozek) : Pracownik(imie, 3000), uprawnieniaNaWozek(uprawnieniaNaWozek) {}

    void wyswietlInformacje() const override {
        std::cout << "Magazynier: " << imie << ", Wynagrodzenie: " << wynagrodzenie << ", Uprawnienia na wozek: " << (uprawnieniaNaWozek ? "Tak" : "Nie") << "\n";
    }

private:
    bool uprawnieniaNaWozek;
};


class Marketer : public Pracownik {
public:
    Marketer(std::string imie, int liczbaObserwujacych) : Pracownik(imie, 4000), liczbaObserwujacych(liczbaObserwujacych) {}

    void wyswietlInformacje() const override {
        std::cout << "Marketer: " << imie << ", Wynagrodzenie: " << wynagrodzenie << ", Obserwujacy: " << liczbaObserwujacych << "\n";
    }

private:
    int liczbaObserwujacych;
};


class Robotnik : public Pracownik {
public:
    Robotnik(std::string imie, float rozmiarButa) : Pracownik(imie, 3500), rozmiarButa(rozmiarButa) {}

    void wyswietlInformacje() const override {
        std::cout << "Robotnik: " << imie << ", Wynagrodzenie: " << wynagrodzenie << ", Rozmiar buta: " << rozmiarButa << "\n";
    }

private:
    float rozmiarButa;
};


class Kredyt {
public:
    Kredyt(double kwota, int okresSplaty, double odsetki) : kwota(kwota), okresSplaty(okresSplaty), odsetki(odsetki), pozostalaKwota(kwota) {}

    double rataMiesieczna() const {
        return (kwota * (1 + odsetki)) / okresSplaty;
    }

    void splacRate() {
        pozostalaKwota -= rataMiesieczna();
    }

    double getPozostalaKwota() const {
        return pozostalaKwota;
    }

    bool jestSplacony() const {
        return pozostalaKwota <= 0;
    }

private:
    double kwota;
    int okresSplaty;
    double odsetki;
    double pozostalaKwota;
};


class Firma {
public:
    Firma() : stanKonta(10000), wartoscSpolki(0) {
        pracownicy.push_back(std::make_unique<Inzynier>("Jan", "Mechanika"));
        pracownicy.push_back(std::make_unique<Magazynier>("Anna", true));
        pracownicy.push_back(std::make_unique<Marketer>("Piotr", 1500));
        pracownicy.push_back(std::make_unique<Robotnik>("Kasia", 42.5));
    }

    void wylistujPracownikow() const {
        for (const auto& pracownik : pracownicy) {
            pracownik->wyswietlInformacje();
        }
    }

    void zatrudnijInzyniera(const std::string& imie, const std::string& wydzial) {
        pracownicy.push_back(std::make_unique<Inzynier>(imie, wydzial));
    }

    void zatrudnijMagazyniera(const std::string& imie, bool uprawnieniaNaWozek) {
        pracownicy.push_back(std::make_unique<Magazynier>(imie, uprawnieniaNaWozek));
    }

    void zatrudnijMarketera(const std::string& imie, int liczbaObserwujacych) {
        pracownicy.push_back(std::make_unique<Marketer>(imie, liczbaObserwujacych));
    }

    void zatrudnijRobotnika(const std::string& imie, float rozmiarButa) {
        pracownicy.push_back(std::make_unique<Robotnik>(imie, rozmiarButa));
    }

    void wezKredyt(double kwota, int okresSplaty) {
        if (okresSplaty > MAX_CREDIT_TERM) {
            std::cout << "Nie mozna wziac kredytu na dluzej niz " << MAX_CREDIT_TERM << " miesiecy.\n";
            return;
        }

        double maksZadluzenie = wartoscSpolki * MAX_DEBT_RATIO;
        double obecneZadluzenie = obliczZadluzenie();

        if (obecneZadluzenie + kwota > maksZadluzenie) {
            std::cout << "Nie mozna wziac kredytu, przekroczono maksymalny poziom zadluzenia.\n";
            return;
        }

        double odsetki = ODSETKI_KREDYT * (1 + (static_cast<double>(okresSplaty) / MAX_CREDIT_TERM));
        kredyty.push_back(Kredyt(kwota, okresSplaty, odsetki));
        stanKonta += kwota;
    }

    void zakonczTure() {
        
        int liczbaInzynierow = liczInzynierow();
        int liczbaMagazynierow = liczMagazynierow();
        int liczbaMarketerow = liczMarketerow();
        int liczbaRobotnikow = liczRobotnikow();

        int pojemnoscMagazynu = liczbaMagazynierow * CMag;
        int teoretycznieWyprodukowane = liczbaRobotnikow * CR;
        int faktycznieWyprodukowane = std::min(pojemnoscMagazynu, teoretycznieWyprodukowane);
        int popyt = liczbaMarketerow * CMkt;
        int sprzedane = std::min(popyt, faktycznieWyprodukowane);
        int przychod = sprzedane * (liczbaInzynierow * CI);

        double wynagrodzenie = obliczWynagrodzenia();
        double ratyKredytow = obliczRateKredytow();
        dochod = przychod - wynagrodzenie - ratyKredytow;
        stanKonta += dochod;

        splacKredyty();

        std::cout << "Stan konta: " << stanKonta << ", Dochod: " << dochod << "\n";

        if (stanKonta < 0) {
            std::cout << "Firma zbankrutowala!\n";
            exit(0);
        }
    }

private:
    double stanKonta;
    double wartoscSpolki;
    double dochod;
    std::vector<std::unique_ptr<Pracownik>> pracownicy;
    std::vector<Kredyt> kredyty;


    int liczInzynierow() const {
        int liczba = 0;
        for (const auto& pracownik : pracownicy) {
            if (dynamic_cast<Inzynier*>(pracownik.get()) != nullptr) {
                liczba++;
            }
        }
        return liczba;
    }

    int liczMagazynierow() const {
        int liczba = 0;
        for (const auto& pracownik : pracownicy) {
            if (dynamic_cast<Magazynier*>(pracownik.get()) != nullptr) {
                liczba++;
            }
        }
        return liczba;
    }

    int liczMarketerow() const {
        int liczba = 0;
        for (const auto& pracownik : pracownicy) {
            if (dynamic_cast<Marketer*>(pracownik.get()) != nullptr) {
                liczba++;
            }
        }
        return liczba;
    }

    int liczRobotnikow() const {
        int liczba = 0;
        for (const auto& pracownik : pracownicy) {
            if (dynamic_cast<Robotnik*>(pracownik.get()) != nullptr) {
                liczba++;
            }
        }
        return liczba;
    }

    double obliczWynagrodzenia() const {
        double suma = 0;
        for (const auto& pracownik : pracownicy) {
            suma += pracownik->getWynagrodzenie();
        }
        return suma;
    }

    double obliczRateKredytow() const {
        double suma = 0;
        for (const auto& kredyt : kredyty) {
            suma += kredyt.rataMiesieczna();
        }
        return suma;
    }

    double obliczZadluzenie() const {
        double suma = 0;
        for (const auto& kredyt : kredyty) {
            suma += kredyt.getPozostalaKwota();
        }
        return suma;
    }

    void splacKredyty() {
        for (auto& kredyt : kredyty) {
            kredyt.splacRate();
        }
        kredyty.erase(std::remove_if(kredyty.begin(), kredyty.end(), [](const Kredyt& kredyt) {
            return kredyt.jestSplacony();
        }), kredyty.end());
    }
};

int main() {
    Firma firma;
    std::string komenda;

    while (true) {
        std::cout << "Dostepne komendy: lp (listuj pracownikow), zinz (zatrudnij inzyniera), zmag (zatrudnij magazyniera), zmkt (zatrudnij marketera), zrob (zatrudnij robotnika), kred (wez kredyt), kt (zakoncz ture)\n";
        std::cout << "Podaj komende: ";
        std::cin >> komenda;

        if (komenda == "lp") {
            firma.wylistujPracownikow();
        } else if (komenda == "zinz") {
            std::string imie, wydzial;
            std::cout << "Podaj imie inzyniera: ";
            std::cin >> imie;
            std::cout << "Podaj wydzial: ";
            std::cin >> wydzial;
            firma.zatrudnijInzyniera(imie, wydzial);
        } else if (komenda == "zmag") {
            std::string imie;
            bool uprawnienia;
            std::cout << "Podaj imie magazyniera: ";
            std::cin >> imie;
            std::cout << "Czy posiada uprawnienia na wozek (1 - tak, 0 - nie): ";
            std::cin >> uprawnienia;
            firma.zatrudnijMagazyniera(imie, uprawnienia);
        } else if (komenda == "zmkt") {
            std::string imie;
            int obserwujacy;
            std::cout << "Podaj imie marketera: ";
            std::cin >> imie;
            std::cout << "Podaj liczbe obserwujacych: ";
           
           while (!(std::cin >> obserwujacy)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Niepoprawna wartosc. Podaj liczbe obserwujacych: ";
            }  

            firma.zatrudnijMarketera(imie, obserwujacy);
        } else if (komenda == "zrob") {
            std::string imie;
            float rozmiarButa;
            std::cout << "Podaj imie robotnika: ";
            std::cin >> imie;
            std::cout << "Podaj rozmiar buta: ";
            std::cin >> rozmiarButa;
            firma.zatrudnijRobotnika(imie, rozmiarButa);
        } else if (komenda == "kred") {
            double kwota;
            int okres;
            std::cout << "Podaj kwote kredytu: ";
            std::cin >> kwota;
            std::cout << "Podaj okres splaty (w miesiacach): ";
            std::cin >> okres;
            firma.wezKredyt(kwota, okres);
        } else if (komenda == "kt") {
            firma.zakonczTure();
        } else {
            std::cout << "Nieznana komenda!\n";
        }
    }

    return 0;
}