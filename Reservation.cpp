//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_1 : Reservation.cpp
//               -----------------------------------------
#include "Reservation.h"
using namespace std;

Reservation::Reservation() {
    reservation_id = 0;
    status = FAILED;
    created_at = time(nullptr);
}

void Reservation::print() {
    cout << "Reservation ID: " << reservation_id << endl;
    cout << "Status: ";
    switch (status) {
    case SUCCESS: cout << "Success!"; break;
    case CANCELLED: cout << "Cancelled!"; break;
    case FAILED: cout << "Failed!"; break;
    }
    cout << endl;

    cout << "Created At: " << ctime(&created_at);
    cout << "--- Student Info ---" << endl;
    student.print();
    cout << "--- Dining Hall ---" << endl;
    dHall.print();
    cout << "--- Meal Info ---" << endl;
    meal.print();
}

bool Reservation::cancel() {
    if (status == CANCELLED) return false;
    status = CANCELLED;
    return true;
}

void Reservation::set_reservation_id(int id) { reservation_id = id; }
void Reservation::set_student(const Student& student) { this->student = student; }
void Reservation::set_dining_hall(const DiningHall& hall) { dHall = hall; }
void Reservation::set_meal(const Meal& meal) { this->meal = meal; }
void Reservation::set_status(ReservationStatus status) { this->status = status; }
void Reservation::set_created_at(time_t created) { created_at = created; }

int Reservation::get_reservation_id() { return reservation_id; }
Student Reservation::get_student() { return student; }
DiningHall Reservation::get_dining_hall() { return dHall; }
Meal Reservation::get_meal() { return meal; }
ReservationStatus Reservation::get_status() { return status; }
time_t Reservation::get_created_at() { return created_at; }

