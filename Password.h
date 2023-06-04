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
public:
    Password();
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
    bool operator<(const Password& other) const{
        if(name != getName()) {
            return name < other.getName();
        }else if (password != getPassword()){
            return password < other.getPassword();
        }else if(category != other.getCategory()){
            return category < other.getCategory();
        }else if(website != other.getWebsite()){
            return category < other.getWebsite();
        } else{
            return login < other.getLogin();
        }
    }
};


#endif //MENADZERHASEL_PASSWORD_H
