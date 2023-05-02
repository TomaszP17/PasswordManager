//
// Created by Tomasz on 02.05.2023.
//

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <filesystem>
#include "PasswordManager.h"
#include "fmt/core.h"
#include <vector>
#include <set>

PasswordManager::PasswordManager() {}

/*PasswordManager::PasswordManager(std::string fileName) {
    this->fileName = fileName;
}*/

void PasswordManager::setFileName(std::string fileName) {
    this->fileName = fileName;
}

/**
* Funkcja wczytuje od uzytkownika nazwe pliku ktory chce otworzyc
* Zwraca nazwe tego pliku
* Funkcja moze tez przyjmowac absolutna sciezke jako nazwe pliku
*/
auto PasswordManager::enterFileName() -> std::string {
    //wyswietlenie zawartosci folderu w ktorym przetrzymywane sa zaszyfrowane pliki uzytkownika
    auto const dirPath = std::string("C:\\Users\\Tomasz\\Desktop\\MenadzerHasel");
    auto dirIterator = std::filesystem::recursive_directory_iterator(dirPath);
    std::cout << "Ponizej znajduje sie lista dostepnych plikow na ktorych mozesz operowac, wybierz index pliku lub podaj jego absolutna sciezke: \n";
    for(auto const& entry : dirIterator){
        std::cout << entry.path() << '\n';
    }
    std::cout << "\nPodaj nazwe pliku, ktory chcesz otworzyc: ";
    std::string nazwaPliku;
    std::cin >> nazwaPliku;
    return nazwaPliku;
}

/**
* Funkcja dostaje na wejscie wskaznik pliku i sprawdza czy taki plik istnieje w folderze programu
* W pozytywnym przypadku zwraca true w przeciwnym false
*/
auto PasswordManager::checkFileStatus(std::fstream *file) -> bool {
    if (file->good()) {
        return true;
    } else {
        return false;
    }
}

/**
 * Funkcja sprawdza czy plik o nazwie fileName istnieje w folderze programu
 * Zwraca true jesli plik istnieje, w przeciwnym przypadku false
 */
bool PasswordManager::fileExists() {
    std::fstream file(fileName);
    return checkFileStatus(&file);
}

/**
 * Funkcja tworzy plik o nazwie fileName w folderze programu
 * Zwraca true jesli plik zostal utworzony, w przeciwnym przypadku false
 */
bool PasswordManager::createFile() {
    std::fstream file(fileName);
    bool status = checkFileStatus(&file);
    file.close();
    return status;
}

/**
 * Funkcja zapisuje dane z wektora passwordList do pliku fileName
 * Zwraca true jesli dane zostaly zapisane, w przeciwnym przypadku false
 */
bool PasswordManager::saveToFile() {
    std::ofstream file(fileName);
    if (file.is_open()) {
        for (Password p: passwordList) {
            file << p.getName() << " " << p.getPassword() << " " << p.getCategory() << " " << p.getWebsite()
                 << " " << p.getLogin() << '\n';
        }
        file.close();
        return true;
    } else {
        std::cout << "\nNie udalo sie otworzyc pliku do zapisu.\n";
        return false;
    }
}

/**
 * Funkcja wczytuje dane z pliku fileName i zapisuje je do wektora passwordList
 * Zwraca true jesli dane zostaly wczytane, w przeciwnym przypadku false
 */
bool PasswordManager::loadFromFile() {
    std::ifstream file(fileName);
    if (file.is_open()) {
        passwordList.clear();
        std::string name, password, category, website, login;
        while (file >> name >> password >> category >> website >> login) {
            Password p(name, password, category, website, login);
            passwordList.push_back(p);
        }
        file.close();
        return true;
    } else {
        std::cout << "\nNie udalo sie otworzyc pliku do odczytu.\n";
        return false;
    }
}

/**
 * Funkcja dodaje nowe haslo do wektora passwordList
 */
void PasswordManager::addPassword() {
    std::string name, password, category, website, login;
    std::cout << "\nDodawanie nowego hasla\n";
    std::cout << "Podaj nazwe np. (Google): ";
    std::cin >> name;
    std::cout << "Podaj haslo: ";
    std::cin >> password;
    std::cout << "Podaj kategorie: ";
    std::cin >> category;
    std::cout << "Podaj strone: ";
    std::cin >> website;
    std::cout << "Podaj login: ";
    std::cin >> login;

    std::cout << "\n\nTwoja nazwa to: " << name << '\n';
    //tutaj trzeba poprawic aby sprawdzalo czy website i login nie sa
    Password p(name, password, category, website, login);
    passwordList.push_back(p);
    saveToFile();
}

/**
 * Funkcja usuwa haslo o podanym indeksie z wektora passwordList
 */
void PasswordManager::removePassword() {
    std::cout << "\nUsuwanie hasla\n";
    int index;
    std::cout << "Podaj indeks hasla do usuniecia: ";
    std::cin >> index;
    if (index < passwordList.size()) {
        passwordList.erase(passwordList.begin() + index);
        saveToFile();
    } else {
        std::cout << "\nNie ma hasla o takim indeksie.\n";
    }
}

/**
 * Funkcja wypisuje na ekranie wszystkie hasla znajdujace sie w wektorze passwordList
 */
void PasswordManager::printPasswords() {
    std::cout << "\nLista zapisanych hasel:\n";
    int i = 0;
    for (Password p: passwordList) {
        std::cout << i++ << ". " << p.getName() << " | " << p.getPassword() << " | " << p.getCategory()
                  << " | " << p.getWebsite() << " | " << p.getLogin() << '\n';
    }
}

/**
 * Funkcja umożliwia zmiane hasla o podanym indeksie w wektorze passwordList
 */
void PasswordManager::changePassword() {
    std::cout << "\nZmiana hasla\n";
    int index;
    std::cout << "Podaj indeks hasla do zmiany: ";
    std::cin >> index;
    if (index < passwordList.size()) {
        std::string newPassword;
        std::cout << "Podaj nowe haslo: ";
        std::cin >> newPassword;
        passwordList[index].setPassword(newPassword);
        saveToFile();
    } else {
        std::cout << "\nNie ma hasla o takim indeksie.\n";
    }
}
/**
 * Funkcja szuka podobnych wynikow wsrod hasel
 * Pyta o wszystkie mozliwe dane i jezeli ktores haslo zawiera podana m.in kategorie to wyswietli vector z tymi wynikami
 */
void PasswordManager::searchPassword(){
    std::cout << "Zaraz poprosze Cie o wypelnienie danych ktorych szukasz: \n";
    std::string name, password, category, website, login;
    std::cout << "Podaj nazwe np. (Google): ";
    std::cin >> name;
    std::cout << "Podaj haslo: ";
    std::cin >> password;
    std::cout << "Podaj kategorie: ";
    std::cin >> category;
    std::cout << "Podaj strone: ";
    std::cin >> website;
    std::cout << "Podaj login: ";
    std::cin >> login;
    auto results = std::vector<Password>();
    std::cout << "[OTO PODOBNE REZULTATY]\n";
    for(auto x : passwordList){
        if (x.getName() == name) {
            results.push_back(x);
        } else if (x.getPassword() == password) {
            results.push_back(x);
        } else if (x.getCategory() == category) {
            results.push_back(x);
        } else if (x.getWebsite() == website) {
            results.push_back(x);
        } else if (x.getLogin() == login) {
            results.push_back(x);
        }
    }
    for(auto x : results){
        std::cout << ". " << x.getName() << " | " << x.getPassword() << " | " << x.getCategory() << " | " << x.getWebsite() << " | " << x.getLogin() << '\n';
    }
}
/**
 * Funkcja sortuje po parametrach
 * NALEZY JAK NAJSZYBCIEJ JA EDYTOWAC
 */
void PasswordManager::sortPasswords(){
    std::cout << "Podaj parametry sortowania:\n";
    std::cout << "1. Name\n";
    std::cout << "2. Password\n";
    std::cout << "3. Category\n";
    std::cout << "4. Website\n";
    std::cout << "5. Login\n";

    std::vector<int> sortingParams;
    int param;
    do {
        std::cout << "Wybierz parametr (0 aby zakonczyc): ";
        std::cin >> param;
        if(param >= 1 && param <= 5){
            sortingParams.push_back(param);
        }
    } while (param != 0);

    std::stable_sort(passwordList.begin(), passwordList.end(), [&](const Password& p1, const Password& p2) {
        for(const auto& param : sortingParams){
            if(param == 1){
                if(p1.getName() != p2.getName()){
                    return p1.getName() < p2.getName();
                }
            } else if(param == 2){
                if(p1.getPassword() != p2.getPassword()){
                    return p1.getPassword() < p2.getPassword();
                }
            } else if(param == 3){
                if(p1.getCategory() != p2.getCategory()){
                    return p1.getCategory() < p2.getCategory();
                }
            } else if(param == 4){
                if(p1.getWebsite() != p2.getWebsite()){
                    return p1.getWebsite() < p2.getWebsite();
                }
            } else if(param == 5){
                if(p1.getLogin() != p2.getLogin()){
                    return p1.getLogin() < p2.getLogin();
                }
            }
        }
        return false;
    });

    std::cout << "[POSORTOWANE HASLA]\n";
    for(const auto& p : passwordList){
        std::cout << p.getName() << " | " << p.getPassword() << " | " << p.getCategory() << " | " << p.getWebsite() << " | " << p.getLogin() << '\n';
    }
}

void PasswordManager::menu() {
    int choice;
    while (true) {
        std::cout << "\n>>- - - - - - - - - - - - - - - - - - - - - - -<<\n";
        std::cout << "WYBIERZ DZIALANIE:\n";
        std::cout << "[1] Wyswietl hasla\n";
        std::cout << "[2] Dodaj nowe haslo\n";
        std::cout << "[3] Usun haslo\n";
        std::cout << "[4] Zmien haslo\n";
        std::cout << "[5] Zakoncz program\n";
        std::cout << "[6] Wyszukaj hasla*\n";
        std::cout << "[7] Posortuj hasla*\n";

        std::cout << "*[DONE]Dodaj haslo\n";
        std::cout << "*Edytuj haslo*\n";
        std::cout << "*Usun Haslo*\n";
        std::cout << "*Dodaj kategorie*";
        std::cout << "*Usun kategorie*\n";

        std::cout << ">>- - - - - - - - - - - - - - - - - - - - - - -<<\n";
        std::cout << "Wybor: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                printPasswords();
                break;
            case 2:
                addPassword();
                break;
            case 3:
                removePassword();
                break;
            case 4:
                changePassword();
                break;
            case 5:
                exit(0);
                break;
            case 6:
                searchPassword();
                break;
            case 7:
                sortPasswords();
                break;
            default:
                std::cout << "\nNieprawidlowy wybor.\n";
                break;
        }
    }
}

/**
 * Funkcja rozpoczyna dzialanie programu
 */
void PasswordManager::start() {
    std::cout << ">>- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - <<\n";
    //poznizsze 3 linijki odpowiedzialne sa za wyswietlanie aktualnej godziny w menu (mozna to potem przerobic na funkcje aby wstawic w inne miejsca)
    auto now = std::chrono::system_clock::now();
    std::time_t c_time = std::chrono::system_clock::to_time_t(now);
    std::cout << "Current time: " << std::ctime(&c_time);

    std::cout << "\nWitaj w Password Manager autorstwa Tomasza Plucinskiego\n";
    std::cout << "Aktualna wersja programu to: 0.1v\n";
    std::cout << "W przypadku jakichkolwiek problemow skontaktuj sie ze mna mailowo: papieskapomoc@gmail.com\n";
    std::cout << ">>- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - <<\n";
    std::string fileName;
    while (true) {
        fileName = enterFileName();
        setFileName(fileName);
        if (fileExists()) {
            if (loadFromFile()) {
                break;
            }
        } else {
            if (createFile()) {
                break;
            }
        }
    }
    menu();
}

/*class PasswordManager {
private:
    std::string fileName;
    std::vector<Password> passwordList;

public:
    PasswordManager(){
    };

    PasswordManager(std::string fileName) {
        this->fileName = fileName;
    }

    void setFileName(std::string fileName) {
        this->fileName = fileName;
    }

    */
/**
 * Funkcja wczytuje od uzytkownika nazwe pliku ktory chce otworzyc
 * Zwraca nazwe tego pliku

    auto enterFileName() -> std::string {
        std::cout << "\nPodaj nazwe pliku, ktory chcesz otworzyc: ";
        std::string nazwaPliku;
        std::cin >> nazwaPliku;
        return nazwaPliku;
    }

    *
 * Funkcja dostaje na wejscie wskaznik pliku i sprawdza czy taki plik istnieje w folderze programu
 * W pozytywnym przypadku zwraca true w przeciwnym false

    auto checkFileStatus(std::fstream *file) -> bool {
        if (file->good()) {
            return true;
        } else {
            return false;
        }
    }

    *
     * Funkcja sprawdza czy plik o nazwie fileName istnieje w folderze programu
     * Zwraca true jesli plik istnieje, w przeciwnym przypadku false

    bool fileExists() {
        std::fstream file(fileName);
        return checkFileStatus(&file);
    }

    *
     * Funkcja tworzy plik o nazwie fileName w folderze programu
     * Zwraca true jesli plik zostal utworzony, w przeciwnym przypadku false

    bool createFile() {
        std::fstream file(fileName);
        bool status = checkFileStatus(&file);
        file.close();
        return status;
    }

    *
     * Funkcja zapisuje dane z wektora passwordList do pliku fileName
     * Zwraca true jesli dane zostaly zapisane, w przeciwnym przypadku false

    bool saveToFile() {
        std::ofstream file(fileName);
        if (file.is_open()) {
            for (Password p: passwordList) {
                file << p.getName() << " " << p.getPassword() << " " << p.getCategory() << " " << p.getWebsite()
                     << " " << p.getLogin() << '\n';
            }
            file.close();
            return true;
        } else {
            std::cout << "\nNie udalo sie otworzyc pliku do zapisu.\n";
            return false;
        }
    }

    *
     * Funkcja wczytuje dane z pliku fileName i zapisuje je do wektora passwordList
     * Zwraca true jesli dane zostaly wczytane, w przeciwnym przypadku false

    bool loadFromFile() {
        std::ifstream file(fileName);
        if (file.is_open()) {
            passwordList.clear();
            std::string name, password, category, website, login;
            while (file >> name >> password >> category >> website >> login) {
                Password p(name, password, category, website, login);
                passwordList.push_back(p);
            }
            file.close();
            return true;
        } else {
            std::cout << "\nNie udalo sie otworzyc pliku do odczytu.\n";
            return false;
        }
    }

    *
     * Funkcja dodaje nowe haslo do wektora passwordList

    void addPassword() {
        std::string name, password, category, website, login;
        std::cout << "\nDodawanie nowego hasla\n";
        std::cout << "Podaj nazwe np. (Google): ";
        std::cin >> name;
        std::cout << "Podaj haslo: ";
        std::cin >> password;
        std::cout << "Podaj kategorie: ";
        std::cin >> category;
        std::cout << "Podaj strone: ";
        std::cin >> website;
        std::cout << "Podaj login: ";
        std::cin >> login;
        //tutaj trzeba poprawic aby sprawdzalo czy website i login nie sa
        Password p(name, password, category, website, login);
        passwordList.push_back(p);
        saveToFile();
    }

    *
     * Funkcja usuwa haslo o podanym indeksie z wektora passwordList

    void removePassword() {
        std::cout << "\nUsuwanie hasla\n";
        int index;
        std::cout << "Podaj indeks hasla do usuniecia: ";
        std::cin >> index;
        if (index < passwordList.size()) {
            passwordList.erase(passwordList.begin() + index);
            saveToFile();
        } else {
            std::cout << "\nNie ma hasla o takim indeksie.\n";
        }
    }

    *
     * Funkcja wypisuje na ekranie wszystkie hasla znajdujace sie w wektorze passwordList

    void printPasswords() {
        std::cout << "\nLista zapisanych hasel:\n";
        int i = 0;
        for (Password p: passwordList) {
            std::cout << i++ << ". " << p.getName() << " " << p.getPassword() << " " << p.getCategory()
                      << " " << p.getWebsite() << " " << p.getLogin() << '\n';
        }
    }

    *
     * Funkcja umożliwia zmiane hasla o podanym indeksie w wektorze passwordList

    void changePassword() {
        std::cout << "\nZmiana hasla\n";
        int index;
        std::cout << "Podaj indeks hasla do zmiany: ";
        std::cin >> index;
        if (index < passwordList.size()) {
            std::string newPassword;
            std::cout << "Podaj nowe haslo: ";
            std::cin >> newPassword;
            passwordList[index].setPassword(newPassword);
            saveToFile();
        } else {
            std::cout << "\nNie ma hasla o takim indeksie.\n";
        }
    }

    void menu() {
        int choice;
        while (true) {
            std::cout << "Wybierz opcje:\n";
            std::cout << "1. Wyswietl hasla\n";
            std::cout << "2. Dodaj nowe haslo\n";
            std::cout << "3. Usun haslo\n";
            std::cout << "4. Zmien haslo\n";
            std::cout << "5. Zakoncz program\n";
            std::cout << "Wybor: ";
            std::cin >> choice;
            switch (choice) {
                case 1:
                    printPasswords();
                    break;
                case 2:
                    addPassword();
                    break;
                case 3:
                    removePassword();
                    break;
                case 4:
                    changePassword();
                    break;
                case 5:
                    exit(0);
                    break;
                default:
                    std::cout << "\nNieprawidlowy wybor.\n";
                    break;
            }
        }
    }

    *
     * Funkcja rozpoczyna dzialanie programu

    void start() {
        std::cout << "\nWitaj w Password Manager!\n";
        std::string fileName;
        while (true) {
            fileName = enterFileName();
            setFileName(fileName);
            if (fileExists()) {
                if (loadFromFile()) {
                    break;
                }
            } else {
                if (createFile()) {
                    break;
                }
            }
        }
        menu();
    }
};*/
