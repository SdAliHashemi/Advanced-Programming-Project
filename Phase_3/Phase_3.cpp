//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_3
//               -----------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip> // for setw if needed
using namespace std;

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
    };
} // namespace AdminSession

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
    price = 0.0f;
    meal_type = BREAKFAST;
    reserve_day = SATURDAY;
    isActive = false;
    side_items.clear();
}
//--------------------------------------
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
//--------------------------------------
void DiningHall::print() const {
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
//--------------------------------------
void Reservation::set_reservation_id(int id) { reservation_id = id; }
void Reservation::set_student(Student* s) { student = s; }
void Reservation::set_meal(Meal* m) { meal = m; }
void Reservation::set_dining_hall(DiningHall* d) { dHall = d; }
void Reservation::set_status(RStatus s) { status = s; }
void Reservation::set_created_at(time_t t) { created_at = t; }
//--------------------------------------
int Reservation::get_reservation_id() const { return reservation_id; }
Student* Reservation::get_student() const { return student; }
Meal* Reservation::get_meal() const { return meal; }
DiningHall* Reservation::get_dining_hall() const { return dHall; }
RStatus Reservation::get_status() const { return status; }
time_t Reservation::get_created_at() const { return created_at; }
//--------------------------------------
bool Reservation::cancel() {
    if (status == CANCELLED) return false;
    status = CANCELLED;
    return true;
}
//--------------------------------------
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
    student_id = "";
    phone = "";
    isActive = false;
    balance = 0.0f;
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
    new_res.set_reservation_id(static_cast<int>(reservations.size() + 1));
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

// ------ TRANSACTION::TRANSACTION ------
Transaction::Transaction() {
    _transactionID = 0;
    _trackingCode = "";
    _amount = 0.0f;
    _type = PAYMENT;
    _status = PENDING;
    _createdAt = time(nullptr);
}
// getters
int Transaction::get_transaction_id() const { return _transactionID; }
string Transaction::get_tracking_code() const { return _trackingCode; }
float Transaction::get_amount() const { return _amount; }
TransactionType Transaction::get_type() const { return _type; }
TransactionStatus Transaction::get_status() const { return _status; }
time_t Transaction::get_created_at() const { return _createdAt; }
// setters
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
    tx.set_transaction_id(static_cast<int>(time(nullptr) % 100000)); 

    tx.set_tracking_code("TRK" + to_string(tx.get_transaction_id()));
    float sum = 0.0f;
    for (auto& r : _reservations) {
        if (r.get_meal()) sum += r.get_meal()->get_price();
    }
    tx.set_amount(sum);
    tx.set_type(PAYMENT);
    tx.set_status(PENDING);
    tx.set_created_at(time(nullptr));

    if (sum <= 0.0f) {
        tx.set_status(FAILED_TX);
    }
    else {
        tx.set_status(COMPLETED);
        for (auto& r : _reservations) {
            r.set_status(SUCCESS);
        }
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
    void SessionManager::load_session() {
        // stub: load saved session (not required to implement fully)
    }
    void SessionManager::save_session() {
        // stub: save session
    }
    void SessionManager::login(const string& identifier, const string& password) {
        // simple stub: in real project lookup student by identifier
        // here we just mark session authenticated
        _status = AUTHENTICATED;
        _lastTimeLogin = time(nullptr);
        // _currentStudent remains nullptr unless associated externally
        cout << "StudentSession: logged in (" << identifier << ")" << endl;
    }
    void SessionManager::logout() {
        _status = ANONYMOUS;
        _lastTimeLogin = time(nullptr);
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
        // stub
    }
    void SessionManager::save_session() {
        // stub
    }
    void SessionManager::login(const string& identifier, const string& password) {
        _status = AUTHENTICATED;
        _lastTimeLogin = time(nullptr);
        cout << "AdminSession: logged in (" << identifier << ")" << endl;
    }
    void SessionManager::logout() {
        _status = ANONYMOUS;
        _lastTimeLogin = time(nullptr);
        cout << "AdminSession: logged out" << endl;
    }
    Admin* SessionManager::currentAdmin() const {
        return _currentAdmin;
    }
} // namespace AdminSession

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
    
    Meal m;
    m.set_meal_id(1);
    m.set_name("Pasta");
    m.set_price(30.0f);
    m.activate();

    DiningHall d;
    d.set_hall_id(1);
    d.set_name("Main Dining");
    d.set_capacity(200);

    Student s;
    s.set_id(1);
    s.set_name("Ali");
    s.set_last_name("Hashemi");
    s.set_email("ali@test.com");
    s.set_phone("0912000000");
    s.activate();
    s.set_balance(100.0f);

    Reservation r;
    r.set_reservation_id(1);
    r.set_student(&s);
    r.set_meal(&m);
    r.set_dining_hall(&d);
    r.set_status(NOT_PAID);
    r.set_created_at(time(nullptr));

    ShoppingCart cart;
    cart.addReservation(r);

    cart.viewShoppingCartItems();

    Transaction tx = cart.confirm();
    tx.print();

    cart.viewShoppingCartItems();

    StudentSession::SessionManager::instance().login("ali@test.com", "password");
    StudentSession::SessionManager::instance().shoppingCart()->addReservation(r);
    StudentSession::SessionManager::instance().logout();

    return 0;
}
