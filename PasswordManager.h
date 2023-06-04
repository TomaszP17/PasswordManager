//
// Created by Tomasz on 02.05.2023.
//

#ifndef MENADZERHASEL_PASSWORDMANAGER_H
#define MENADZERHASEL_PASSWORDMANAGER_H

#include <vector>
#include "Password.h"
#include "Timestamp.h"

class PasswordManager {
private:
    std::vector<std::string> categoryList = {"Internet", "Wideo", "Edukacja", "Gry", "Muzyka", "Rozrywka", "Biznes"};
    std::string fileName;
    std::vector<Password> passwordList;
    std::string passwordToFile;
    int counterOfResult = 0;
    std::string lastTimeStamp;
public:
    template<typename T>
    void getInputFromUser(T& data);
    void getInputFromUser(std::string& data);
    /**
 * Funkcja przypisuje nazwe pliku do pola klasy PasswordManager filename
 * @param fileName - nazwa pliku
 */
    void setFileName(std::string fileName);
    /**
 * Funkcja przypisuje haslo do pliku do pola klasy PasswordManager passwordToFile
 * @param passwordToFile - haslo uzywane do szyfrowania oraz odszyfrowania
 */
    auto setPasswordToFile(std::string passwordToFile);
    /**
 * Funkcja przypisuje timestamp do pola klasy PasswordManager lastTimeStamp
 * @param timestamp - timestamp, ktory przypisujemy
 */
    auto setTimeStamp(std::string timestamp);
    /**
 * Funkcja wczytuje od uzytkownika haslo uzywane w szyfrowaniu zawartosci pliku
 * Po wczytaniu wykonuje przeksztalcenie podanego hasla na ciag znakow ascii
 * Nastepnie ten ciag znakow przeksztalca w odpowiedni sposob aby haslo bylo w miare unikatowe
 * i pomocne w szyfrowaniu oraz odszyfrowaniu danych
 * @return Zwraca haslo(string)
 */
    auto enterPasswordToFile() -> std::string;
    /**
* Funkcja wyswietla zawartosc foldera "PM files", ktory zawiera wszystkie pliki na ktorych operuje program
 * Nastepnie uzytkownik wybiera nazwe pliku jednego z wyswietlonych plikow lub pelna sciezke absolutna
 * Funkcja sama sprawdza czy podal nazwe czy sciezke
 * @returns nazwaPliku(string) - plik, ktory uzytkownik chce otworzyc
*/
    auto enterFileName() -> std::string;
    /**
* Funkcja sprawdza czy nie wystapil zaden error przy pliku
 * @param file - wskaznik do pliku
 * @return true jesli nie ma errorow
*/
    auto checkFileStatus(std::fstream *file) -> bool;
    /**
 * Funkcja sprawdza czy plik istnieje
 * @return - true jeœli istnieje
 */
    bool fileExists();
    /**
 * Funkcja szyfruje dane za pomoca hasla podanego przez uzytkownika
 * Szyfrowane sa linie w sposob przesuniec na tabeli ascii
 * Kazda nastepna linia jest szyfrowana w inny sposob niz poprzednia
 * Do szyfrowania potrzebne jest haslo podane wczesniej przez uzytkownika oraz zawartosc linii ktore szyfrujemy
 * @param name - nazwa rekordu
 * @param pass - haslo rekordu
 * @param category - kategoria rekordu
 * @param website - strona rekordu
 * @param login - login rekordu
 * @returns Zwraca zaszyfrowana linie
 */
    auto encryptData(const std::string& name, const std::string& pass, const std::string& category, const std::string& website, const std::string& login) -> std::string;
    /**
 * Funkcja rozszyfrowywuj¹ca dane - wykonuje odwrotn¹ operacjê do szyfrowania przy u¿yciu hasla od uzytkownika
 * @param line - Zaszyfrowana linia z pliku
 * @return zwraca string, ktory jest odszyfrowanym rekordem
 */
    auto decryptData(std::string line) -> std::string;
    /**
 * Funkcja zlicza ilosc linii w pliku
 * @param file - plik sprawdzany
 * @return - ilosc linii
 */
    int counterLines(std::ifstream& file);
    /**
 * Funkcja zapisuje dane do pliku w kolejnoœci: zawartosc passwordList zaszyfrowane, timestamp
 * @return true - jesli udalo sie otworzyc plik do zapisu danych
 */
    bool saveToFile();
    /**
 * Funkcja tworzy nowego timestampa z aktualnym czasem
 * @return zwraca go w postaci stringa
 */
    std::string newTimeStamp();
    /**
 * Funkcja wczytuje dane z pliku fileName i zapisuje je do wektora passwordList
 * Podczas odczytywania z pliku aktualizowany jest nowy timestamp
 * @return - true jesli powiedzie sie otwieranie pliku
 */
    bool loadFromFile();
    /**
 * Funkcja sprawdza czy haslo podane przez uzytkownika jest bezpieczne
 * Minimalna dlugosc = 10 i min 1 mala litera duza litera cyfra i znak specjalny
 * @param - sprawdzane haslo
 */
    void isPasswordSecure(const std::string& password);
    /**
 * Funkcja sprawdza czy haslo spelnia wymagania uzytkownika z funkcji generatePassword()
 * @param chars - sprawdzane haslo
 * @param small - male litery (true/false)
 * @param big - duze litery (true/false)
 * @param dig - cyfry (true/false)
 * @param spec - znaki specjalne (true/false)
 * @return - zwraca wartosc logiczna czy haslo spelnia wymagania od uzytkownika
 */
    bool isPasswordGood(std::string const& chars, bool small, bool big, bool dig, bool spec);
    /**
 * Funkcja sprawdza czy przekazany string zawiera conajmniej 1 duza litere
 * @param password - sprawdzana fraza
 * @return true - jesli zawiera min 1 duza litere
 */
    bool hasUpperCase(const std::string& password);
    /**
 * Funkcja sprawdza czy przekazany string zawiera conajmniej 1 mala litere
 * @param password - sprawdzana fraza
 * @return true - jesli zawiera min 1 mala litere
 */
    bool hasLowerCase(const std::string& password);
    /**
 * Funkcja sprawdza czy przekazany string zawiera conajmniej 1 cyfre
 * @param password - sprawdzana fraza
 * @return true - jesli zawiera min 1 cyfre
 */
    bool hasDigit(const std::string& password);
    /**
 * Funkcja sprawdza czy przekazany string zawiera conajmniej 1 znak specjalny
 * @param password - sprawdzana fraza
 * @return true - jesli zawiera min 1 znak specjalny
 */
    bool hasSpecialChar(const std::string& password);
    /**
 * Sprawdza czy haslo istnieje w rekordach
 * @param password - haslo sprawdzane
 * @return - zwraca wartosc logiczna czy haslo istnieje
 */
    bool isPasswordExists(const std::string& password);
    /**
 * Funkcja generuje haslo, dajac elastycznosc uzytkownikowi na wybranie specyfikacji hasla
 * @return - zwraca wygenerowane haslo
 */
    std::string generatePassword();
    /**
 * Funkcja dodaje nowe haslo do wektora passwordList
 * Pozwala na dodanie dowolnego hasla od uzytkownika jednak informuje o potencjalnych brakach w bezpieczenstwie
 * Pozwala rowniez na generowanie hasel ze sprecyzowanymi specyfikacjami od uzytkownika
 * Ostatecznie dodaje rekord do passwordList
 */
    void addPassword();
    /**
 * Funkcja usuwa haslo o podanym indeksie z wektora passwordList
 *
 * Usuñ has³o – usuwa wybrane has³o lub has³a. Przed ka¿dym usuniêciem informuje uzytkownika o nieodwracalnych skutkach
 */
    void removePassword();
    /**
 * Funkcja wypisuje na ekranie wszystkie hasla znajdujace sie w wektorze passwordList
 * Nastepnie wyswietla czas ostatniej modyfikacji programu
 */
    void printPasswords();
    /**
 * Funkcja umo¿liwia zmiane hasla o podanym indeksie w wektorze passwordList
 */
    void changePassword();
/**
 * Funkcja wyswietla zawartosc parametrow z ktorych sklada sie rekord
 */
    auto printParameters()->void;
    /**
 * Funkcja szuka podobnych wynikow wsrod rekordow. Pyta uzytkownika o parametry w jakich szuka podanych przez niego fraz
 * i rekordy w ktorych podane frazy wystepuja
 */
    void searchPassword();
    /**
 * Funkcja sortuje po parametrach vector passwordList. Wykorzystuje stablesort
 */
    void sortPasswords();
    /**
 * Funkcja wyswietla zawartosc vectora categoryList
 */
    void printCategory();
/**
 * Funkcja dodaje do categoryList nowe kategorie
 */
    void addCategory();
    /**
 * Funkcja usuwa z categoryList kategorie oraz wszystkie rekordy ktore zawieraja podana kategorie
 */
    void deleteCategory();
    /**
 * Funkcja wyœwietla menu oraz pobiera od uzytkownika wybor jednej z dzialalnosci programu
 */
    void menu();
    /**
 * Funkcja rozpoczyna dzialanie programu
 */
    void start();
};
#endif //MENADZERHASEL_PASSWORDMANAGER_H
