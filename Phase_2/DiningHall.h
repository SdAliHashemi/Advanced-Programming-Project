//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_2 : DiningHall.h
//               -----------------------------------------
#pragma once
#include <string>

class DiningHall {
private:
    int hall_id;
    std::string name;
    std::string address;
    int capacity;

public:
    DiningHall();

    void print();

    void set_hall_id(int id);
    void set_name(const std::string& name);
    void set_address(const std::string& address);
    void set_capacity(int capacity);

    int get_hall_id();
    std::string get_name();
    std::string get_address();
    int get_capacity();
};

