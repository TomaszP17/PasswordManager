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
#include <set>
#include "Timestamp.h"
#include "string"

void PasswordManager::setFileName(std::string fileName) {
    this->fileName = fileName;
}

/**
 * Funkcja ustawia wartosc zmiennej passwordToFile
 * @param passwordToFile - haslo do odszyfrowania pliku
 */
auto PasswordManager::setPasswordToFile(std::string passwordToFile){
    this->passwordToFile = passwordToFile;
}
/**
 * Funkcja wczytuje od uzytkownika haslo sluzace do odszyfrowania zawartosci pliku
 * @return Zwraca haslo (string)
 */
auto PasswordManager::enterPasswordToFile() -> std::string {
    std::cout << "Podaj haslo do pliku: ";
    std::string passwordFromUser;
    std::cin >> passwordFromUser;

    /*bool isGoodPassword = false;
    auto result = 0;
    while(!isGoodPassword) {
        for (char ch : passwordFromUser) {
            result += (int) ch;
        }
        if (result > 99999) {
            std::cout << "Twoje Haslo jest za dlugie, podaj inne :)\n";
            passwordFromUser = enterPasswordToFile();
        } else {
            isGoodPassword = true;
        }
    }
    return std::to_string(result);*/
    std::string result;
    for(char c : passwordFromUser){
        std::string chs = std::to_string((int)c);
        result += chs;
    }
    std::cout << result;
}
/**
 * Funkcja szyfruje dane za pomoca hasla podanego przez uzytkownika
 */
auto PasswordManager::encryptData(std::string name, std::string pass, std::string category, std::string website, std::string login) -> std::string {
    std::string input = name + "|" + pass + "|" + category + "|" + website + "|" + login;
    std::string result;
    for(int i = 0; i < input.length(); i++){
        result += char(int(input[i] + 5));
    }
    return result;
}
/**
 * Funkcja rozszyfrowywująca dane - wykonuje odwrotną operację do szyfrowania przy użyciu klucza
 * @return zwraca wektor z rozszyfrowanymi danymi
 */
auto PasswordManager::decryptData(std::string line) -> std::string {
    //std::string input = name + "|" + pass + "|" + category + "|" + website + "|" + login;
    std::string result;
    for(int i = 0; i < line.length(); i++){
            result += char(int(line[i] - 5));
    }
    return result;
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

    if(file.is_open()){
        for (const Password& p : passwordList) {
            //std::vector<std::string> arr = encryptData(p.getName(), p.getPassword(), p.getCategory(), p.getWebsite(), p.getLogin());
            //fileOut << arr[0] << "|" << arr[1] << "|" << arr[2] << "|" << arr[3] << "|" << arr[4] << '\n';
            std::string line = encryptData(p.getName(), p.getPassword(), p.getCategory(), p.getWebsite(), p.getLogin());
            file << line << std::endl;
        }
        file.close();
        return true;
    } else{
        std::cout << "\nNie udalo sie otworzyc pliku do zapisu danych.\n";
        return false;
    }
}

/**
 * Funkcja wczytuje dane z pliku fileName i zapisuje je do wektora passwordList
 * Zwraca true jesli dane zostaly wczytane, w przeciwnym przypadku false
 */
bool PasswordManager::loadFromFile() {
    std::ifstream file(fileName);
    std::string line;
    if(file.is_open()){
    while(std::getline(file, line)){
        std::string name, password, category, website, login;
        std::string decryptedLine = decryptData(line);
        std::stringstream ss(decryptedLine);
        std::getline(ss, name, '|');
        std::getline(ss, password, '|');
        std::getline(ss, category, '|');
        std::getline(ss, website, '|');
        std::getline(ss, login, '|');
        /*std::vector<std::string> arr = decryptData(name, password, category, website, login);
        Password p(arr[0], arr[1], arr[2],arr[3],arr[4]);*/
        Password p(name, password, category, website, login);
        passwordList.push_back(p);
    }
    file.close();
    return true;
} else{
        std::cout << "\nNie udalo sie otworzyc pliku do odczytu\n";
        return false;
    }
}
auto PasswordManager::printParameters()->void {
    std::cout << "1. Name\n";
    std::cout << "2. Password\n";
    std::cout << "3. Category\n";
    std::cout << "4. Website\n";
    std::cout << "5. Login\n";
}
bool PasswordManager::hasUpperCase(const std::string& password){
    for(auto const& x : password){
        if(x >= 'A' && x <= 'Z'){
            return true;
        }
    }
    return false;
}
bool PasswordManager::hasLowerCase(const std::string& password){
    for(auto const& x : password){
        if(x >= 'a' && x <= 'z'){
            return true;
        }
    }
    return false;
}
bool PasswordManager::hasDigit(const std::string& password){
    for(auto const& x : password){
        if(x >= '1' && x <= '9'){
            return true;
        }
    }
    return false;
}
bool PasswordManager::hasSpecialChar(const std::string& password){
    for(auto const& x : password){
        if(x >= '!' && x <= ')'){
            return true;
        }
    }
    return false;
}

/**
 * Funkcja sprawdza czy haslo podane przez uzytkownika jest bezpieczne
 * Minimalna dlugosc = 10 i min 1 mala litera duza litera cyfra i znak specjalny
 */
void PasswordManager::isPasswordSecure(const std::string& password){
    if(password.length() < 10){
        std::cout << "\nTWOJE HASLO POWINNO MIEC CONAJMNIEJ 10 ZNAKOW\n";
    }
    if(hasUpperCase(password) && hasLowerCase(password) && hasDigit(password) && hasSpecialChar(password)){
        std::cout << "\nTwoje haslo jest bezpieczne :)\n";
    }else{
        std::cout << "\nHaslo nie jest bezpieczne :( [powinno zawierac conajmniej 1 mala 2 duza litere 1 cyfre i 1 znak specjalny]\n";
        std::cout << "Czy pomimo to chcesz kontynuowac? [1-TAK] [2-NIE]: ";
    }

}
bool PasswordManager::isPasswordGood(std::string const& chars, bool small, bool big, bool dig, bool spec){
    if(small && !hasLowerCase(chars)){
        return false;
    }
    if(big && !hasUpperCase(chars)){
        return false;
    }
    if(dig && !hasDigit(chars)){
        return false;
    }
    if(spec && !hasSpecialChar(chars)){
        return false;
    }
    return true;
}
/**
 * Funkcja generuje haslo
 * Nie dziala dlugosc generowanych hasel tzn w przypadku podania np dlugosci 6 haslo generuje sie dluzsze
 */
std::string PasswordManager::generatePassword() {
    std::string generatedPassword;
    int passwordLength = 0;
    bool outOfLoop = false;

    //sprawdzamy czy haslo nie jest krotsze od wymaganego rozmiaru (10)
    while (!outOfLoop) {
        std::cout << "Podaj prosze dlugosc hasla: ";
        std::cin >> passwordLength;

        if (passwordLength < 10) {
            std::cout << "\nHaslo nie moze byc krotsze niz 10 znakow\n";
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
    std::string chars;
    bool small = false;
    bool big = false;
    bool digs = false;
    bool spec = false;
    int input;
    bool endOfLoop = false;
    do {
        // Pobierz preferencje użytkownika odnośnie składników hasła
        std::cout << ">>Podaj prosze czy chcesz abys mial duze Litery: [1-TAK] [2-NIE]: ";
        std::cin >> input;
        if (input == 1) {
            chars += bigLetters;
            big = true;
        }

        std::cout << ">>Podaj prosze czy chcesz abys mial male litery: [1-TAK] [2-NIE]: ";
        std::cin >> input;
        if (input == 1) {
            chars += smallLetters;
            small = true;
        }

        std::cout << ">>Podaj prosze czy chcesz abys mial cyfry: [1-TAK] [2-NIE]: ";
        std::cin >> input;
        if (input == 1) {
            chars += digits;
            digs = true;
        }

        std::cout << ">>Podaj prosze czy chcesz abys znaki specjalne: [1-TAK] [2-NIE]: ";
        std::cin >> input;
        if (input == 1) {
            chars += specials;
            spec = true;
        }

        if (chars.length() <= 0) {
            std::cout << "NIE PODALES ZADNYCH DOSTEPNYCH ZNAKOW WIEC NIE JESTEM W STANIE WYGENEROWAC HASLA :(\n";
            small = false;
            big = false;
            digs = false;
            spec = false;
        } else {
            endOfLoop = true;
        }
    } while (!endOfLoop);

    srand(time(nullptr));

    while (!isPasswordGood(generatedPassword, small, big, digs, spec)) {
        generatedPassword = "";
        for (int i = 0; i < passwordLength; i++) {
            generatedPassword += chars[rand() % chars.length()];
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
    std::cout << ">>Podaj nazwe np. (Google): ";
    //std::cin >> name;
    std::cin.get();
    std::getline(std::cin, name);
    bool passwordSet = false;
    while(!passwordSet) {
        std::cout << ">>Podaj haslo lub wygeneruj automatycznie wybierajac [1] na klawiaturze";
        //std::cin >> password;
        std::getline(std::cin, password);
        if (password == "1") {
            password = generatePassword();
            std::cout << "Twoje wygenerowane haslo to: " << password << '\n';
            passwordSet = true;
        } else {
            if(isPasswordExists(password)){
                std::cout << "Haslo wystepowalo w przeszlosci, wprowadz inne ze wzgledow bezpieczenstwa\n";
                passwordSet = false;
            }else{
                isPasswordSecure(password);
                std::string decision;
                std::getline(std::cin, decision);
                if(decision == "1"){
                    passwordSet = true;
                } else{
                    passwordSet = false;
                }
            }
        }
    }
    bool isCategoryOkay = false;
    std::cin.get();
    while(!isCategoryOkay) {
        std::cout << ">>Podaj kategorie z podanych powyzej: \n";
        printCategory();
        std::getline(std::cin, category);
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
    std::cout << ">>Podaj strone (opcjonalne): ";
    std::getline(std::cin, website);
    std::cout << ">>Podaj login (opcjonalne): ";
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
    std::cout << ">>Wybor: ";
    std::cin >> choise;

    if(choise == 1){
        //dzialanie dla 1 hasla, uzytkownik podaje indeks hasla aby je usunac
        printPasswords(); //wyswietla hasla

        int index = 0;
        std::cout << ">>Podaj indeks hasla do usuniecia: [UWAGA OPERACJA TA POWODUJE NIEODWRACALNE ZMIANY W BAZIE HASEL] ";
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
        std::cout << ">>Podaj ile chcesz usunac hasel: [UWAGA OPERACJA TA POWODUJE NIEODWRACALNE ZMIANY W BAZIE HASEL] ";
        std::cin >> howManyIndex;

        for(int i = 0; i < howManyIndex; i++){
            printPasswords();
            int index = 0;
            std::cout << ">>Podaj indeks hasla do usuniecia: ";
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
        printParameters();
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
                std::cout << ">>Podaj haslo lub wygeneruj automatycznie wybierajac [1] na klawiaturze";
                std::cin >> newPassword;
                if (newPassword == "1") {
                    newPassword = generatePassword();
                    std::cout << "Twoje wygenerowane haslo to: " << newPassword << '\n';
                    passwordSet = true;
                } else {
                    if(isPasswordExists(newPassword)){
                        std::cout << "Haslo wystepowalo w przeszlosci, wprowadz inne ze wzgledow bezpieczenstwa\n";
                        passwordSet = false;
                    }
                }
            }
            passwordList[index].setPassword(newPassword);
            saveToFile();
        } else if (choise == 3) {
            std::string newCategory;
            std::cout << ">>Podaj nowa kategorie: ";
            std::cin >> newCategory;
            passwordList[index].setCategory(newCategory);
            saveToFile();
        } else if (choise == 4) {
            std::string newWebsite;
            std::cout << ">>Podaj nowa strone: ";
            std::cin >> newWebsite;
            passwordList[index].setWebsite(newWebsite);
            saveToFile();
        } else if (choise == 5) {
            std::string newLogin;
            std::cout << "Podaj nowy login: ";
            std::cin >> newLogin;
            passwordList[index].setLogin(newLogin);
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
    std::cout << "Wybierz parametr, po ktorym chcesz szukac: \n";
    printParameters();
    std::cout << "Podaj index po ktorym chcesz szukac: [PODAWAJ INDEXY PO SPACJI]";
    std::cin.get();
    std::string index;
    std::getline(std::cin, index);
    std::set<int> indexes;
    std::stringstream ss(index);
    int number;
    while(ss >> number){
        indexes.insert(number);
    }

    std::set<Password> result;
    std::string phraseFromUser;
for(auto const& x : indexes) {
    switch (x) {
        case 1:
            std::cout << "Podaj name jakiego szukasz: ";
            std::getline(std::cin, phraseFromUser);
            for (Password& y: passwordList) {
                std::string name = y.getName();
                size_t found = name.find(phraseFromUser);
                if (found != std::string::npos) {
                    result.insert(y);
                }
            }
            break;
        case 2:
            std::cout << "Podaj password jakiego szukasz: ";
            std::getline(std::cin, phraseFromUser);
            for (Password &y: passwordList) {
                std::string password = y.getPassword();
                size_t found = password.find(phraseFromUser);
                if (found != std::string::npos) {
                    result.insert(y);
                }
            }
            break;
        case 3:
            //szukanie po kategorii
            std::cout << "Podaj kategorie jakiego szukasz: ";
            std::getline(std::cin, phraseFromUser);
            for (Password &y: passwordList) {
                std::string category = y.getCategory();
                size_t found = category.find(phraseFromUser);
                if (found != std::string::npos) {
                    result.insert(y);
                }
            }
            break;
        case 4:
            //szukanie po stronie internetowej
            std::cout << "Podaj website jakiego szukasz: ";
            std::getline(std::cin, phraseFromUser);
            for (Password &y: passwordList) {
                std::string website = y.getWebsite();
                size_t found = website.find(phraseFromUser);
                if (found != std::string::npos) {
                    result.insert(y);
                }
            }
            break;
        case 5:
            //szukanie po loginie
            std::cout << "Podaj login jakiego szukasz: ";
            std::getline(std::cin, phraseFromUser);
            for (Password &y: passwordList) {
                std::string login = y.getLogin();
                size_t found = login.find(phraseFromUser);
                if (found != std::string::npos) {
                    result.insert(y);
                }
            }
            break;
        default:
            std::cout << "Podales nieprawidlowy index :(\n";
            break;
    }
}
    for (const auto &x: result) {
        std::cout << ". " << x.getName() << " | " << x.getPassword() << " | " << x.getCategory() << " | "
                  << x.getWebsite() << " | " << x.getLogin() << '\n';
    }
}
/**
 * Funkcja sortuje po parametrach
 * NALEZY JAK NAJSZYBCIEJ JA EDYTOWAC
 */
void PasswordManager::sortPasswords(){
    std::cout << "Podaj parametry sortowania:\n";
    printParameters();
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
        int choice;
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

    std::cout << "\nWitaj w Password Manager autorstwa Tomasza Plucinskiego\n";
    std::cout << "Aktualna wersja programu to: 0.1v\n";
    std::cout << "W przypadku jakichkolwiek problemow skontaktuj sie ze mna mailowo: papieskapomoc@gmail.com\n";
    std::cout << ">>- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - <<\n";
    while (true) {
        fileName = enterFileName();
        setFileName(fileName);
        if (fileExists()) {
            passwordToFile = enterPasswordToFile();
            setPasswordToFile(passwordToFile);
            if (loadFromFile()) {
                break;
            }
        }
    }
    menu();
}