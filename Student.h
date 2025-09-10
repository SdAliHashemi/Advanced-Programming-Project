//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_1 : Student.h
//               -----------------------------------------
#pragma once
#include "User.h"
#include "Reservation.h"
#include <vector>

class Student : public User {
private:
    std::string phone;
    bool isActive;
    std::vector<Reservation> reservations;

public:
    Student();

    void print() const override;
    std::string getType() const override;

    void set_phone(const std::string& p);
    void activate();
    void deactivate();

    std::string get_phone() const;
    bool is_active() const;
    std::vector<Reservation> get_reserves() const;

    bool reserve_meal(Meal* meal, DiningHall* hall);
    bool cancel_reservation(int reservation_id);
};

