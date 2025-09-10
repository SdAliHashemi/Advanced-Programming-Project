//               Seyyed Ali Moaven Hashemi
//               Project Reservation System
//               -----------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

enum ReserveDay { SATURDAY, SUNDAY, MONDAY, TUESDAY, WEDNESDAY };
enum MealType { BREAKFAST, LUNCH, DINNER };
enum RStatus { SUCCESS, FAILED, CANCELLED };

// =========================== Classes! ===========================

// ------------- Declarations -------------
class Meal;
class DiningHall;
class Student;
class Reservation;

// --------- User ---------
class User {
protected:
    int id;
    string name;
    string lastName;
    string email;
    string password;
public:
    User();

    // Getters
    int get_id() const;
    string get_name() const;
    string get_last_name() const;
    string get_email() const;
    string get_password() const;

    virtual void print() const = 0;
    virtual string getType() const = 0;

    // Setters
    void set_id(int _id);
    void set_name(const string& _name);
    void set_last_name(const string& _lastName);
    void set_email(const string& _email);
    void set_password(const string& _password);

    virtual ~User();
};

// --------- Admin ---------
class Admin : public User {
public:
    Admin();

    void print() const override;
    string getType() const override;
};

// --------- Meal ---------
class Meal {
private:
    int meal_id;
    string name;
    float price;
    MealType meal_type;
    vector<string> side_items;
    ReserveDay reserve_day;
    bool isActive;

public:
    Meal();

    void print();
    void update_price(float new_price);
    void add_side_item(const string& item);

    // Setters
    void set_meal_id(int id);
    void set_name(const string& name);
    void set_price(float price);
    void set_meal_type(MealType type);
    void set_reserve_day(ReserveDay day);
    void activate();
    void deactivate();

    // Getters
    int get_meal_id();
    string get_name();
    float get_price();
    MealType get_meal_type();
    vector<string> get_side_items();
    ReserveDay get_reserve_day();
    bool is_active();
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

    // Getters
    int get_hall_id();
    string get_name();
    string get_address();
    int get_capacity();

    // Setters
    void set_hall_id(int id);
    void set_name(const string& name);
    void set_address(const string& address);
    void set_capacity(int capacity);
};

// --------- Student ---------
class Student : public User {
private:
    string student_id;
    string phone;
    bool isActive;
    float balance;
    vector<Reservation> reservations;

public:
    Student();

    // ==== Overrides ====
    void print() const override;
    string getType() const override;

    // ==== Setters ====
    void set_student_id(const string& sid);
    void set_phone(const string& p);
    void activate();
    void deactivate();
    void set_balance(float b);
    // ==== Getters ====
    string get_student_id() const;
    string get_phone() const;
    bool is_active() const;
    vector<Reservation> get_reserves() const;
    float get_balance() const;
    // ==== Actions ====
    bool reserve_meal(Meal* meal, DiningHall* hall);
    bool cancel_reservation(int reservation_id);
};

// --------- Reservation ---------
class Reservation {
private:
    int reservation_id;
    Student* student;
    Meal* meal;
    DiningHall* dHall;
    RStatus status;
    time_t created_at;

public:
    Reservation();

    // Setters
    void set_reservation_id(int id);
    void set_student(Student* s);
    void set_meal(Meal* m);
    void set_dining_hall(DiningHall* d);
    void set_status(RStatus s);
    void set_created_at(time_t t);

    // Getters
    int get_reservation_id();
    Student* get_student();
    Meal* get_meal();
    DiningHall* get_dining_hall();
    RStatus get_status();
    time_t get_created_at();

    // Methods
    bool cancel();
    void print();
};

// --------- Panel (Skeleton) ---------
class Panel {
public:
    void Action(int);
    void showMenu();
    void exit();
};

// --------- Storage (Singleton) ---------
class Storage {
private:
    int _mealIDCounter;
    int _diningHallIDCounter;
    vector<Meal> allMeals;
    vector<DiningHall> allDiningHalls;

    Storage() : _mealIDCounter(0), _diningHallIDCounter(0) {}
    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;

public:
    static Storage& instance() {
        static Storage instance;
        return instance;
    }

    vector<Meal>& getAllMeals() { return allMeals; }
    vector<DiningHall>& getAllDiningHalls() { return allDiningHalls; }

    int nextMealID() { return ++_mealIDCounter; }
    int nextDiningHallID() { return ++_diningHallIDCounter; }
};

//=================================================================================

// ------ USER::USER ------
User::User() : id(0), name(""), lastName(""), email(""), password("") {}
//--------------------------------------
void User::set_id(int _id) { id = _id; }
void User::set_name(const string& _name) { name = _name; }
void User::set_last_name(const string& _lastName) { lastName = _lastName; }
void User::set_email(const string& _email) { email = _email; }
void User::set_password(const string& _password) { password = _password; }
//--------------------------------------
int User::get_id() const { return id; }
string User::get_name() const { return name; }
string User::get_last_name() const { return lastName; }
string User::get_email() const { return email; }
string User::get_password() const { return password; }
//--------------------------------------
User::~User() {}

// ------ ADMIN::ADMIN ------
Admin::Admin() : User() {}
//--------------------------------------
void Admin::print() const {
    cout << "[ADMIN]" << endl;
    cout << "ID: " << id << endl;
    cout << "Name: " << name << " " << lastName << endl;
    cout << "Email: " << email << endl;
}
//--------------------------------------
string Admin::getType() const {
    return "Admin";
}

// ------ MEAL::MEAL ------
Meal::Meal() {
    meal_id = 0;
    name = "";
    price = 0.0;
    meal_type = BREAKFAST;
    reserve_day = SATURDAY;
    isActive = false;
    side_items.clear();
}
//--------------------------------------
void Meal::print() {
    cout << "Meal ID: " << meal_id << endl;
    cout << "Name: " << name << endl;
    cout << "Price: " << price << endl;
    cout << "Type: ";
    switch (meal_type) {
    case BREAKFAST: cout << "Breakfast"; break;
    case LUNCH: cout << "Lunch"; break;
    case DINNER: cout << "Dinner"; break;
    }
    cout << "\nReserve Day: ";
    switch (reserve_day) {
    case SATURDAY: cout << "Saturday"; break;
    case SUNDAY: cout << "Sunday"; break;
    case MONDAY: cout << "Monday"; break;
    case TUESDAY: cout << "Tuesday"; break;
    case WEDNESDAY: cout << "Wednesday"; break;
    }
    cout << "\nStatus: " << (isActive ? "Active" : "Inactive") << endl;

    cout << "Side Items: ";
    for (const auto& item : side_items) {
        cout << item << " ";
    }
    cout << endl;
}
//--------------------------------------
void Meal::update_price(float new_price) { price = new_price; }
void Meal::add_side_item(const string& item) { side_items.push_back(item); }
//--------------------------------------
void Meal::set_meal_id(int id) { meal_id = id; }
void Meal::set_name(const string& name) { this->name = name; }
void Meal::set_price(float price) { this->price = price; }
void Meal::set_meal_type(MealType type) { meal_type = type; }
void Meal::set_reserve_day(ReserveDay day) { reserve_day = day; }
void Meal::activate() { isActive = true; }
void Meal::deactivate() { isActive = false; }
//--------------------------------------
int Meal::get_meal_id() { return meal_id; }
string Meal::get_name() { return name; }
float Meal::get_price() { return price; }
MealType Meal::get_meal_type() { return meal_type; }
vector<string> Meal::get_side_items() { return side_items; }
ReserveDay Meal::get_reserve_day() { return reserve_day; }
bool Meal::is_active() { return isActive; }

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
void DiningHall::set_name(const string& name) { this->name = name; }
void DiningHall::set_address(const string& address) { this->address = address; }
void DiningHall::set_capacity(int capacity) { this->capacity = capacity; }
//--------------------------------------
int DiningHall::get_hall_id() { return hall_id; }
string DiningHall::get_name() { return name; }
string DiningHall::get_address() { return address; }
int DiningHall::get_capacity() { return capacity; }

// ------ STUDENT::STUDENT ------
Student::Student() : User() {
    student_id = "";
    phone = "";
    isActive = false;
    balance = 0.0;
    reservations.clear();
}
//--------------------------------------
void Student::print() const {
    cout << "[STUDENT]" << endl;
    cout << "ID: " << id << endl;
    cout << "Name: " << name << " " << lastName << endl;
    cout << "Email: " << email << endl;
    cout << "Phone: " << phone << endl;
    cout << "Active: " << (isActive ? "Yes" : "No") << endl;
    cout << "Reservations Count: " << reservations.size() << endl;
}
string Student::getType() const {
    return "Student";
}
//--------------------------------------
void Student::set_student_id(const string& sid) { student_id = sid; }
void Student::set_phone(const string& p) { phone = p; }
void Student::activate() { isActive = true; }
void Student::deactivate() { isActive = false; }
void Student::set_balance(float b) { balance = b; }
//--------------------------------------
string Student::get_student_id() const { return student_id; }
string Student::get_phone() const { return phone; }
bool Student::is_active() const { return isActive; }
float Student::get_balance() const { return balance; }
vector<Reservation> Student::get_reserves() const { return reservations; }
//--------------------------------------
bool Student::reserve_meal(Meal* meal, DiningHall* hall) {
    if (!isActive) {
        cout << "Reservation failed: student is not active." << endl;
        return false;
    }

    if (!meal || !hall) {
        cout << "Reservation failed: invalid meal or hall." << endl;
        return false;
    }

    if (balance < meal->get_price()) {
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
                res.get_meal()->get_meal_type() == meal->get_meal_type()) {
                cout << "Reservation failed: already reserved for this meal type today." << endl;
                return false;
            }
        }
    }

    Reservation new_res;
    new_res.set_reservation_id(reservations.size() + 1);
    new_res.set_student(this);
    new_res.set_meal(meal);
    new_res.set_dining_hall(hall);
    new_res.set_status(SUCCESS);
    new_res.set_created_at(now);

    balance -= meal->get_price();
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

            if (res.get_meal()) {
                balance += res.get_meal()->get_price();
            }

            res.cancel();
            cout << "Reservation cancelled successfully. Refund issued." << endl;
            return true;
        }
    }

    cout << "Cancellation failed: reservation not found." << endl;
    return false;
}

// ------ RESERVATION::RESERVATION ------
Reservation::Reservation() {
    reservation_id = 0;
    student = nullptr;
    meal = nullptr;
    dHall = nullptr;
    status = FAILED;
    created_at = time(nullptr);
}
//--------------------------------------
void Reservation::set_reservation_id(int id) { reservation_id = id; }
void Reservation::set_student(Student* s) { student = s; }
void Reservation::set_meal(Meal* m) { meal = m; }
void Reservation::set_dining_hall(DiningHall* d) { dHall = d; }
void Reservation::set_status(RStatus s) { status = s; }
void Reservation::set_created_at(time_t t) { created_at = t; }
//--------------------------------------
int Reservation::get_reservation_id() { return reservation_id; }
Student* Reservation::get_student() { return student; }
Meal* Reservation::get_meal() { return meal; }
DiningHall* Reservation::get_dining_hall() { return dHall; }
RStatus Reservation::get_status() { return status; }
time_t Reservation::get_created_at() { return created_at; }
//--------------------------------------
bool Reservation::cancel() {
    if (status == CANCELLED) return false;
    status = CANCELLED;
    return true;
}
//--------------------------------------
void Reservation::print() {
    cout << "Reservation ID: " << reservation_id << endl;
    cout << "Status: ";
    switch (status) {
    case SUCCESS: cout << "Success"; break;
    case FAILED: cout << "Failed"; break;
    case CANCELLED: cout << "Cancelled"; break;
    }
    cout << endl;

    cout << "Created At: " << ctime(&created_at);
    if (student) {
        cout << "--- Student Info ---" << endl;
        student->print();
    }
    if (dHall) {
        cout << "--- Dining Hall ---" << endl;
        dHall->print();
    }
    if (meal) {
        cout << "--- Meal Info ---" << endl;
        meal->print();
    }
}

int main() {







}
