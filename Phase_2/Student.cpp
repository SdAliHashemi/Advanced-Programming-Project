//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_1 : Student.cpp
//               -----------------------------------------
#include "Student.h"
#include "Reservation.h"
#include "Meal.h"
#include "DiningHall.h"
using namespace std;

Student::Student() {
    user_id = 0;
    student_id = "";
    name = "";
    email = "";
    balance = 0.0;
    is_active = false;
    reservations.clear();
}

void Student::print() {
    cout << "User ID: " << user_id << endl;
    cout << "Student ID: " << student_id << endl;
    cout << "Name: " << name << endl;
    cout << "Email: " << email << endl;
    cout << "Balance: " << balance << endl;
    cout << "Active: " << (is_active ? "Yes" : "No") << endl;
    cout << "Reservations Count: " << reservations.size() << endl;
}

void Student::set_user_id(int id) { user_id = id; }
void Student::set_student_id(const string& id) { student_id = id; }
void Student::set_name(const string& name) { this->name = name; }
void Student::set_email(const string& email) { this->email = email; }
void Student::set_balance(float balance) { this->balance = balance; }
void Student::set_active(bool active) { is_active = active; }

int Student::get_user_id() { return user_id; }
string Student::get_student_id() { return student_id; }
string Student::get_name() { return name; }
string Student::get_email() { return email; }
float Student::get_balance() { return balance; }
bool Student::get_active() { return is_active; }

bool Student::reserve_meal(const Meal& meal, const DiningHall& hall) {
    if (!is_active) {
        cout << "Reservation failed: student is not active." << endl;
        return false;
    }

    if (balance < meal.get_price()) {
        cout << "Reservation failed: insufficient balance." << endl;
        return false;
    }

    time_t now = time(nullptr);
    struct tm* now_tm = localtime(&now);
    for (const auto& res : reservations) {
        if (res.get_status() == SUCCESS) {
            struct tm* res_tm = localtime(&res.get_created_at());
            if (now_tm->tm_year == res_tm->tm_year &&
                now_tm->tm_yday == res_tm->tm_yday &&
                res.get_meal().get_meal_type() == meal.get_meal_type()) {
                cout << "Reservation failed: already reserved for this meal type today." << endl;
                return false;
            }
        }
    }

    Reservation new_res;
    new_res.set_reservation_id(reservations.size() + 1);
    new_res.set_student(*this);
    new_res.set_meal(meal);
    new_res.set_dining_hall(hall);
    new_res.set_status(SUCCESS);
    new_res.set_created_at(now);
    balance -= meal.get_price();
    reservations.push_back(new_res);

    cout << "Reservation successful!" << endl;
    return true;
}

bool Student::cancel_reservation(int reservation_id) {
    for (auto& res : reservations) {
        if (res.get_reservation_id() == reservation_id) {
            if (res.get_status() == CANCELLED) {
                cout << "Cancellation failed: already cancelled." << endl;
                return false;
            }

            float refund = res.get_meal().get_price();
            balance += refund;
            res.cancel();
            cout << "Reservation cancelled successfully. Refund: " << refund << endl;
            return true;
        }
    }

    cout << "Cancellation failed: reservation not found." << endl;
    return false;
}

