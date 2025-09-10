//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_1 : DiningHall.cpp
//               -----------------------------------------
#include "DiningHall.h"
using namespace std;

DiningHall::DiningHall() {
    hall_id = 0;
    name = "";
    address = "";
    capacity = 0;
}

void DiningHall::print() {
    cout << "Dining Hall ID: " << hall_id << endl;
    cout << "Name: " << name << endl;
    cout << "Address: " << address << endl;
    cout << "Capacity: " << capacity << endl;
}

void DiningHall::set_hall_id(int id) { hall_id = id; }
void DiningHall::set_name(const string& name) { this->name = name; }
void DiningHall::set_address(const string& address) { this->address = address; }
void DiningHall::set_capacity(int capacity) { this->capacity = capacity; }

int DiningHall::get_hall_id() { return hall_id; }
string DiningHall::get_name() { return name; }
string DiningHall::get_address() { return address; }
int DiningHall::get_capacity() { return capacity; }

