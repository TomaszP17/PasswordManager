//
// Created by Tomasz on 02.05.2023.
//

#include "Password.h"

Password::Password() {}

Password::Password(std::string name, std::string password, std::string category) {
    this->name = name;
    this->password = password;
    this->category = category;
    this->website = "";
    this->login = "";
    this->timestamp = "";
}

Password::Password(std::string name, std::string password, std::string category, std::string website) {
    this->name = name;
    this->password = password;
    this->category = category;
    this->website = website;
    this->login = "";
    this->timestamp = "";
}

Password::Password(std::string name, std::string password, std::string category, std::string website, std::string login) {
    this->name = name;
    this->password = password;
    this->category = category;
    this->website = website;
    this->login = login;
    this->timestamp = "";
}

const std::string& Password::getName() const {
    return name;
}

void Password::setName(const std::string& name) {
    Password::name = name;
}

const std::string& Password::getPassword() const {
    return password;
}

void Password::setPassword(const std::string& password) {
    Password::password = password;
}

const std::string& Password::getCategory() const {
    return category;
}

void Password::setCategory(const std::string& category) {
    Password::category = category;
}

const std::string& Password::getWebsite() const {
    return website;
}

void Password::setWebsite(const std::string& website) {
    Password::website = website;
}

const std::string& Password::getLogin() const {
    return login;
}

void Password::setLogin(const std::string& login) {
    Password::login = login;
}
