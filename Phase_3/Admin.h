//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_2 : Admin.h
//               -----------------------------------------
#pragma once
#include "User.h"

class Admin : public User {
public:
    Admin();
    void print() const override;
    std::string getType() const override;
};


