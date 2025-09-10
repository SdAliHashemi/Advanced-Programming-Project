//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_2 : Reservation.h
//               -----------------------------------------
#pragma once
#include "Student.h"
#include "Meal.h"
#include "DiningHall.h"
#include <ctime>

enum RStatus { SUCCESS, FAILED, CANCELLED };

class Reservation {
private:
    int reservation_id;
    Student student;
    Meal* meal;
    DiningHall* dHall;
    RStatus status;
    time_t created_at;

public:
    Reservation();

    void set_reservation_id(int id);
    void set_student(const Student& s);
    void set_meal(Meal* m);
    void set_dining_hall(DiningHall* d);
    void set_status(RStatus s);
    void set_created_at(time_t t);

    int get_reservation_id();
    Student get_student();
    Meal* get_meal();
    DiningHall* get_dining_hall();
    RStatus get_status();
    time_t get_created_at();

    bool cancel();
    void print();
};

