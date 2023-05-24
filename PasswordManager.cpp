//
// Created by Tomasz on 02.05.2023.
//
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <filesystem>
#include "PasswordManager.h"
#include <vector>

PasswordManager::PasswordManager() {}

void PasswordManager::setFileName(std::string fileName) {
    this->fileName = fileName;
}
/**
 * Funkcja ustawia wartosc zmiennej passwordToFile
 * @param passwordToFile - haslo do odszyfrowania pliku
 * @return - void
 */
auto PasswordManager::setPasswordToFile(std::string passwordToFile){
    this->passwordToFile = passwordToFile;
}
/**
 * Funkcja wczytuje od uzytkownika haslo sluzace do odszyfrowania zawartosci pliku
 * @return Zwraca haslo
 */
auto PasswordManager::enterPasswordToFile() -> std::string {
    std::cout << "Podaj haslo do pliku: ";
    std::string passwordFromUser;
    std::cin >> passwordFromUser;
    //return passwordFromUser;
    bool isGoodPassword = false;
    auto result = 0;
    counterOfResult = 0;
    while(!isGoodPassword) {
        for (int i = 0; i < passwordFromUser.length(); i++) {
            char ch = passwordFromUser[i];
            result += (int) ch;
        }
        if (result > 99999) {
            std::cout << "Twoje Haslo jest za dlugie, podaj inne :)\n";
            passwordFromUser = enterPasswordToFile();
        } else {
            isGoodPassword = true;
        }
    }
    return std::to_string(result);
}
/**
 * Funkcja szyfruje dane za pomoca hasla podanego przez uzytkownika
 */
auto PasswordManager::encryptData(std::string name, std::string pass, std::string category, std::string website, std::string login) -> std::vector<std::string> {
    std::vector<std::string> arr = {name, pass, category, website, login};
    std::vector<std::string> arr2;
    counterOfResult = 0;

    for(int i = 0; i < arr.size(); i++){
        std::string wordBefore = arr[i];
        std::string stringResult = passwordToFile;
        std::string newString;

        for(int j = 0; j < wordBefore.length(); j++){
            char ch = wordBefore[j];
            //int digitOfResult = (int)(stringResult[counterOfResult]) - '0';
            int digitOfResult = (int)(stringResult[counterOfResult % stringResult.length()]) - '0';
            ch += digitOfResult;
            newString += ch;
            counterOfResult+=1;
        }
        arr2.push_back(newString);
        if(counterOfResult < stringResult.length()){
            counterOfResult += 1;
        }
    }
    return arr2;

}
auto PasswordManager::decryptData(std::string name, std::string pass, std::string category, std::string website, std::string login) -> std::vector<std::string>{
    std::vector<std::string> arr = {name, pass, category, website, login};
    std::vector<std::string> arr2;
    counterOfResult = 0;

    for(int i = 0; i < arr.size(); i++){
        std::string wordBefore = arr[i];
        std::string stringResult = passwordToFile;
        std::string newString;

        for(int j = 0; j < wordBefore.length(); j++){
            char ch = wordBefore[j];
            //int digitOfResult = (int)(stringResult[counterOfResult]) - '0';
            int digitOfResult = (int)(stringResult[counterOfResult % stringResult.length()]) - '0';
            ch -= digitOfResult;
            newString += ch;
            counterOfResult+=1;
        }
        arr2.push_back(newString);
        /*if(counterOfResult < stringResult.length()){
            counterOfResult += 1;
        }*/

    }
    return arr2;
}
/**
* Funkcja wczytuje od uzytkownika nazwe pliku ktory chce otworzyc
* Zwraca nazwe tego pliku
* Funkcja moze tez przyjmowac absolutna sciezke jako nazwe pliku
*/
auto PasswordManager::enterFileName() -> std::string {
    //wyswietlenie zawartosci folderu w ktorym przetrzymywane sa zaszyfrowane pliki uzytkownika
    auto const dirPath = std::string("C:\\Users\\Tomasz\\Desktop\\MenadzerHasel\\PM_files");
    auto dirIterator = std::filesystem::recursive_directory_iterator(dirPath);
    //przechodzi przez pliki w folderze (sciezka dirPath)
    std::cout << "Ponizej znajduje sie lista dostepnych plikow na ktorych mozesz operowac, wybierz nazwe lub podaj jego absolutna sciezke: \n";
    for(auto const& entry : dirIterator){
        std::cout << entry.path() << '\n';
    }
    //wczytuje nazwe pliku
    std::cout << "\nPodaj nazwe pliku, ktory chcesz otworzyc: ";
    std::string nazwaPliku;
    std::cin >> nazwaPliku;
    std::filesystem::path filePath(nazwaPliku);
    //sprawdzamy czy sciezka pliku jest sciezka absolutna jezeli tak to spoko jak nie to zamieniamy nazwe pliku w sciezke absolutna recznie
    if(filePath.is_absolute()){
        return nazwaPliku;
    }else{
        nazwaPliku= dirPath + "\\" + nazwaPliku;
        return nazwaPliku;
    }
}
/**
* Funkcja dostaje na wejscie wskaznik pliku i sprawdza czy taki plik istnieje w folderze programu
* W pozytywnym przypadku zwraca true w przeciwnym false
*/
auto PasswordManager::checkFileStatus(std::fstream *file) -> bool {
    //funkcja good() sprawdza czy nie wystapil zaden blad w w strumieniu
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
 * Funkcja zapisuje dane z wektora passwordList do pliku fileName
 * Zwraca true jesli dane zostaly zapisane, w przeciwnym przypadku false
 */
bool PasswordManager::saveToFile() {
    std::ofstream file(fileName);
    if (file.is_open()) {
        for (const Password& p: passwordList) {
            //tutaj szyfrowanie
            std::vector<std::string> arr = encryptData(p.getName(), p.getPassword(), p.getCategory(), p.getWebsite(), p.getLogin());
            file << arr[0] << " " << arr[1] << " " << arr[2] << " " << arr[3]
                 << " " << arr[4] << '\n';
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
            //funkcja odszyfrowywujaca
            std::vector<std::string> arr = decryptData(name, password, category, website, login);
            Password p(arr[0], arr[1], arr[2],arr[3],arr[4]);
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
 * Funkcja sprawdza czy haslo podane przez uzytkownika jest bezpieczne
 * Minimalna dlugosc = 6 i min 1 mala litera duza litera cyfra i znak specjalny
 */
bool PasswordManager::isPasswordSecure(const std::string& password){
    if(password.length() < 6){
        return false;
    }
    bool hasupperCase = false;
    bool haslowerCase = false;
    bool hasDigit = false;
    bool hasSpecial = false;
    for(auto x : password){
        if(x >= 'A' && x <= 'Z'){
            hasupperCase = true;
        }else if(x >= 'a' && x <= 'z'){
            haslowerCase = true;
        }else if(x >= '1' && x <= '9'){
            hasDigit = true;
        }else{
            //skoro nie jest litera ani cyfra to musi byc to jakis znak specjalny
            hasSpecial = true;
        }
    }
    if(hasDigit && haslowerCase && hasupperCase && hasSpecial){
        std::cout << "\nTwoje haslo jest bezpieczne :)\n";
        return true;
    }else{
        std::cout << "\nHaslo nie jest bezpieczne :(\n";
        return false;
    }
}
/**
 * Funkcja generuje haslo
 * Nie dziala dlugosc generowanych hasel tzn w przypadku podania np dlugosci 6 haslo generuje sie dluzsze
 */
std::string PasswordManager::generatePassword(){
    std::string generatedPassword;
    int passwordLength = 0;
    bool outOfLoop = false;
    //sprawdzamy czy haslo nie jest krotsze od wymaganego rozmiaru (6)
    while(!outOfLoop){
        std::cout << "Podaj prosze dlugosc hasla: ";
        std::cin >> passwordLength;
        if(passwordLength < 6){
            std::cout << "\nHaslo nie moze byc krotsze niz 6 znakow\n";
            outOfLoop = false;
            passwordLength = 0;
            continue;
        }
        outOfLoop = true;
    }

    //teraz generuje haslo o dlugosci passwordLength z liter duzych i malych cyfr i znakow specjalnych
    std::string smallLetters = "abcdefghijklmnoprstuwxyz";
    std::string bigLetters = "ABCDEFGHIJKLMNOPRSTUWXYZ";
    std::string digits = "0123456789";
    std::string specials = "!@#$%^&*()";
    std::string chars = "";
    chars += smallLetters+bigLetters+digits+specials;

    srand (time(nullptr));
    while(!isPasswordSecure(generatedPassword)){
        int counter = 0;
        while(counter < passwordLength){
            generatedPassword += chars[rand()%chars.length()];
            counter++;
        }
    }
    return generatedPassword;
}

bool PasswordManager::isPasswordExists(const std::string& password){
    for(const auto& x : passwordList){
        if(password == x.getPassword()){
            //std::cout << "Haslo bylo juz uzywane, prosze zmien je\n";
            return true;
        }
    }
    return false;
}
/**
 * Funkcja dodaje nowe haslo do wektora passwordList
 * nie dziala sprawdzanie czy haslo wystepowalo w bazie i to powinno blokowac przechodzenie do nastepnego etapu
 */
void PasswordManager::addPassword() {
    std::string name, password, category, website, login;
    std::cout << "\nDodawanie nowego hasla\n";
    std::cout << "Podaj nazwe np. (Google): ";
    std::cin >> name;
    bool passwordSet = false;
    while(!passwordSet) {
        std::cout << "Podaj haslo lub wygeneruj automatycznie wybierajac [1] na klawiaturze";
        std::cin >> password;
        if (password == "1") {
            password = generatePassword();
            std::cout << "Twoje wygenerowane haslo to: " << password << '\n';
            passwordSet = true;
        } else {
            if(isPasswordExists(password)){
                std::cout << "Haslo wystepowalo w przeszlosci, wprowadz inne ze wzgledow bezpieczenstwa\n";
                passwordSet = false;
            }else{
                if(!isPasswordSecure(password)){
                    std::cout << "Haslo nie jest bezpieczne\n";
                    passwordSet = false;
                }else{
                    passwordSet = true;
                }
            }
        }
    }
    bool isCategoryOkay = false;
    while(!isCategoryOkay) {
        std::cout << "Podaj kategorie z podanych ponizej: \n";
        printCategory();
        std::cin >> category;
        for (const auto &x: categoryList) {
            if (category == x) {
                isCategoryOkay = true;
                break;
            }
        }
        if (!isCategoryOkay) {
            std::cout << "Podales zla kategorie, sprobuj ponownie!\n";
        }
    }
    std::cin.get();
    std::cout << "Podaj strone (opcjonalne): ";
    std::getline(std::cin, website);
    std::cout << "Podaj login (opcjonalne): ";
    std::getline(std::cin, login);
    Password p(name, password, category, website, login);
    passwordList.push_back(p);
    saveToFile();
}

/**
 * Funkcja usuwa haslo o podanym indeksie z wektora passwordList
 *
 * Usuń hasło – usuwa wybrane hasło lub hasła. Przed każdym usunięciem powinniśmy powiadomić
 * o tym użytkownika szczególnie jeżeli usuwane jest więcej niż jedno hasło.
 */
void PasswordManager::removePassword() {
    std::cout << "\nChcesz usunac 1 czy wiecej hasel?\n";
    std::cout << "[1] - 1 haslo\n";
    std::cout << "[2] - wiecej hasel\n";

    int choise = 0;
    std::cout << "Wybor: ";
    std::cin >> choise;

    if(choise == 1){
        //dzialanie dla 1 hasla, uzytkownik podaje indeks hasla aby je usunac
        printPasswords(); //wyswietla hasla

        int index = 0;
        std::cout << "Podaj indeks hasla do usuniecia: [UWAGA OPERACJA TA POWODUJE NIEODWRACALNE ZMIANY W BAZIE HASEL] ";
        std::cin >> index;
        if(index < passwordList.size()){
            passwordList.erase(passwordList.begin() + index);
            saveToFile();
        }else{
            std::cout << "\nBledny index\n";
        }

    }else if(choise == 2){
        //dzialanie dla wielu hasel, uzytkownik podaje indeksy ktore zapisuje do tablicy intow i przechodzac po kazdym elemencie
        //tej tablicy usuwa te indeksy wczesniej wyswietlajac komunikat ostrzegajacy o tym

        printPasswords();
        int howManyIndex = 0;
        std::cout << "Podaj ile chcesz usunac hasel: [UWAGA OPERACJA TA POWODUJE NIEODWRACALNE ZMIANY W BAZIE HASEL] ";
        std::cin >> howManyIndex;

        for(int i = 0; i < howManyIndex; i++){
            printPasswords();
            int index = 0;
            std::cout << "Podaj indeks hasla do usuniecia: ";
            std::cin >> index;
            if(index < passwordList.size()){
                passwordList.erase(passwordList.begin() + index);
                saveToFile();
            }else{
                std::cout << "\nBledny index\n";
            }
        }

    }else{
        //jak uzytkownik to idiota i nie potrafi wybrac opcji prawdilowej
        std::cout << "\nWybrales zla opcje\n";
    }
}
/**
 * Funkcja wypisuje na ekranie wszystkie hasla znajdujace sie w wektorze passwordList
 */
void PasswordManager::printPasswords() {
    std::cout << "\nLista zapisanych hasel:\n";
    int i = 0;
    for (const Password& p: passwordList) {
        std::cout << i++ << ". " << p.getName() << " | " << p.getPassword() << " | " << p.getCategory()
                  << " | " << p.getWebsite() << " | " << p.getLogin() << '\n';
    }
}
/**
 * Funkcja umożliwia zmiane hasla o podanym indeksie w wektorze passwordList
 * Edytuj hasło – pozwala na edycje danych w istniejącym już haśle.
 */
void PasswordManager::changePassword() {
    std::cout << "\nZmiana hasla\n";
    int counter = 0;
    //wyswietla hasla z indeksami zaczynajac od 0 do X
    for(const auto& x : passwordList){
        std::cout << counter++ << ". " << x.getName() << " | " << x.getPassword() << " | " << x.getCategory() << " | " << x.getWebsite() << " | " << x.getLogin() << '\n';
    }
    int index;
    std::cout << "Podaj indeks hasla do zmiany: ";
    std::cin >> index;

    //sprawdza czy index jest prawidlowy jesli tak to przechodzi do wyboru zmiany w istniejacym juz hasle
    if (index < passwordList.size()) {
        std::cout << "\nCo chcesz zmienić w poniższym rekordzie?\n";
        std::cout << "1. Nazwe\n";
        std::cout << "2. Haslo\n";
        std::cout << "3. Kategorie\n";
        std::cout << "4. Strone\n";
        std::cout << "5. Login\n";
        int choise = 0;
        std::cin >> choise;
        if (choise == 1) {
            std::string newName;
            std::cout << "Podaj nowa nazwe: ";
            std::cin >> newName;
            passwordList[index].setName(newName);
            saveToFile();
        } else if (choise == 2) {
            bool passwordSet = false;
            std::string newPassword;
            while(!passwordSet) {
                std::cout << "Podaj haslo lub wygeneruj automatycznie wybierajac [1] na klawiaturze";
                std::cin >> newPassword;
                if (newPassword == "1") {
                    newPassword = generatePassword();
                    std::cout << "Twoje wygenerowane haslo to: " << newPassword << '\n';
                    passwordSet = true;
                } else {
                    if(isPasswordExists(newPassword)){
                        std::cout << "Haslo wystepowalo w przeszlosci, wprowadz inne ze wzgledow bezpieczenstwa\n";
                        passwordSet = false;
                    }else{
                        if(!isPasswordSecure(newPassword)){
                            std::cout << "Haslo nie jest bezpieczne\n";
                            passwordSet = false;
                        }else{
                            passwordSet = true;
                        }
                    }
                }
            }
            passwordList[index].setPassword(newPassword);
            saveToFile();
        } else if (choise == 3) {
            std::string newCategory;
            std::cout << "Podaj nowa kategorie: ";
            std::cin >> newCategory;
            passwordList[index].setCategory(newCategory);
            saveToFile();
        } else if (choise == 4) {
            std::string newWebsite;
            std::cout << "Podaj nowa strone: ";
            std::cin >> newWebsite;
            passwordList[index].setWebsite(newWebsite);
            saveToFile();
        } else if (choise == 5) {
            std::string newLogin;
            std::cout << "Podaj nowy login: ";
            std::cin >> newLogin;
            passwordList[index].setWebsite(newLogin);
            saveToFile();
        } else {
            std::cout << "Wybrales zly numer :(\n";
        }
    }else {
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
    for(const auto& x : passwordList){
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
    for(const auto& x : results){
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
void PasswordManager::printCategory(){
    int index = 0;
    for(const auto& x : categoryList){
        std::cout << index++ << ". " << x << '\n';
    }
}

void PasswordManager::addCategory(){
    std::cout << "Oto kategorie ktore juz istnieja: \n";
    printCategory(); //wyswietla zawartosc vectora categoryList

    std::string newCategory;
    std::cout << "Podaj nazwe kategorii jaka chcesz dodac: ";
    std::cin >> newCategory;

    bool categoryExist = false;
    for(const auto& x : categoryList){
        if(newCategory == x){
            categoryExist = true;
            break;
        }
    }

    if(!categoryExist){
        categoryList.push_back(newCategory);
        std::cout << "Kategoria " << newCategory << " zostala dodana!\n";
    }else{
        std::cout << "Nie mozna dodac kategorii, poniewaz ta juz istnieje!\n";
    }
}

void PasswordManager::deleteCategory(){

    std::cout << "Ponizej znajduje sie lista aktualnych kategorii: \n";
    printCategory();

    int index = 0;
    std::cout << "Podaj index kategorii do usuniecia: [UWAGA OPERACJA TA POWODUJE NIEODWRACALNE ZMIANY W BAZIE HASEL] ";
    std::cin >> index;

    //funkcja usuwa kategorie ale wraz z kategoria wszystkie hasla ktore maja w sobie ta kategorie
    //potrzebna implementacja przechodzenia przez passwordList i sprawdzaniu nazwy kategorii i usuwaniu tej wprowadzonej
    if(index < categoryList.size()){
        //implementacja usuwania hasel z ta kategoria
        std::string thisCategory = categoryList[index];
        std::cout << thisCategory;
        for(int i = 0; i < passwordList.size(); i++){
            if(thisCategory == passwordList[i].getCategory()){
                passwordList.erase(passwordList.begin() + i);
            }
        }
        categoryList.erase(categoryList.begin() + index);
        saveToFile();
    }else{
        std::cout << "\nBledny index\n";
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
        std::cout << "[4] Edytuj haslo\n";
        std::cout << "[5] Wyszukaj hasla\n";
        std::cout << "[6] Posortuj hasla\n";
        std::cout << "[7] Dodaj kategorie\n";
        std::cout << "[8] Usun kategorie\n";
        std::cout << "[9] Zakoncz program\n";
        std::cout << ">>- - - - - - - - - - - - - - - - - - - - - - -<<\n";
        std::cout << "Co wybierasz?: ";
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
                searchPassword();
                break;
            case 6:
                sortPasswords();
                break;
            case 7:
                addCategory();
                break;
            case 8:
                deleteCategory();
                break;
            case 9:
                //std::cout << "░░░░░░░░░░░░░▄▄▀▀▀▀▀▀▄▄\n░░░░░░░░░░▄▄▀▄▄▄█████▄▄▀▄\n░░░░░░░░▄█▀▒▀▀▀█████████▄█▄\n░░░░░░▄██▒▒▒▒▒▒▒▒▀▒▀▒▀▄▒▀▒▀▄\n░░░░▄██▀▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒█▄\n░░░░██▀▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒█▌\n░░░▐██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▐█\n░▄▄███▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒█\n▐▒▄▀██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▐▌\n▌▒▒▌▒▀▒▒▒▒▒▒▄▀▀▄▄▒▒▒▒▒▒▒▒▒▒▒▒█▌\n▐▒▀▒▌▒▒▒▒▒▒▒▄▄▄▄▒▒▒▒▒▒▒▀▀▀▀▄▒▐\n░█▒▒▌▒▒▒▒▒▒▒▒▀▀▒▀▒▒▐▒▄▀██▀▒▒▒▌\n░░█▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▐▒▒▒▒▒▒▒▒█\n░░░▀▌▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▌▒▒▒▒▒▒▄▀\n░░░▐▒▒▒▒▒▒▒▒▒▄▀▐▒▒▒▒▒▐▒▒▒▒▄▀\n░░▄▌▒▒▒▒▒▒▒▄▀▒▒▒▀▄▄▒▒▒▌▒▒▒▐▀▀▀▄▄▄\n▄▀░▀▄▒▒▒▒▒▒▒▒▀▀▄▄▄▒▄▄▀▌▒▒▒▌░░░░░░\n▐▌░░░▀▄▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▄▀░░░░░░░\n░█░░░░░▀▄▄▒▒▒▒▒▒▒▒▒▒▒▒▄▀░█░░░░░░░\n░░█░░░░░░░▀▄▄▄▒▒▒▒▒▒▄▀░░░░█░░░░░░\n░░░█░░░░░░░░░▌▀▀▀▀▀▀▐░░░░░▐▌░░░░░";
                exit(0); //zamyka konsole
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

    //serce programu
    //tutaj jest zalączanie plikow i sprawdzanie czy istnieją

    std::cout << "\nWitaj w Password Manager autorstwa Tomasza Plucinskiego\n";
    std::cout << "Aktualna wersja programu to: 0.1v\n";
    std::cout << "W przypadku jakichkolwiek problemow skontaktuj sie ze mna mailowo: papieskapomoc@gmail.com\n";
    std::cout << ">>- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - <<\n";
    while (true) {
        fileName = enterFileName();
        setFileName(fileName);
        if (fileExists()) {
            //tutaj trzeba przeprowadzic pytanie o haslo i odszyfrowanie danych
            passwordToFile = enterPasswordToFile();
            setPasswordToFile(passwordToFile);
            if (loadFromFile()) {
                break;
            }
        }
    }
    menu();
}
