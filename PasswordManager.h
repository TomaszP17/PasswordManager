//
// Created by Tomasz on 02.05.2023.
//

#ifndef MENADZERHASEL_PASSWORDMANAGER_H
#define MENADZERHASEL_PASSWORDMANAGER_H

#include <vector>
#include "Password.h"

class PasswordManager {
private:
    std::vector<std::string> categoryList = {"Internet", "Wideo", "Edukacja"};
    std::string fileName;
    std::vector<Password> passwordList;
    std::string passwordToFile;
    int counterOfResult;
public:
    PasswordManager();
    void setFileName(std::string fileName);
    auto setPasswordToFile(std::string passwordToFile);
    auto enterPasswordToFile() -> std::string;
    auto enterFileName() -> std::string;
    auto checkFileStatus(std::fstream *file) -> bool;
    bool fileExists();
    auto encryptData(std::string name, std::string pass, std::string category, std::string website, std::string login) -> std::vector<std::string>;
    auto decryptData(std::string name, std::string pass, std::string category, std::string website, std::string login) -> std::vector<std::string>;
    bool saveToFile();
    bool loadFromFile();
    bool isPasswordSecure(const std::string& password);
    bool isPasswordExists(const std::string& password);
    std::string generatePassword();
    void addPassword();
    void removePassword();
    void printPasswords();
    void changePassword();
    void searchPassword();
    void sortPasswords();
    void printCategory();
    void addCategory();
    void deleteCategory();
    void menu();
    void start();
};
#endif //MENADZERHASEL_PASSWORDMANAGER_H
