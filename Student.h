//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_1 : Student.h
//               -----------------------------------------
#pragma once
#include <iostream>
#include <string>
#include <vector>

class Reservation;
class Meal;
class DiningHall;

class Student {
private:
    int user_id;
    std::string student_id;
    std::string name;
    std::string email;
    float balance;
    bool is_active;
    std::vector<Reservation> reservations;

public:
    Student();
    void print();
    bool reserve_meal(const Meal& meal, const DiningHall& hall);
    bool cancel_reservation(int reservation_id);

    int get_user_id();
    std::string get_student_id();
    std::string get_name();
    std::string get_email();
    float get_balance();
    bool get_active();

    void set_user_id(int id);
    void set_student_id(const std::string& id);
    void set_name(const std::string& name);
    void set_email(const std::string& email);
    void set_balance(float balance);
    void set_active(bool active);
};

