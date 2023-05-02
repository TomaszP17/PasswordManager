//
// Created by Tomasz on 02.05.2023.
//

#ifndef MENADZERHASEL_PASSWORD_H
#define MENADZERHASEL_PASSWORD_H


#include <string>

class Password {
private:
    std::string name;
    std::string password;
    std::string category;
    std::string website;
    std::string login;
    std::string timestamp; //obecnie timestamp zawsze jest pusty
public:
    Password();
    Password(std::string name, std::string password, std::string category);
    Password(std::string name, std::string password, std::string category, std::string website);
    Password(std::string name, std::string password, std::string category, std::string website, std::string login);
    const std::string &getName() const;
    void setName(const std::string &name);
    const std::string &getPassword() const;
    void setPassword(const std::string &password);
    const std::string &getCategory() const;
    void setCategory(const std::string &category);
    const std::string &getWebsite() const;
    void setWebsite(const std::string &website);
    const std::string &getLogin() const;
    void setLogin(const std::string &login);
};


#endif //MENADZERHASEL_PASSWORD_H
