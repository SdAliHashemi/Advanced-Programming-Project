//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_1 : Reservation.h
//               -----------------------------------------
#pragma once
#include <iostream>
#include <ctime>
#include "Meal.h"
#include "DiningHall.h"
#include "Student.h"

enum ReservationStatus { SUCCESS, CANCELLED, FAILED };

class Reservation {
private:
    int reservation_id;
    Student student;
    DiningHall dHall;
    Meal meal;
    ReservationStatus status;
    time_t created_at;

public:
    Reservation();
    void print();
    bool cancel();

    int get_reservation_id();
    Student get_student();
    DiningHall get_dining_hall();
    Meal get_meal();
    ReservationStatus get_status();
    time_t get_created_at();

    void set_reservation_id(int id);
    void set_student(const Student& student);
    void set_dining_hall(const DiningHall& hall);
    void set_meal(const Meal& meal);
    void set_status(ReservationStatus status);
    void set_created_at(time_t created);
};

