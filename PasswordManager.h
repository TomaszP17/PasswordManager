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
    int counterOfResult;
public:
    void setFileName(std::string fileName);
    auto setPasswordToFile(std::string passwordToFile);
    auto enterPasswordToFile() -> std::string;
    auto enterFileName() -> std::string;
    auto checkFileStatus(std::fstream *file) -> bool;
    bool fileExists();
    auto encryptData(std::string name, std::string pass, std::string category, std::string website, std::string login) -> std::string;
    auto decryptData(std::string line) -> std::string;
    bool saveToFile();
    bool loadFromFile();
    void isPasswordSecure(const std::string& password);
    bool isPasswordGood(std::string const& chars, bool small, bool big, bool dig, bool spec);
    bool hasUpperCase(const std::string& password);
    bool hasLowerCase(const std::string& password);
    bool hasDigit(const std::string& password);
    bool hasSpecialChar(const std::string& password);
    bool isPasswordExists(const std::string& password);
    std::string generatePassword();
    void addPassword();
    void removePassword();
    void printPasswords();
    void changePassword();
    auto printParameters()->void;
    void searchPassword();
    void sortPasswords();
    void printCategory();
    void addCategory();
    void deleteCategory();
    void menu();
    void start();
};
#endif //MENADZERHASEL_PASSWORDMANAGER_H
