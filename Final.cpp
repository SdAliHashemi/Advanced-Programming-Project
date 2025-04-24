#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;


enum MealType { BREAKFAST, LUNCH, DINNER };
enum ReservationStatus { SUCCESS, CANCELLED, FAILED };

// -------------------------- Classes! --------------------------

// ------------- Declarations -------------
class Meal;
class DiningHall;
class Student;
class Reservation;

// --------- Meal ---------
class Meal {
private:
    int meal_id;
    string name;
    float price;
    MealType meal_type;
    vector<string> side_items;

public:
    Meal();
    void print();
    void update_price(float new_price);
    void add_side_item(const string& item);

    // getters
    int get_meal_id();
    string get_name();
    float get_price();
    MealType get_meal_type();
    vector<string> get_side_items();

    // setters
    void set_meal_id(int id);
    void set_name(const string& name);
    void set_price(float price);
    void set_meal_type(MealType type);
};


// --------- DiningHall ---------
class DiningHall {
private:
    int hall_id;
    string name;
    string address;
    int capacity;

public:
    DiningHall();
    void print();

    // getters
    int get_hall_id();
    string get_name();
    string get_address();
    int get_capacity();

    // setters
    void set_hall_id(int id);
    void set_name(const string& name);
    void set_address(const string& address);
    void set_capacity(int capacity);
};


// --------- Student ---------
class Student {
private:
    int user_id;
    string student_id;
    string name;
    string email;
    float balance;
    bool is_active;
    vector<Reservation> reservations;

public:
    Student();
    void print();
    bool reserve_meal(const Meal& meal, const DiningHall& hall);
    bool cancel_reservation(int reservation_id);

    // getters
    int get_user_id();
    string get_student_id();
    string get_name();
    string get_email();
    float get_balance();
    bool get_active();

    // setters
    void set_user_id(int id);
    void set_student_id(const string& id);
    void set_name(const string& name);
    void set_email(const string& email);
    void set_balance(float balance);
    void set_active(bool active);
};


// --------- Reservation ---------
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

    // getters
    int get_reservation_id();
    Student get_student();
    DiningHall get_dining_hall();
    Meal get_meal();
    ReservationStatus get_status();
    time_t get_created_at();

    // setters
    void set_reservation_id(int id);
    void set_student(const Student& student);
    void set_dining_hall(const DiningHall& hall);
    void set_meal(const Meal& meal);
    void set_status(ReservationStatus status);
    void set_created_at(time_t created);
};
























































int main(){
	
	
	
	
	
	
	
	
	
	
	
	return 0;
}
