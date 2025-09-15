//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_4
//               -----------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip> // for setw if needed
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional> // placeholder hashing
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

// ======= Enums =======
enum ReserveDay { SATURDAY, SUNDAY, MONDAY, TUESDAY, WEDNESDAY };
enum MealType { BREAKFAST, LUNCH, DINNER };
enum RStatus { SUCCESS, FAILED, CANCELLED, NOT_PAID };

enum TransactionType { TRANSFER, PAYMENT };
enum TransactionStatus { PENDING, COMPLETED, FAILED_TX };
enum SessionStatus { AUTHENTICATED, ANONYMOUS };

// =========================== Classes! ===========================

// ------------- Declarations -------------
class Meal;
class DiningHall;
class Student;
class Reservation;
class ShoppingCart;
class Transaction;
class SessionBase;

// --------- ConfigPaths (singleton) ---------
class ConfigPaths {
public:
    fs::path c_students;
    fs::path c_admins;
    fs::path c_meals;
    fs::path c_dininghalls;
    fs::path session_dir;
    fs::path t_student_transactions;
    fs::path l_students_log_file;
    fs::path l_admins_log_file;

    static ConfigPaths& instance() {
        static ConfigPaths cfg;
        return cfg;
    }
private:
    ConfigPaths() {
        c_students = "students.csv";
        c_admins = "admins.csv";
        c_meals = "meals.csv";
        c_dininghalls = "dining.csv";
        session_dir = "sessions";
        t_student_transactions = "student_transactions.txt";
        l_students_log_file = "students.log";
        l_admins_log_file = "admins.log";
        // Ensure session dir exists
        if (!fs::exists(session_dir)) {
            fs::create_directories(session_dir);
        }
    }
};

// --------- User ---------
class User {
protected:
    int id;
    string name;
    string lastName;
    string email;
    // note: store only hashed password
    string hashedPassword;
public:
    User();

    // Getters
    int get_id() const;
    string get_name() const;
    string get_last_name() const;
    string get_email() const;
    // legacy-compatible getter (returns hashed)
    string get_password() const;

    virtual void print() const = 0;
    virtual string getType() const = 0;

    // Setters
    void set_id(int _id);
    void set_name(const string& _name);
    void set_last_name(const string& _lastName);
    void set_email(const string& _email);

    // Password utilities (Phase 4)
    // setPassword hashes the plain password and stores the hash
    void setPassword(const string& password);
    // check hashed password against input
    bool checkPassword(const string& password) const;
    // set/get hashed directly
    void setHashedPassword(const string& hash);
    string getHashedPassword() const;

    // maintain legacy names to avoid breaking code
    void set_password(const string& plain) { setPassword(plain); }
    // get_password returns hashed for compatibility
    // (prefer getHashedPassword)
    string get_password_legacy() const { return getHashedPassword(); }

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

    void print() const;
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
    int get_meal_id() const;
    string get_name() const;
    float get_price() const;
    MealType get_meal_type() const;
    vector<string> get_side_items() const;
    ReserveDay get_reserve_day() const;
    bool is_active() const;
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
    void print() const;

    // Getters
    int get_hall_id() const;
    string get_name() const;
    string get_address() const;
    int get_capacity() const;

    // Setters
    void set_hall_id(int id);
    void set_name(const string& name);
    void set_address(const string& address);
    void set_capacity(int capacity);
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
    int get_reservation_id() const;
    Student* get_student() const;
    Meal* get_meal() const;
    DiningHall* get_dining_hall() const;
    RStatus get_status() const;
    time_t get_created_at() const;

    // Methods
    bool cancel();
    void print() const;
};

// --------- Panel (Skeleton) ---------
class Panel {
public:
    void Action(int);
    void showMenu();
    void showStudentInfo();
    void checkBalance();
    void viewReservations();
    void viewShoppingCart();
    void addToShoppingCart();
    void removeShoppingCartItem();
    void increaseBalance();
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

    // management helpers required in Phase 4
    void addMeal(const Meal& m);
    void addDiningHall(const DiningHall& d);
    void removeMeal(int id);
    void removeDiningHall(int id);
    void mealActivation(int id, bool active);
    vector<Meal>::iterator findMeal(int id);
    vector<DiningHall>::iterator findDiningHall(int id);
};

// --------- Transaction ---------
class Transaction {
private:
    int _transactionID;
    string _trackingCode;
    float _amount;
    TransactionType _type;
    TransactionStatus _status;
    time_t _createdAt;

public:
    Transaction();
    // getters
    int get_transaction_id() const;
    string get_tracking_code() const;
    float get_amount() const;
    TransactionType get_type() const;
    TransactionStatus get_status() const;
    time_t get_created_at() const;
    // setters
    void set_transaction_id(int id);
    void set_tracking_code(const string& code);
    void set_amount(float amount);
    void set_type(TransactionType t);
    void set_status(TransactionStatus s);
    void set_created_at(time_t t);

    void print() const;
};

// --------- ShoppingCart ---------
class ShoppingCart {
private:
    vector<Reservation> _reservations;

public:
    ShoppingCart();

    // confirm now checks StudentSession balance and writes transaction log
    Transaction confirm();
    void addReservation(const Reservation& reservation);
    bool removeReservation(int ID);
    void viewShoppingCartItems() const;
    void clear();
    vector<Reservation> getReservations() const;
};

// --------- Sessions - abstract base ---------
class SessionBase {
protected:
    time_t _createdAt;
    time_t _lastTimeLogin;
    SessionStatus _status;

public:
    SessionBase();
    virtual ~SessionBase() = default;

    virtual void load_session() = 0;
    virtual void save_session() = 0;
    virtual void login(const string& identifier, const string& password) = 0;
    virtual void logout() = 0;

    // getters / setters
    time_t get_created_at() const { return _createdAt; }
    time_t get_last_login() const { return _lastTimeLogin; }
    SessionStatus get_status() const { return _status; }
    void set_status(SessionStatus s) { _status = s; }
};

// Namespaced SessionManagers (Singletons)
namespace StudentSession {
    class SessionManager : public SessionBase {
    private:
        Student* _currentStudent;
        ShoppingCart* _shopping_cart;
        int _studentID;

        SessionManager();
        SessionManager(const SessionManager&) = delete;
        SessionManager& operator=(const SessionManager&) = delete;

    public:
        static SessionManager& instance();
        // overrides
        void load_session() override;
        void save_session() override;
        void login(const string& identifier, const string& password) override;
        void logout() override;

        // helpers
        Student* currentStudent() const;
        ShoppingCart* shoppingCart() const;
        // getters
        int get_student_id() const { return _studentID; }

        // setters
        void setCurrentStudent(Student* s) { _currentStudent = s; }
        void setShoppingCart(ShoppingCart* sc) { _shopping_cart = sc; }
        void setStudentID(int id) { _studentID = id; }
    };
} // namespace StudentSession

namespace AdminSession {
    class SessionManager : public SessionBase {
    private:
        Admin* _currentAdmin;
        int _adminID;

        SessionManager();
        AdminSession::SessionManager(const AdminSession::SessionManager&) = delete;
        AdminSession::SessionManager& operator=(const AdminSession::SessionManager&) = delete;

    public:
        static AdminSession::SessionManager& instance();
        // overrides
        void load_session() override;
        void save_session() override;
        void login(const string& identifier, const string& password) override;
        void logout() override;

        Admin* currentAdmin() const;
        int get_admin_id() const { return _adminID; }

        // setters
        void setCurrentAdmin(Admin* a) { _currentAdmin = a; }
        void setAdminID(int id) { _adminID = id; }
    };
} // namespace AdminSession

// --------- AdminPanel ---------
class AdminPanel {
public:
    void chooseCsvFile(const fs::path& p);
    void displayAllMeals();
    void displayAllDiningHalls();
    void addNewMealInteractive();
    void addNewDiningHallInteractive();
    void removeMeal(int id);
    void mealActivation(int id, bool active);
    void removeDiningHall(int id);
    void showMenu();
    void action(int opt);
};

//=================================================================================

// ------ USER::USER ------
User::User() : id(0), name(""), lastName(""), email(""), hashedPassword("") {}
//--------------------------------------
void User::set_id(int _id) { id = _id; }
void User::set_name(const string& _name) { name = _name; }
void User::set_last_name(const string& _lastName) { lastName = _lastName; }
void User::set_email(const string& _email) { email = _email; }
int User::get_id() const { return id; }
string User::get_name() const { return name; }
string User::get_last_name() const { return lastName; }
string User::get_email() const { return email; }
string User::get_password() const { return hashedPassword; }
string User::getHashedPassword() const { return hashedPassword; }
void User::setHashedPassword(const string& hash) { hashedPassword = hash; }
//--------------------------------------
// Placeholder hashing: replace with bcrypt/argon2 in production
void User::setPassword(const string& password) {
    hash<string> hasher;
    hashedPassword = to_string(hasher(password));
}
bool User::checkPassword(const string& password) const {
    hash<string> hasher;
    return hashedPassword == to_string(hasher(password));
}
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
string Admin::getType() const { return "Admin"; }

// ------ MEAL::MEAL ------
Meal::Meal() {
    meal_id = 0;
    name = "";
    price = 0.0f;
    meal_type = BREAKFAST;
    reserve_day = SATURDAY;
    isActive = false;
    side_items.clear();
}
void Meal::print() const {
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
    for (const auto& item : side_items) cout << item << " ";
    cout << endl;
}
void Meal::update_price(float new_price) { price = new_price; }
void Meal::add_side_item(const string& item) { side_items.push_back(item); }
void Meal::set_meal_id(int id) { meal_id = id; }
void Meal::set_name(const string& name) { this->name = name; }
void Meal::set_price(float price) { this->price = price; }
void Meal::set_meal_type(MealType type) { meal_type = type; }
void Meal::set_reserve_day(ReserveDay day) { reserve_day = day; }
void Meal::activate() { isActive = true; }
void Meal::deactivate() { isActive = false; }
int Meal::get_meal_id() const { return meal_id; }
string Meal::get_name() const { return name; }
float Meal::get_price() const { return price; }
MealType Meal::get_meal_type() const { return meal_type; }
vector<string> Meal::get_side_items() const { return side_items; }
ReserveDay Meal::get_reserve_day() const { return reserve_day; }
bool Meal::is_active() const { return isActive; }

// ------ DININGHALL::DININGHALL ------
DiningHall::DiningHall() {
    hall_id = 0;
    name = "";
    address = "";
    capacity = 0;
}
void DiningHall::print() const {
    cout << "Dining Hall ID: " << hall_id << endl;
    cout << "Name: " << name << endl;
    cout << "Address: " << address << endl;
    cout << "Capacity: " << capacity << endl;
}
void DiningHall::set_hall_id(int id) { hall_id = id; }
void DiningHall::set_name(const string& name) { this->name = name; }
void DiningHall::set_address(const string& address) { this->address = address; }
void DiningHall::set_capacity(int capacity) { this->capacity = capacity; }
int DiningHall::get_hall_id() const { return hall_id; }
string DiningHall::get_name() const { return name; }
string DiningHall::get_address() const { return address; }
int DiningHall::get_capacity() const { return capacity; }

// ------ RESERVATION::RESERVATION ------
Reservation::Reservation() {
    reservation_id = 0;
    student = nullptr;
    meal = nullptr;
    dHall = nullptr;
    status = NOT_PAID;
    created_at = time(nullptr);
}
void Reservation::set_reservation_id(int id) { reservation_id = id; }
void Reservation::set_student(Student* s) { student = s; }
void Reservation::set_meal(Meal* m) { meal = m; }
void Reservation::set_dining_hall(DiningHall* d) { dHall = d; }
void Reservation::set_status(RStatus s) { status = s; }
void Reservation::set_created_at(time_t t) { created_at = t; }
int Reservation::get_reservation_id() const { return reservation_id; }
Student* Reservation::get_student() const { return student; }
Meal* Reservation::get_meal() const { return meal; }
DiningHall* Reservation::get_dining_hall() const { return dHall; }
RStatus Reservation::get_status() const { return status; }
time_t Reservation::get_created_at() const { return created_at; }
bool Reservation::cancel() {
    if (status == CANCELLED) return false;
    status = CANCELLED;
    return true;
}
void Reservation::print() const {
    cout << "Reservation ID: " << reservation_id << endl;
    cout << "Status: ";
    switch (status) {
    case SUCCESS: cout << "Success"; break;
    case FAILED: cout << "Failed"; break;
    case CANCELLED: cout << "Cancelled"; break;
    case NOT_PAID: cout << "Not Paid"; break;
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

// ------ STUDENT::STUDENT ------
Student::Student() : User() {
    // Student-specific initializations
    // note: User ctor already ran
    // student_id & phone are part of Student
    // default values:
    // student_id = ""; phone = ""; isActive=false; balance=0; reservations cleared
    // We'll set them explicitly if needed in implementation section (below)
}
void Student::print() const {
    cout << "[STUDENT]" << endl;
    cout << "ID: " << id << endl;
    cout << "Name: " << name << " " << lastName << endl;
    cout << "Email: " << email << endl;
    cout << "Phone: " << phone << endl;
    cout << "Active: " << (isActive ? "Yes" : "No") << endl;
    cout << "Reservations Count: " << reservations.size() << endl;
}
// getters/setters & actions implemented below (kept original style)

// (We'll re-declare Student's members and methods to match previous file
//  because in the original long file Student had its own private fields.)
// To maintain the exact layout of your original file, put full Student definition here:

// Rewriting Student class (to match your original longer version)
class StudentFull : public User {
private:
    string student_id;
    string phone;
    bool isActive;
    float balance;
    vector<Reservation> reservations;

public:
    StudentFull();
    // overrides
    void print() const override;
    string getType() const override;

    // setters
    void set_student_id(const string& sid);
    void set_phone(const string& p);
    void activate();
    void deactivate();
    void set_balance(float b);

    // getters
    string get_student_id() const;
    string get_phone() const;
    bool is_active() const;
    vector<Reservation> get_reserves() const;
    float get_balance() const;

    // actions
    bool reserve_meal(Meal* meal, DiningHall* hall);
    bool cancel_reservation(int reservation_id);
};

// For backwards-compatibility with rest of original code, we'll alias Student to StudentFull
// (so other code that refers to class Student will still compile)
typedef StudentFull Student;

// Implement StudentFull methods (keeping same logic as your original code)
StudentFull::StudentFull() : User() {
    student_id = "";
    phone = "";
    isActive = false;
    balance = 0.0f;
    reservations.clear();
}
void StudentFull::print() const {
    cout << "[STUDENT]" << endl;
    cout << "ID: " << id << endl;
    cout << "Name: " << name << " " << lastName << endl;
    cout << "Email: " << email << endl;
    cout << "Phone: " << phone << endl;
    cout << "Active: " << (isActive ? "Yes" : "No") << endl;
    cout << "Reservations Count: " << reservations.size() << endl;
}
string StudentFull::getType() const { return "Student"; }
void StudentFull::set_student_id(const string& sid) { student_id = sid; }
void StudentFull::set_phone(const string& p) { phone = p; }
void StudentFull::activate() { isActive = true; }
void StudentFull::deactivate() { isActive = false; }
void StudentFull::set_balance(float b) { balance = b; }
string StudentFull::get_student_id() const { return student_id; }
string StudentFull::get_phone() const { return phone; }
bool StudentFull::is_active() const { return isActive; }
float StudentFull::get_balance() const { return balance; }
vector<Reservation> StudentFull::get_reserves() const { return reservations; }

bool StudentFull::reserve_meal(Meal* meal, DiningHall* hall) {
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
    new_res.set_reservation_id(static_cast<int>(reservations.size() + 1));
    new_res.set_student(this);
    new_res.set_meal(meal);
    new_res.set_dining_hall(hall);
    // per Phase 3/4, when created via reserve_meal we can set SUCCESS directly (assuming payment)
    new_res.set_status(SUCCESS);
    new_res.set_created_at(now);
    balance -= meal->get_price();
    reservations.push_back(new_res);
    cout << "Reservation successful!" << endl;
    return true;
}

bool StudentFull::cancel_reservation(int reservation_id) {
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

// ------ TRANSACTION::TRANSACTION ------
Transaction::Transaction() {
    _transactionID = 0;
    _trackingCode = "";
    _amount = 0.0f;
    _type = PAYMENT;
    _status = PENDING;
    _createdAt = time(nullptr);
}
int Transaction::get_transaction_id() const { return _transactionID; }
string Transaction::get_tracking_code() const { return _trackingCode; }
float Transaction::get_amount() const { return _amount; }
TransactionType Transaction::get_type() const { return _type; }
TransactionStatus Transaction::get_status() const { return _status; }
time_t Transaction::get_created_at() const { return _createdAt; }
void Transaction::set_transaction_id(int id) { _transactionID = id; }
void Transaction::set_tracking_code(const string& code) { _trackingCode = code; }
void Transaction::set_amount(float amount) { _amount = amount; }
void Transaction::set_type(TransactionType t) { _type = t; }
void Transaction::set_status(TransactionStatus s) { _status = s; }
void Transaction::set_created_at(time_t t) { _createdAt = t; }
void Transaction::print() const {
    cout << "Transaction ID: " << _transactionID << endl;
    cout << "Tracking: " << _trackingCode << endl;
    cout << "Amount: " << _amount << endl;
    cout << "Status: ";
    switch (_status) {
    case PENDING: cout << "Pending"; break;
    case COMPLETED: cout << "Completed"; break;
    case FAILED_TX: cout << "Failed"; break;
    }
    cout << endl;
    cout << "Created At: " << ctime(&_createdAt);
}

// ------ SHOPPINGCART::SHOPPINGCART ------
ShoppingCart::ShoppingCart() {
    _reservations.clear();
}

Transaction ShoppingCart::confirm() {
    Transaction tx;
    // generate simple id/time-based
    int txid = static_cast<int>(time(nullptr) % 1000000);
    tx.set_transaction_id(txid);
    tx.set_tracking_code("TRK" + to_string(tx.get_transaction_id()));

    float sum = 0.0f;
    for (auto& r : _reservations) {
        if (r.get_meal()) sum += r.get_meal()->get_price();
    }
    tx.set_amount(sum);
    tx.set_type(PAYMENT);
    tx.set_status(PENDING);
    tx.set_created_at(time(nullptr));

    // If a student session exists, try to deduct balance
    Student* cur = StudentSession::SessionManager::instance().currentStudent();
    if (cur) {
        float bal = cur->get_balance();
        if (sum <= 0.0f) {
            tx.set_status(FAILED_TX);
        }
        else if (bal < sum) {
            tx.set_status(FAILED_TX);
            // leave reservations as NOT_PAID
            cout << "Confirm failed: student's balance insufficient." << endl;
        }
        else {
            // deduct and confirm
            cur->set_balance(bal - sum);
            tx.set_status(COMPLETED);
            for (auto& r : _reservations) {
                r.set_status(SUCCESS);
                // optionally add reservation to student's reservation list
                // cur->get_reserves() returns a copy; to push into student's reservations we need access
                // We'll assume Student has method to add reservation in future; for now we leave as-is.
            }
            cout << "Confirm succeeded: amount deducted from student." << endl;
        }
        // append transaction to a transaction log for student
        ofstream fout(ConfigPaths::instance().t_student_transactions, ios::app);
        if (fout.is_open()) {
            fout << "tx_id:" << tx.get_transaction_id()
                << ",student_id:" << cur->get_id()
                << ",amount:" << tx.get_amount()
                << ",status:" << (tx.get_status() == COMPLETED ? "COMPLETED" : "FAILED")
                << ",time:" << tx.get_created_at() << "\n";
            fout.close();
        }
    }
    else {
        // no logged student: mark NOT_PAID or FAILED depending on sum
        if (sum <= 0.0f) tx.set_status(FAILED_TX);
        else tx.set_status(FAILED_TX); // require a payment flow
        cout << "Confirm: no student session - transaction marked failed (no payer)." << endl;
    }
    return tx;
}

void ShoppingCart::addReservation(const Reservation& reservation) {
    Reservation r = reservation;
    r.set_status(NOT_PAID);
    _reservations.push_back(r);
}

bool ShoppingCart::removeReservation(int ID) {
    for (auto it = _reservations.begin(); it != _reservations.end(); ++it) {
        if (it->get_reservation_id() == ID) {
            _reservations.erase(it);
            return true;
        }
    }
    return false;
}

void ShoppingCart::viewShoppingCartItems() const {
    cout << "---- Shopping Cart Items (" << _reservations.size() << ") ----" << endl;
    for (const auto& r : _reservations) {
        r.print();
        cout << "------------------" << endl;
    }
}

void ShoppingCart::clear() {
    _reservations.clear();
}

vector<Reservation> ShoppingCart::getReservations() const {
    return _reservations;
}

// ------ SESSIONS: SessionBase ------
SessionBase::SessionBase() {
    _createdAt = time(nullptr);
    _lastTimeLogin = 0;
    _status = ANONYMOUS;
}

// ------ StudentSession::SessionManager ------
namespace StudentSession {
    SessionManager::SessionManager() : _currentStudent(nullptr), _shopping_cart(nullptr), _studentID(0) {
        _shopping_cart = new ShoppingCart();
    }
    SessionManager& SessionManager::instance() {
        static SessionManager inst;
        return inst;
    }

    // Basic CSV parsing helper
    static vector<string> split_csv_line(const string& line) {
        vector<string> cols;
        string cur;
        bool in_quotes = false;
        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (c == '"') {
                in_quotes = !in_quotes;
            }
            else if (c == ',' && !in_quotes) {
                cols.push_back(cur);
                cur.clear();
            }
            else {
                cur.push_back(c);
            }
        }
        cols.push_back(cur);
        return cols;
    }

    void SessionManager::load_session() {
        // Attempt to load last session file for students (simple approach)
        fs::path dir = ConfigPaths::instance().session_dir;
        if (!fs::exists(dir)) return;
        // look for a file like student_{id}.session (pick first)
        for (auto& p : fs::directory_iterator(dir)) {
            string filename = p.path().filename().string();
            if (filename.rfind("student_", 0) == 0) {
                // open and read basic info
                ifstream fin(p.path());
                if (fin.is_open()) {
                    string line;
                    while (getline(fin, line)) {
                        // expect key:value per line
                        if (line.rfind("student_id:", 0) == 0) {
                            int sid = stoi(line.substr(11));
                            _studentID = sid;
                            // we won't reconstruct full student here; login handles that
                        }
                    }
                    fin.close();
                }
                break;
            }
        }
    }

    void SessionManager::save_session() {
        if (!_currentStudent) return;
        fs::path dir = ConfigPaths::instance().session_dir;
        if (!fs::exists(dir)) fs::create_directories(dir);
        string fname = "student_" + to_string(_currentStudent->get_id()) + ".session";
        fs::path fp = dir / fname;
        ofstream fout(fp, ios::trunc);
        if (!fout.is_open()) return;
        fout << "student_id:" << _currentStudent->get_id() << "\n";
        fout << "last_login:" << time(nullptr) << "\n";
        fout.close();
    }

    void SessionManager::login(const string& identifier, const string& password) {
        // Attempt to find student in CSV by identifier (try email or student_id)
        fs::path students_csv = ConfigPaths::instance().c_students;
        ifstream fin(students_csv);
        if (!fin.is_open()) {
            cout << "Student CSV not found at " << students_csv << " — cannot login." << endl;
            return;
        }

        string header;
        if (!getline(fin, header)) {
            cout << "Student CSV empty." << endl;
            fin.close();
            return;
        }

        // We'll try each line; flexible mapping: assume columns:
        // student_id,name,email,phone,balance,hashed_password (but if missing, we adapt)
        string line;
        bool found = false;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            vector<string> cols = split_csv_line(line);
            // trim spaces
            for (auto& c : cols) if (!c.empty() && c.front() == ' ') c.erase(0, c.find_first_not_of(' '));
            // search identifier among likely columns
            bool id_match = false;
            if (!cols.empty() && cols[0] == identifier) id_match = true; // student_id
            if (cols.size() >= 3 && cols[2] == identifier) id_match = true; // email
            if (!id_match) continue;

            // create Student and populate available fields
            Student* s = new Student();
            // student id
            if (cols.size() >= 1) {
                // if student id is numeric, set User::id to numeric
                try {
                    int nid = stoi(cols[0]);
                    s->set_id(nid);
                }
                catch (...) {
                    // non-numeric student_id: leave User::id as 0
                }
                s->set_student_id(cols[0]);
            }
            if (cols.size() >= 2) {
                // name
                s->set_name(cols[1]);
            }
            if (cols.size() >= 3) {
                s->set_email(cols[2]);
            }
            if (cols.size() >= 4) {
                s->set_phone(cols[3]);
            }
            if (cols.size() >= 5) {
                try { s->set_balance(stof(cols[4])); }
                catch (...) {}
            }
            if (cols.size() >= 6) {
                // assume it's a hashed password
                s->setHashedPassword(cols[5]);
                // verify password if provided
                if (!password.empty()) {
                    if (!s->checkPassword(password)) {
                        cout << "Student login failed: password mismatch." << endl;
                        delete s;
                        fin.close();
                        return;
                    }
                }
            }
            else {
                // no hashed password in csv: accept login (or you may require specific logic)
                // NOTE: for security, prefer csv to contain hashed password.
            }

            // found and set
            _currentStudent = s;
            _studentID = s->get_id();
            _status = AUTHENTICATED;
            _lastTimeLogin = time(nullptr);
            cout << "StudentSession: logged in (" << identifier << ")" << endl;
            found = true;
            break;
        }
        fin.close();
        if (!found) {
            cout << "Student not found in CSV." << endl;
        }
    }

    void SessionManager::logout() {
        if (_currentStudent) {
            // save session before logout
            save_session();
            delete _currentStudent;
            _currentStudent = nullptr;
        }
        _status = ANONYMOUS;
        _lastTimeLogin = time(nullptr);
        // clear shopping cart if needed
        if (_shopping_cart) {
            _shopping_cart->clear();
        }
        cout << "StudentSession: logged out" << endl;
    }

    Student* SessionManager::currentStudent() const {
        return _currentStudent;
    }
    ShoppingCart* SessionManager::shoppingCart() const {
        return _shopping_cart;
    }
} // namespace StudentSession

// ------ AdminSession::SessionManager ------
namespace AdminSession {
    SessionManager::SessionManager() : _currentAdmin(nullptr), _adminID(0) {}
    AdminSession::SessionManager& SessionManager::instance() {
        static AdminSession::SessionManager inst;
        return inst;
    }

    void SessionManager::load_session() {
        // similar idea to student sessions; not fully required
    }
    void SessionManager::save_session() {
        // save admin session info to file if needed
        if (!_currentAdmin) return;
        fs::path dir = ConfigPaths::instance().session_dir;
        if (!fs::exists(dir)) fs::create_directories(dir);
        string fname = "admin_" + to_string(_currentAdmin->get_id()) + ".session";
        fs::path fp = dir / fname;
        ofstream fout(fp, ios::trunc);
        if (!fout.is_open()) return;
        fout << "admin_id:" << _currentAdmin->get_id() << "\n";
        fout << "last_login:" << time(nullptr) << "\n";
        fout.close();
    }

    void SessionManager::login(const string& identifier, const string& password) {
        // read admins CSV and attempt to match
        fs::path admins_csv = ConfigPaths::instance().c_admins;
        ifstream fin(admins_csv);
        if (!fin.is_open()) {
            cout << "Admin CSV not found at " << admins_csv << " — cannot login admin." << endl;
            return;
        }
        string header;
        getline(fin, header);
        string line;
        bool found = false;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            // simple split
            vector<string> cols;
            stringstream ss(line);
            string item;
            while (getline(ss, item, ',')) cols.push_back(item);
            // assume columns: id,name,lastName,email,hashedPassword
            if (cols.size() >= 4) {
                if (cols[3] == identifier) {
                    Admin* a = new Admin();
                    try { a->set_id(stoi(cols[0])); }
                    catch (...) {}
                    a->set_name(cols[1]);
                    a->set_last_name(cols[2]);
                    a->set_email(cols[3]);
                    if (cols.size() >= 5) {
                        a->setHashedPassword(cols[4]);
                        if (!password.empty() && !a->checkPassword(password)) {
                            delete a;
                            cout << "Admin login failed: password mismatch." << endl;
                            fin.close();
                            return;
                        }
                    }
                    _currentAdmin = a;
                    _adminID = a->get_id();
                    _status = AUTHENTICATED;
                    _lastTimeLogin = time(nullptr);
                    cout << "AdminSession: logged in (" << identifier << ")" << endl;
                    found = true;
                    break;
                }
            }
        }
        fin.close();
        if (!found) cout << "Admin not found in CSV." << endl;
    }

    void SessionManager::logout() {
        if (_currentAdmin) {
            save_session();
            delete _currentAdmin;
            _currentAdmin = nullptr;
        }
        _status = ANONYMOUS;
        _lastTimeLogin = time(nullptr);
        cout << "AdminSession: logged out" << endl;
    }

    Admin* SessionManager::currentAdmin() const {
        return _currentAdmin;
    }
} // namespace AdminSession

// ------ STORAGE methods ------
void Storage::addMeal(const Meal& m) {
    allMeals.push_back(m);
}
void Storage::addDiningHall(const DiningHall& d) {
    allDiningHalls.push_back(d);
}
void Storage::removeMeal(int id) {
    allMeals.erase(remove_if(allMeals.begin(), allMeals.end(),
        [id](const Meal& m) { return m.get_meal_id() == id; }), allMeals.end());
}
void Storage::removeDiningHall(int id) {
    allDiningHalls.erase(remove_if(allDiningHalls.begin(), allDiningHalls.end(),
        [id](const DiningHall& d) { return d.get_hall_id() == id; }), allDiningHalls.end());
}
void Storage::mealActivation(int id, bool active) {
    for (auto& m : allMeals) if (m.get_meal_id() == id) {
        if (active) m.activate(); else m.deactivate();
    }
}
vector<Meal>::iterator Storage::findMeal(int id) {
    return find_if(allMeals.begin(), allMeals.end(),
        [id](const Meal& m) { return m.get_meal_id() == id; });
}
vector<DiningHall>::iterator Storage::findDiningHall(int id) {
    return find_if(allDiningHalls.begin(), allDiningHalls.end(),
        [id](const DiningHall& d) { return d.get_hall_id() == id; });
}

// ------ ADMINPANEL ------
void AdminPanel::chooseCsvFile(const fs::path& p) {
    cout << "Admin selected CSV file: " << p << endl;
}
void AdminPanel::displayAllMeals() {
    for (auto& m : Storage::instance().getAllMeals()) m.print();
}
void AdminPanel::displayAllDiningHalls() {
    for (auto& d : Storage::instance().getAllDiningHalls()) d.print();
}
void AdminPanel::addNewMealInteractive() {
    // skeleton: interactive prompt to add meal (not mandatory)
    cout << "Add new meal interactive (not implemented)." << endl;
}
void AdminPanel::addNewDiningHallInteractive() {
    cout << "Add new dining hall interactive (not implemented)." << endl;
}
void AdminPanel::removeMeal(int id) { Storage::instance().removeMeal(id); }
void AdminPanel::mealActivation(int id, bool active) { Storage::instance().mealActivation(id, active); }
void AdminPanel::removeDiningHall(int id) { Storage::instance().removeDiningHall(id); }
void AdminPanel::showMenu() { cout << "AdminPanel menu (not implemented)." << endl; }
void AdminPanel::action(int opt) { cout << "AdminPanel action " << opt << " (not implemented)." << endl; }

// ------ PANEL methods (empty skeletons) ------
void Panel::Action(int) { /* stub */ }
void Panel::showMenu() { /* stub */ }
void Panel::showStudentInfo() { /* stub */ }
void Panel::checkBalance() { /* stub */ }
void Panel::viewReservations() { /* stub */ }
void Panel::viewShoppingCart() { /* stub */ }
void Panel::addToShoppingCart() { /* stub */ }
void Panel::removeShoppingCartItem() { /* stub */ }
void Panel::increaseBalance() { /* stub */ }
void Panel::exit() { /* stub */ }

//=================================================================================

int main() {
    // quick sanity test for Phase 4 changes

    // prepare ConfigPaths (defaults already set)
    cout << "Config students csv: " << ConfigPaths::instance().c_students << endl;

    // seed storage
    Meal m;
    m.set_meal_id(1);
    m.set_name("Pasta");
    m.set_price(30.0f);
    m.activate();
    Storage::instance().addMeal(m);

    DiningHall d;
    d.set_hall_id(1);
    d.set_name("Main Dining");
    d.set_capacity(200);
    Storage::instance().addDiningHall(d);

    // Simulate a student record in CSV for test (you said you have CSV; this is just demo)
    // NOTE: In real use you won't write CSV here; keep for local quick test only.
    {
        ofstream fout(ConfigPaths::instance().c_students, ios::trunc);
        fout << "student_id,name,email,phone,balance,hashed_password\n";
        // password "pass123" hashed with std::hash placeholder
        hash<string> hasher;
        fout << "1,Ali,ali@test.com,0912000000,100," << to_string(hasher("pass123")) << "\n";
        fout.close();
    }

    // login student via StudentSession (will check CSV)
    StudentSession::SessionManager::instance().login("ali@test.com", "pass123");
    Student* cur = StudentSession::SessionManager::instance().currentStudent();
    if (cur) {
        cout << "Logged student: " << cur->get_name() << " balance: " << cur->get_balance() << endl;
        // create reservation and push to shopping cart
        Reservation r;
        r.set_reservation_id(1);
        r.set_student(cur);
        r.set_meal(&m);
        r.set_dining_hall(&d);
        r.set_status(NOT_PAID);
        r.set_created_at(time(nullptr));
        StudentSession::SessionManager::instance().shoppingCart()->addReservation(r);
        StudentSession::SessionManager::instance().shoppingCart()->viewShoppingCartItems();

        // confirm
        Transaction tx = StudentSession::SessionManager::instance().shoppingCart()->confirm();
        tx.print();

        // save session
        StudentSession::SessionManager::instance().save_session();

        // logout
        StudentSession::SessionManager::instance().logout();
    }

    cout << "Phase 4 integrated test complete." << endl;
    return 0;
}
