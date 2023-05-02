//
// Created by Tomasz on 02.05.2023.
//

#ifndef MENADZERHASEL_PASSWORDMANAGER_H
#define MENADZERHASEL_PASSWORDMANAGER_H


#include <vector>
#include "Password.h"

class PasswordManager {
private:
    std::string fileName;
    std::vector<Password> passwordList;
public:
    PasswordManager();
    //PasswordManager(std::string fileName);
    void setFileName(std::string fileName);
    auto enterFileName() -> std::string;
    auto checkFileStatus(std::fstream *file) -> bool;
    bool fileExists();
    bool createFile();
    bool saveToFile();
    bool loadFromFile();
    void addPassword();
    void removePassword();
    void printPasswords();
    void changePassword();
    void searchPassword();
    void sortPasswords();
    void menu();
    void start();

};


#endif //MENADZERHASEL_PASSWORDMANAGER_H
