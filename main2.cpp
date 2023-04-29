#include <iostream>
#include <fstream>
#include <string>
#include "fmt/core.h"
/**
 * Struktura Password reprezentuje haslo ktore bedzie przetrzymywane w pliku
 * Zawiera wszystkie informacje potrzebne do uzytecznosci programu
 * Wszystkie zmienne struktury sa publiczne
 */
struct Password {
    std::string name;
    std::string password;
    std::string category;
    std::string website;
    std::string login;
    std::string timestamp;
};
/**
 * Funkcja wyswietla powitalna wiadomosc na start informujac o wersji programu oraz o autorze
 * Cel: kosmetyka programu
 */
auto printWelcomeMessage()->void {
    std::cout << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =\n";
    std::cout << "Witaj w menadzerze hasel stworzonym przez Tomasza Plucinskiego\n";
    std::cout << "Program stosuje autorskie szyfrowanie zawartosci pliku: hasla.txt\n";
    std::cout<< '\n';
    std::cout << "Jest to wersja beta v0.1\n";
    std::cout << "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =\n";
}


/**
 * Funkcja wczytuje od uzytkownika nazwe pliku ktory chce otworzyc
 * Zwraca nazwe tego pliku
 */
auto enterFileName() -> std::string {
    std::cout << "\nPodaj nazwe pliku, ktory chcesz otworzyc: ";
    std::string nazwaPliku;
    std::cin >> nazwaPliku;
    return nazwaPliku;
}
/**
 * Funkcja dostaje na wejscie wskaznik pliku i sprawdza czy taki plik istnieje w folderze programu
 * W pozytywnym przypadku zwraca true w przeciwnym false
 */
auto checkFileStatus(std::fstream* file) -> bool {
    if(file->good()){
        return true;
    }else{
        return false;
    }
}
/**
 * Funkcja sprawdza czy plik istnieje w folderze programu o podanej przez uzytkownika nazwie
 * Korzysta z dwoch innych funkcji: enterFileName() i checkFileStatus()
 * Wyswietla na konsoli pozytywna informacje jezeli uzytkownik poda prawidlowa nazwe pliku z haslami
 * Lub negatywna do momentu az uzytkownik bedzie wpisywal zle nazwy
 */
auto processOfCheckingFileCorrect() -> void {
    bool correctNameOfFile = false;
    while(!correctNameOfFile) {
        auto file = std::fstream("C:\\Users\\Tomasz\\Desktop\\MenadzerHasel\\" + enterFileName()); //wczytuje plik
        //sprawdza czy plik istnieje i nie sypie bledem
        if (checkFileStatus(&file)) {
            std::cout << "File is fine\n";
            correctNameOfFile = true;
        } else {
            std::cout << "Wrong name of file, please try again\n";
        }
    }
}
/**
 * funkcja wyswietlajaca menu glowne z opcjami jakie oferuje program
 */
auto printProgramMenu() -> void {
    std::cout<< "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =\n";
    std::cout << "[MENU GLOWNE]\n";
    std::cout << "[1] - WYSZUKAJ HASLA\n";
    std::cout << "[2] - POSORTUJ HASLA\n";
    std::cout << "[3] - DODAJ HASLO\n";
    std::cout << "[4] - EDYTUJ HASLO\n";
    std::cout << "[5] - USUN HASLO\n";
    std::cout << "[6] - DODAJ KATEGORIE\n";
    std::cout << "[7] - USUN KATEGORIE\n";
    std::cout<< "= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =\n";
}
/**
 * funkcja szuka po podanych parametrach czy sie zgadzaja informacje
 * np. szuka nazwy: Google i loginu: skrillex
 * funkcja chodzi po calym pliku i szuka ktory z obecnych Password spełnia wymagania i wyświetla na ekranie
 */
auto searchForPassword(Password searchFor) -> void {

}

/**
 * Serce programu
 */
int main(){
    printWelcomeMessage();
    processOfCheckingFileCorrect();

    //teraz jest wybor co do tego co uzytkownik chce zrobic w programie, nalezy zrobic to w petli i uzyc switch case
    //w kazdej z tych opcji bedzie rozwiniecie kazdej z opcji poprzez funkcje
    while(true) {
        printProgramMenu();
        std::cout << "Wybierz opcje: ";
        int wyborOpcji;
        std::cin >> wyborOpcji;
        switch (wyborOpcji) {
            case 1:
                std::cout << "zwraca hasła, które zawierają konkretne parametry.\n";
                break;
            case 2:
                std::cout << "zwraca posortowaną listę wszystkich haseł. Ma umożliwiać posortowanie po co najmniej"
                             " 2 różnych parametrach w tym samym czasie, czyli na przykład po nazwie i kategorii.\n";
                break;
            case 3:
                std::cout << "dodaje nowe hasło do zaszyfrowanego pliku. Użytkownikowi powinna być umożliwiona opcja"
                             " wpisania własnego hasła i poinformowania go, na ile jest to bezpieczne hasło i czy nie"
                             " zostało już wcześniej wykorzystane. Dodatkowo należy zaproponować mu hasło automatycznie"
                             " wygenerowane dając mu jednocześnie możliwość wybrania pewnych parametrów, takich jak:\n"
                             "Ilość znaków;\n"
                             "Czy ma zawierać wielkie i małe litery;\n"
                             "Czy ma zawierać znaki specjalne.\n";
                break;
            case 4:
                std::cout << "pozwala na edycje danych w istniejącym już haśle.\n";
                break;
            case 5:
                std::cout << "usuwa wybrane hasło lub hasła. Przed każdym usunięciem powinniśmy powiadomić o tym"
                             " użytkownika szczególnie jeżeli usuwane jest więcej niż jedno hasło.\n";
                break;
            case 6:
                std::cout << "dodaje nową kategorię, którą będziemy mogli wykorzystywać przy tworzeniu nowych haseł.\n";
                break;
            case 7:
                std::cout << "usuwa kategorie wraz ze wszystkimi hasłami, które do tej kategorii są przypisane.\n";
                break;
            default:
                std::cout << "Wybrales zla opcje\n";
                break;
        }
    }
}