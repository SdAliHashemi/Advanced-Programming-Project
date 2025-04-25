//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_1
//               -----------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;


enum MealType { BREAKFAST, LUNCH, DINNER };
enum ReservationStatus { SUCCESS, CANCELLED, FAILED};

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





// ------ MEAL::MEAL ------
Meal::Meal() {
    meal_id = 0;
    name = "";
    price = 0.0;
    meal_type = BREAKFAST;
    side_items.clear();
}
//--------------------------------------
void Meal::print() {
    cout << "Meal ID: "<< meal_id<< endl;
    cout << "Name: " << name << endl;
    cout << "Price: " <<price << endl;
    cout << "Type: ";
    switch (meal_type) {
        case BREAKFAST: cout << "Breakfast"; break;
        case LUNCH: cout << "Lunch"; break;
        case DINNER: cout << "Dinner"; break;
    }
    cout << endl;
    
    cout << "Side Items: ";
    for (const auto& item : side_items){
        cout << item << " ";
    }
    cout << endl;
}
//--------------------------------------
void Meal::set_meal_id(int id) {meal_id = id;}
void Meal::set_name(const string& name) {this->name = name;}
void Meal::set_price(float price) {this->price = price;}
void Meal::set_meal_type(MealType type) {meal_type = type;}
//--------------------------------------
int Meal::get_meal_id() {return meal_id;}
string Meal::get_name() {return name;}
float Meal::get_price() {return price;}
MealType Meal::get_meal_type() {return meal_type;}
vector<string> Meal::get_side_items() {return side_items;}
//--------------------------------------
void Meal::update_price(float new_price) {price = new_price;}
void Meal::add_side_item(const string& item) {side_items.push_back(item);}


// ------ DININGHALL::DININGHALL ------
DiningHall::DiningHall() {
    hall_id = 0;
    name = "";
    address = "";
    capacity = 0;
}
//--------------------------------------
void DiningHall::print() {
    cout << "Dining Hall ID: " << hall_id << endl;
    cout << "Name: " << name << endl;
    cout << "Address: " << address << endl;
    cout << "Capacity: " << capacity << endl;
}

//--------------------------------------
void DiningHall::set_hall_id(int id) { hall_id = id; }
void DiningHall::set_name(const string& name) {this->name = name;}
void DiningHall::set_address(const string& address){this->address = address;}
void DiningHall::set_capacity(int capacity) {this->capacity = capacity;}

//--------------------------------------
int DiningHall::get_hall_id() {return hall_id;}
string DiningHall::get_name() {return name;}
string DiningHall::get_address() {return address;}
int DiningHall::get_capacity() {return capacity;}


// ------ STUDENT::STUDENT ------
Student::Student() {
    user_id = 0;
    student_id = "";
    name = "";
    email = "";
    balance = 0.0;
    is_active = false;
    reservations.clear();
}

//--------------------------------------
void Student::print() {
    cout << "User ID: " << user_id << endl;
    cout << "Student ID: " << student_id << endl;
    cout << "Name: " << name << endl;
    cout << "Email: " << email << endl;
    cout << "Balance: " << balance << endl;
    cout << "Active: " << (is_active ? "Yes" : "No") << endl;

    cout << "Reservations Count: " << reservations.size() << endl;
}
//--------------------------------------
void Student::set_user_id(int id) {user_id = id; }
void Student::set_student_id(const string& id) {student_id = id; }
void Student::set_name(const string& name) {this->name = name; }
void Student::set_email(const string& email) {this->email = email; }
void Student::set_balance(float balance) {this->balance = balance;}
void Student::set_active(bool active) {is_active = active; }
//--------------------------------------
int Student::get_user_id() {return user_id; }
string Student::get_student_id() {return student_id; }
string Student::get_name() { return name;}
string Student::get_email() {return email;}
float Student::get_balance() {return balance;}
bool Student::get_active() { return is_active;}
//--------------------------------------


// ------ RESERVATION::RESERVATION ------
Reservation::Reservation() {
    reservation_id = 0;
    status = FAILED;
    created_at = time(nullptr);
}

//--------------------------------------
void Reservation::print() {
    cout << "Reservation ID: " <<reservation_id << endl;
    cout << "Status: ";
    switch (status) {
        case SUCCESS: cout <<"Success!"; break;
        case CANCELLED: cout << "Cancelled!"; break;
        case FAILED: cout <<"Failed!"; break;
    }
    cout << endl;

    cout << "Created At: "<<ctime(&created_at);

    cout << "--- Student Info ---" << endl;
    student.print();
    cout << "--- Dining Hall ---" << endl;
    dHall.print();
    cout << "--- Meal Info ---" <<endl;
    meal.print();
}
//--------------------------------------
bool Reservation::cancel() {
    if (status == CANCELLED) return false;
    status = CANCELLED;
    return true;
}
//--------------------------------------
void Reservation::set_reservation_id(int id) {reservation_id = id;}
void Reservation::set_student(const Student& student) { this->student = student;}
void Reservation::set_dining_hall(const DiningHall& hall) { dHall = hall;}
void Reservation::set_meal(const Meal& meal) { this->meal = meal; }
void Reservation::set_status(ReservationStatus status) { this->status = status;}
void Reservation::set_created_at(time_t created) { created_at = created;}
//--------------------------------------
int Reservation::get_reservation_id() {return reservation_id;}
Student Reservation::get_student() {return student;}
DiningHall Reservation::get_dining_hall() { return dHall;}
Meal Reservation::get_meal(){ return meal;}
ReservationStatus Reservation::get_status() { return status;}
time_t Reservation::get_created_at() {return created_at;}


//--------------------------------------
bool Student::reserve_meal(const Meal& meal, const DiningHall& hall) {
    if (!is_active) {
        cout << "Reservation failed: student is not active." << endl;
        return false;
    }

    
    if (balance < meal.get_price()) {
        cout << "Reservation failed: insufficient balance."<< endl;
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

//--------------------------------------

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

    cout << "Cancellation failed: reservation not found."<< endl;
    return false;
}












































int main(){
	
	
	
	
	
	
	
	
	
	
	
	return 0;
}
