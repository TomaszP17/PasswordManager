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

template<typename T>
void PasswordManager::getInputFromUser(T& data){
    while (true) {
        try {
            std::cin >> data;
            if (std::cin.fail()) {
                throw std::runtime_error("Bad input");
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        } catch (const std::exception& e) {
            std::cout << "Enter new input: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}
void PasswordManager::getInputFromUser(std::string& data) {
    while (true) {
        try {
            std::getline(std::cin, data);
            if (std::cin.fail()) {
                throw std::runtime_error("Bad input.");
            }
            break;
        } catch (const std::exception& e) {
            std::cout << "Enter new input: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}


void PasswordManager::setFileName(std::string fileName) {
    this->fileName = fileName;
}

auto PasswordManager::setPasswordToFile(std::string passwordToFile){
    this->passwordToFile = passwordToFile;
}

auto PasswordManager::setTimeStamp(std::string timestamp){
    this->lastTimeStamp = timestamp;
}

auto PasswordManager::enterPasswordToFile() -> std::string {
    std::cout << ">>Enter password to file: ";
    std::string passwordFromUser;
    getInputFromUser(passwordFromUser);
    bool isGood = false;
    std::string result;

    for(char c : passwordFromUser){
        std::string chs = std::to_string((int)c);
        result += chs;
    }
    return result;
}

auto PasswordManager::encryptData(const std::string& name, const std::string& pass, const std::string& category, const std::string& website, const std::string& login) -> std::string {
    std::string input = name + "|" + pass + "|" + category + "|" + website + "|" + login;
    std::string result;
    int counterOfResult = 0;

    for (int i = 0; i < input.length(); i++) {
        char encryptedChar = input[i] + passwordToFile[counterOfResult] + counterOfResult + passwordToFile.length();
        result += char(encryptedChar ^ passwordToFile.length());
        counterOfResult = (counterOfResult + 1) % passwordToFile.length();
    }

    return result;
}

auto PasswordManager::decryptData(std::string line) -> std::string {
    std::string result;
    int counterOfResult = 0;

    for (int i = 0; i < line.length(); i++) {
        char decryptedChar = line[i] ^ passwordToFile.length();
        decryptedChar -= passwordToFile[counterOfResult] + counterOfResult + passwordToFile.length();
        result += decryptedChar;
        counterOfResult = (counterOfResult + 1) % passwordToFile.length();
    }

    return result;
    }

auto PasswordManager::enterFileName() -> std::string {
    auto const dirPath = std::string("C:\\Users\\Tomasz\\Desktop\\MenadzerHasel\\PM_files");
    auto dirIterator = std::filesystem::recursive_directory_iterator(dirPath);
    std::cout << "Below is a list of available files you can offer on, choose a name or enter its absolute path: \n";
    for(auto const& entry : dirIterator){
        std::cout << entry.path() << '\n';
    }

    std::cout << "\nEnter the name of the file you want to open: ";
    std::string nameOfFile;
    getInputFromUser(nameOfFile);
    std::filesystem::path filePath(nameOfFile);

    if(filePath.is_absolute()){
        return nameOfFile;
    }else{
        nameOfFile= dirPath + "\\" + nameOfFile;
        return nameOfFile;
    }
}

auto PasswordManager::checkFileStatus(std::fstream *file) -> bool {
    if (file->good()) {
        return true;
    } else {
        return false;
    }
}

bool PasswordManager::fileExists() {
    std::fstream file(fileName);
    return checkFileStatus(&file);
}

bool PasswordManager::saveToFile() {
    std::ofstream file(fileName);
    if (file.is_open()) {
        counterOfResult = 0;
        for (const Password &p: passwordList) {
            std::string line = encryptData(p.getName(), p.getPassword(), p.getCategory(), p.getWebsite(), p.getLogin());
            file << line << std::endl;
        }
        file << lastTimeStamp << std::endl;
        file.close();
        return true;
    } else {
        std::cout << "\nFailed to open save file.\n";
        return false;
    }
}

int PasswordManager::counterLines(std::ifstream& file){
    int counter = 0;
    std::string line;
    while(std::getline(file, line)){
        ++counter;
    }
    return counter;
}

std::string PasswordManager::newTimeStamp(){
    Timestamp timestamp;
    std::string hours = std::to_string(timestamp.getHours());
    std::string minutes = std::to_string(timestamp.getMinutes());
    std::string secundos = std::to_string(timestamp.getSeconds());
    std::string line;
    if(hours.length() < 2){
        hours = "0" + hours;
    }
    if(minutes.length() < 2){
        minutes = "0" + minutes;
    }
    if(secundos.length() < 2){
        secundos = "0" + secundos;
    }
    line = hours+":"+minutes+":"+secundos;

    return line;
}

bool PasswordManager::loadFromFile() {
    std::ifstream fileIn(fileName);

    if(!fileIn.is_open()){
        return false;
    }
    int numberOfLines = counterLines(fileIn);
    fileIn.close();

    std::ifstream file(fileName);
    std::string line;
    if(file.is_open()){
        counterOfResult = 0;
        int counterOfWhileLoop = 0;
        while(std::getline(file, line)){
            ++counterOfWhileLoop;
            if(counterOfWhileLoop == numberOfLines){
                lastTimeStamp = newTimeStamp();
            }else {
                std::string name, password, category, website, login;
                std::string decryptedLine = decryptData(line);
                std::stringstream ss(decryptedLine);
                std::getline(ss, name, '|');
                std::getline(ss, password, '|');
                std::getline(ss, category, '|');
                std::getline(ss, website, '|');
                std::getline(ss, login, '|');

                Password p(name, password, category, website, login);
                passwordList.push_back(p);
            }
        }
        file.close();
        return true;
} else{
        std::cout << "\nFailed to open file for reading\n";
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

void PasswordManager::isPasswordSecure(const std::string& password){
    if(password.length() < 10){
        std::cout << "\nYOUR PASSWORD SHOULD BE AT LEAST 10 CHARACTERS\n";
    }
    if(hasUpperCase(password) && hasLowerCase(password) && hasDigit(password) && hasSpecialChar(password)){
        std::cout << "\nYour password is safe :)\n";
    }else{
        std::cout << "\nPassword is not secure :( [should contain at least 1 lowercase, 1 uppercase letters, 1 number and 1 special character]\n";
        std::cout << "Do you want to continue anyway? [1-YES] [2-NO]: ";
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

std::string PasswordManager::generatePassword() {
    std::string generatedPassword;
    int passwordLength = 0;
    bool outOfLoop = false;

    while (!outOfLoop) {
        std::cout << "Please enter password length: ";
        //std::cin >> passwordLength;
        getInputFromUser(passwordLength);

        if (passwordLength < 10) {
            std::cout << "\nThe password cannot be shorter than 10 characters\n";
            outOfLoop = false;
            passwordLength = 0;
            continue;
        }
        outOfLoop = true;
    }

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
        std::cout << ">>Please specify if you want upper case letters: [1-YES] [2-NO]: ";
        //std::cin >> input;
        getInputFromUser(input);
        if (input == 1) {
            chars += bigLetters;
            big = true;
        }

        std::cout << ">>Please specify if you want lower case letters: [1-YES] [2-NO]: ";
        //std::cin >> input;
        getInputFromUser(input);
        if (input == 1) {
            chars += smallLetters;
            small = true;
        }

        std::cout << ">>Please indicate if you want to have digits: [1-YES] [2-NO]: ";
        //std::cin >> input;
        getInputFromUser(input);
        if (input == 1) {
            chars += digits;
            digs = true;
        }

        std::cout << ">>Please specify if you want special characters: [1-YES] [2-NO]: ";
        //std::cin >> input;
        getInputFromUser(input);
        if (input == 1) {
            chars += specials;
            spec = true;
        }

        if (chars.length() <= 0) {
            std::cout << "YOU DID NOT PROVIDE ANY AVAILABLE CHARACTERS SO I AM NOT ABLE TO GENERATE A PASSWORD :(\n";
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
            return true;
        }
    }
    return false;
}

void PasswordManager::addPassword() {
    std::string name, password, category, website, login;
    std::cout << "\nAdding a new password\n";
    std::cout << ">>Enter the name, e.g. (Google): ";

    getInputFromUser(name);
    bool passwordSet = false;
    while(!passwordSet) {
        std::cout << ">>Enter the password or generate it automatically by selecting [1] on the keyboard";
        getInputFromUser(password);
        if (password == "1") {
            password = generatePassword();
            std::cout << "Your generated password is: " << password << '\n';
            passwordSet = true;
        } else {
            if(isPasswordExists(password)){
                std::cout << "Password used in the past, please enter a different one for security reasons\n";
                passwordSet = false;
            }else{
                isPasswordSecure(password);
                std::string decision;
                getInputFromUser(decision);
                if(decision == "1"){
                    passwordSet = true;
                } else{
                    passwordSet = false;
                }
            }
        }
    }
    bool isCategoryOkay = false;
    while(!isCategoryOkay) {
        std::cout << ">>Please enter categories from the following: \n";
        printCategory();
        getInputFromUser(category);
        for (const auto &x: categoryList) {
            if (category == x) {
                isCategoryOkay = true;
                break;
            }
        }
        if (!isCategoryOkay) {
            std::cout << "You entered the wrong category, please try again!\n";
        }
    }
    std::cout << ">>Enter page (optional): ";
    getInputFromUser(website);
    std::cout << ">>Enter login (optional): ";
    getInputFromUser(login);
    Password p(name, password, category, website, login);
    passwordList.push_back(p);
}

void PasswordManager::removePassword() {
    std::cout << "\nDo you want to delete 1 or more passwords?\n";
    std::cout << "[1] - 1 password\n";
    std::cout << "[2] - more passwords\n";

    int choise = 0;
    std::cout << ">>Choice: ";
    getInputFromUser(choise);

    if(choise == 1){
        printPasswords();
        int index = 0;
        std::cout << ">>Enter the index of the password to be deleted: [WARNING THIS OPERATION CAUSES IRREVERSIBLE CHANGES IN THE PASSWORD DATABASE] ";
        getInputFromUser(index);
        if(index < passwordList.size()){
            passwordList.erase(passwordList.begin() + index);
        }else{
            std::cout << "\nBledny index\n";
        }

    }else if(choise == 2){
        printPasswords();
        int howManyIndex = 0;
        std::cout << ">>Enter how many passwords you want to delete: [NOTE THIS OPERATION CAUSES IRREVERSIBLE CHANGES IN THE PASSWORD DATABASE] ";
        getInputFromUser(howManyIndex);

        for(int i = 0; i < howManyIndex; i++){
            printPasswords();
            int index = 0;
            std::cout << ">>Enter the index of the password to be removed: ";
            getInputFromUser(index);
            if(index < passwordList.size()){
                passwordList.erase(passwordList.begin() + index);
            }else{
                std::cout << "\nWrong index\n";
            }
        }
    }else{
        std::cout << "\nYou chose the wrong options\n";
    }
}

void PasswordManager::printPasswords() {
    std::cout << "\nList of saved passwords:\n";
    int i = 0;
    for (const Password& p: passwordList) {
        std::cout << i++ << ". " << p.getName() << " | " << p.getPassword() << " | " << p.getCategory()
                  << " | " << p.getWebsite() << " | " << p.getLogin() << '\n';
    }
    std::cout << "LAST PROGRAM MODIFICATION: " << lastTimeStamp << '\n';
}

void PasswordManager::changePassword() {
    std::cout << "\nPassword change\n";
    int counter = 0;
    for(const auto& x : passwordList){
        std::cout << counter++ << ". " << x.getName() << " | " << x.getPassword() << " | " << x.getCategory() << " | " << x.getWebsite() << " | " << x.getLogin() << '\n';
    }
    int index;
    std::cout << "Enter the index of the password to be changed: ";
    getInputFromUser(index);

    if (index < passwordList.size()) {
        std::cout << "\nWhat do you want to change in the record below?\n";
        printParameters();
        int choise = 0;
        getInputFromUser(choise);
        if (choise == 1) {
            std::string newName;
            std::cout << "Enter a new name: ";
            getInputFromUser(newName);
            passwordList[index].setName(newName);
        } else if (choise == 2) {
            bool passwordSet = false;
            std::string newPassword;
            while(!passwordSet) {
                std::cout << ">>Enter the password or generate it automatically by selecting [1] on the keyboard";
                getInputFromUser(newPassword);
                if (newPassword == "1") {
                    newPassword = generatePassword();
                    std::cout << "Your generated password is: " << newPassword << '\n';
                    passwordSet = true;
                } else {
                    if(isPasswordExists(newPassword)){
                        std::cout << "Password used in the past, please enter a different one for security reasons\n";
                        passwordSet = false;
                    }
                }
            }
            passwordList[index].setPassword(newPassword);
        } else if (choise == 3) {
            std::string newCategory;
            std::cout << ">>Enter a new category: ";
            getInputFromUser(newCategory);
            passwordList[index].setCategory(newCategory);
        } else if (choise == 4) {
            std::string newWebsite;
            std::cout << ">>Enter a new page: ";
            getInputFromUser(newWebsite);
            passwordList[index].setWebsite(newWebsite);
        } else if (choise == 5) {
            std::string newLogin;
            std::cout << "Enter a new login: ";
            getInputFromUser(newLogin);
            passwordList[index].setLogin(newLogin);
        } else {
            std::cout << "You chose the wrong number :(\n";
        }
    }else {
        std::cout << "\nThere is no entry with this index.\n";
    }
}


void PasswordManager::searchPassword(){
    std::cout << "Select the parameter you want to search for: \n";
    printParameters();
    std::cout << "Enter the index you want to search by: [ENTER INDEXES AFTER SPACEBAR]";
    std::string index;
    getInputFromUser(index);
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
                std::cout << "Enter the name you are looking for: ";
                getInputFromUser(phraseFromUser);
                for (Password& y: passwordList) {
                    std::string name = y.getName();
                    size_t found = name.find(phraseFromUser);
                    if (found != std::string::npos) {
                        result.insert(y);
                    }
                }
                break;
            case 2:
                std::cout << "Enter the password you are looking for: ";
                getInputFromUser(phraseFromUser);
                for (Password &y: passwordList) {
                    std::string password = y.getPassword();
                    size_t found = password.find(phraseFromUser);
                    if (found != std::string::npos) {
                        result.insert(y);
                    }
                }
                break;
            case 3:
                std::cout << "Enter the categories you are looking for: ";
                getInputFromUser(phraseFromUser);
                for (Password &y: passwordList) {
                    std::string category = y.getCategory();
                    size_t found = category.find(phraseFromUser);
                    if (found != std::string::npos) {
                        result.insert(y);
                    }
                }
                break;
            case 4:
                std::cout << "Enter the website you are looking for: ";
                getInputFromUser(phraseFromUser);
                for (Password &y: passwordList) {
                    std::string website = y.getWebsite();
                    size_t found = website.find(phraseFromUser);
                    if (found != std::string::npos) {
                        result.insert(y);
                    }
                }
                break;
            case 5:
                std::cout << "Enter the login you are looking for: ";
                getInputFromUser(phraseFromUser);
                for (Password &y: passwordList) {
                    std::string login = y.getLogin();
                    size_t found = login.find(phraseFromUser);
                    if (found != std::string::npos) {
                        result.insert(y);
                    }
                }
                break;
            default:
                std::cout << "You entered an invalid index :(\n";
                break;
        }
    }
    for (const auto &x: result) {
        std::cout << ". " << x.getName() << " | " << x.getPassword() << " | " << x.getCategory() << " | "
                  << x.getWebsite() << " | " << x.getLogin() << '\n';
    }
}

void PasswordManager::sortPasswords(){
    std::cout << "Enter sorting parameters:\n";
    printParameters();
    std::vector<int> sortingParams;
    int parameter;
    do {
        std::cout << "Select parameter (0 to exit): ";
        getInputFromUser(parameter);
        if(parameter >= 1 && parameter <= 5){
            sortingParams.push_back(parameter);
        }
    } while (parameter != 0);

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

    std::cout << "[SORTED PASSWORDS]\n";
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
    std::cout << "Here are the categories that already exist: \n";
    printCategory();

    std::string newCategory;
    std::cout << "Enter the name of the category you want to add: ";
    getInputFromUser(newCategory);

    bool categoryExist = false;
    for(const auto& x : categoryList){
        if(newCategory == x){
            categoryExist = true;
            break;
        }
    }
    if(!categoryExist){
        categoryList.push_back(newCategory);
        std::cout << "Category " << newCategory << " has been added!\n";
    }else{
        std::cout << "Can't add a category because it already exists!\n";
    }
}

void PasswordManager::deleteCategory(){
    std::cout << "Below is a list of current categories: \n";
    printCategory();

    int index = 0;
    std::cout << "Enter the index of the category to be removed: [WARNING THIS OPERATION CAUSES IRREVERSIBLE CHANGES IN THE PASSWORD DATABASE] ";
    getInputFromUser(index);

    if(index < categoryList.size()){
        std::string thisCategory = categoryList[index];
        std::cout << thisCategory;
        for(int i = 0; i < passwordList.size(); i++){
            if(thisCategory == passwordList[i].getCategory()){
                passwordList.erase(passwordList.begin() + i);
            }
        }
        categoryList.erase(categoryList.begin() + index);
    }else{
        std::cout << "\nWrong index\n";
    }
}

void PasswordManager::menu() {
    while (true) {
        std::cout << "\n>>- - - - - - - - - - - - - - - - - - - - - - -<<\n";
        std::cout << "SELECT ACTION:\n";
        std::cout << "[1] Display passwords and last modified information\n";
        std::cout << "[2] Add a new password\n";
        std::cout << "[3] Delete password\n";
        std::cout << "[4] Edit password\n";
        std::cout << "[5] Search passwords\n";
        std::cout << "[6] Sort passwords\n";
        std::cout << "[7] Add categories\n";
        std::cout << "[8] Delete categories\n";
        std::cout << "[9] End program\n";
        std::cout << ">>- - - - - - - - - - - - - - - - - - - - - - -<<\n";
        std::cout << "What you choose?: ";
        int choice;
        getInputFromUser(choice);
        switch (choice) {
            case 1:
                printPasswords();
                break;
            case 2:
                addPassword();
                saveToFile();
                break;
            case 3:
                removePassword();
                saveToFile();
                break;
            case 4:
                changePassword();
                saveToFile();
                break;
            case 5:
                searchPassword();
                break;
            case 6:
                sortPasswords();
                saveToFile();
                break;
            case 7:
                addCategory();
                break;
            case 8:
                deleteCategory();
                saveToFile();
                break;
            case 9:
                exit(0);
            default:
                std::cout << "\nInvalid selection.\n";
                break;
        }
    }
}

void PasswordManager::start() {
    std::cout << ">>- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - <<\n";
    auto now = std::chrono::system_clock::now();
    std::time_t c_time = std::chrono::system_clock::to_time_t(now);
    std::cout << "Current time: " << std::ctime(&c_time);

    std::cout << "\nWelcome to Password Manager by Tomasz Plucinski\n";
    std::cout << "Aktualna wersja programu to: 1.1v\n";
    std::cout << ">>- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - <<\n";
    while (true) {
        fileName = enterFileName();
        setFileName(fileName);
        if (fileExists()) {
            passwordToFile = enterPasswordToFile();
            setPasswordToFile(passwordToFile);
            if (loadFromFile()) {
                setTimeStamp(newTimeStamp());
                break;
            }
        }
    }
    menu();
}