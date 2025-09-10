//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_2 : User.h
//               -----------------------------------------
#pragma once
#include <string>

class User {
protected:
    int id;
    std::string name;
    std::string email;
    std::string password;

public:
    User();

    int get_id() const;
    std::string get_name() const;
    std::string get_email() const;
    std::string get_password() const;

    virtual void print() const = 0;
    virtual std::string getType() const = 0;

    void set_id(int _id);
    void set_name(const std::string& _name);
    void set_email(const std::string& _email);
    void set_password(const std::string& _password);

    virtual ~User();
};


