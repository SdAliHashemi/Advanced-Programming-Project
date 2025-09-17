//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_4
//               -----------------------------------------

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <memory>
#include <unordered_map>
#include <cctype>
#include <limits>
#include <chrono>
using namespace std;
namespace fs = std::filesystem;

// ======= Enums =======
enum ReserveDay { SATURDAY = 0, SUNDAY = 1, MONDAY = 2, TUESDAY = 3, WEDNESDAY = 4, THURSDAY = 5, FRIDAY_OFF = -1 };
enum MealType { BREAKFAST, LUNCH, DINNER };
enum RStatus { SUCCESS, FAILED, CANCELLED, NOT_PAID };

enum TransactionType { TRANSFER, PAYMENT };
enum TransactionStatus { PENDING, COMPLETED, FAILED_TX };
enum SessionStatus { AUTHENTICATED, ANONYMOUS };

// =========================== ConfigPaths ===========================
class ConfigPaths {
public:
    fs::path base_dir;
    fs::path users_dir;
    fs::path meals_dir;
    fs::path logs_dir;
    fs::path sessions_dir;

    fs::path c_students;    // data/users/students.csv
    fs::path c_admins;      // data/users/admins.csv
    fs::path c_dining;      // data/meals/dining.csv

    fs::path t_student_transactions;
    fs::path l_students_log_file;
    fs::path l_admins_log_file;

    static ConfigPaths& instance() {
        static ConfigPaths cfg;
        return cfg;
    }

private:
    ConfigPaths() {
        base_dir = "data";
        users_dir = base_dir / "user";
        meals_dir = base_dir / "meals";
        logs_dir = base_dir / "logs";
        sessions_dir = base_dir / "sessions";

        // ensure directories exist (don't modify CSVs)
        try {
            fs::create_directories(users_dir);
            fs::create_directories(meals_dir);
            fs::create_directories(logs_dir);
            fs::create_directories(sessions_dir);
        }
        catch (...) {}

        c_students = users_dir / "studentsCsvFile.csv"; // use your provided CSV filename
        c_admins = users_dir / "admins.csv";
        c_dining = meals_dir / "dining.csv";

        t_student_transactions = base_dir / "student_transactions.txt";
        l_students_log_file = logs_dir / "students.log";
        l_admins_log_file = logs_dir / "admins.log";
    }
};

// =========================== Helpers ===========================
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

static string trim_str(const string& s) {
    size_t a = 0;
    while (a < s.size() && isspace((unsigned char)s[a])) ++a;
    size_t b = s.size();
    while (b > a && isspace((unsigned char)s[b - 1])) --b;
    return s.substr(a, b - a);
}

static MealType mealTypeFromString(const string& s) {
    string t = s;
    transform(t.begin(), t.end(), t.begin(), [](unsigned char c) { return (char)tolower(c); });
    if (t.find("break") != string::npos || t.find("breakfast") != string::npos) return BREAKFAST;
    if (t.find("din") != string::npos || t.find("dinner") != string::npos) return DINNER;
    return LUNCH;
}

static string mealTypeToString(MealType mt) {
    switch (mt) {
    case BREAKFAST: return "Breakfast";
    case LUNCH: return "Lunch";
    case DINNER: return "Dinner";
    }
    return "Lunch";
}

// Map uniDay (0..5) to filename
static string dayFileName(int uniDay) {
    switch (uniDay) {
    case 0: return "saturday.csv";
    case 1: return "sunday.csv";
    case 2: return "monday.csv";
    case 3: return "tuesday.csv";
    case 4: return "wednesday.csv";
    case 5: return "thursday.csv";
    default: return "";
    }
}

// determine university day from system localtime
static int get_university_day() {
    time_t now = time(nullptr);
    struct tm local_tm;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&local_tm, &now);
#else
    localtime_r(&now, &local_tm);
#endif
    int wday = local_tm.tm_wday; // 0=Sunday ... 6=Saturday
    if (wday == 6) return 0; // Saturday
    if (wday == 0) return 1; // Sunday
    if (wday == 1) return 2; // Monday
    if (wday == 2) return 3; // Tuesday
    if (wday == 3) return 4; // Wednesday
    if (wday == 4) return 5; // Thursday
    return FRIDAY_OFF;
}

// clear screen
static void clear_screen() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    // prefer ANSI clear
    if (const char* term = getenv("TERM")) {
        // use system clear for safety
        system("clear");
    }
    else {
        cout << string(100, '\n');
    }
#endif
}

// write log into data/logs/system.log
static void write_log(const string& msg) {
    fs::path logdir = ConfigPaths::instance().logs_dir; // data/logs as requested
    try { fs::create_directories(logdir); }
    catch (...) {}
    fs::path logfile = logdir / "system.log";

    ofstream fout(logfile, ios::app);
    if (fout.is_open()) {
        auto now = chrono::system_clock::now();
        time_t tnow = chrono::system_clock::to_time_t(now);
#if defined(_WIN32) || defined(_WIN64)
        struct tm timeinfo;
        localtime_s(&timeinfo, &tnow);
        char buf[64];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
        fout << "[" << buf << "] " << msg << "\n";
#else
        struct tm timeinfo;
        localtime_r(&tnow, &timeinfo);
        char buf[64];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
        fout << "[" << buf << "] " << msg << "\n";
#endif
    }
}

// =========================== Classes ===========================
class User {
protected:
    int id;
    string name;
    string lastName;
    string email;
    string hashedPassword;
public:
    User() : id(0), name(""), lastName(""), email(""), hashedPassword("") {}
    virtual ~User() {}
    // getters / setters
    void set_id(int _id) { id = _id; }
    int get_id() const { return id; }
    void set_name(const string& n) { name = n; }
    string get_name() const { return name; }
    void set_last_name(const string& ln) { lastName = ln; }
    string get_last_name() const { return lastName; }
    void set_email(const string& e) { email = e; }
    string get_email() const { return email; }
    void setHashedPassword(const string& hash) { hashedPassword = hash; }
    string getHashedPassword() const { return hashedPassword; }

    static string simpleHash(const string& input) {
        unsigned int hash = 0;
        for (char c : input) {
            hash = hash * 31 + static_cast<unsigned int>(c);
        }
        return to_string(hash);
    }
    void setPassword(const string& password) {
        hashedPassword = simpleHash(password);
    }
    bool checkPassword(const string& password) const {
        if (!hashedPassword.empty() && hashedPassword.rfind("$2", 0) == 0) {
#if defined(__unix__) || defined(__APPLE__)
            char* res = crypt(password.c_str(), hashedPassword.c_str());
            if (res) return hashedPassword == string(res);
#endif
            return false;
        }
        return hashedPassword == simpleHash(password);
    }

    virtual void print() const = 0;
    virtual string getType() const = 0;
};

class Admin : public User {
public:
    Admin() : User() {}
    void print() const override {
        cout << "[ADMIN] " << name << " " << lastName << " (id=" << id << ") email: " << email << endl;
    }
    string getType() const override { return "Admin"; }
};

class Meal {
private:
    int meal_id;
    string name;
    float price;
    MealType meal_type;
    bool active;
public:
    Meal() : meal_id(0), name(""), price(0.0f), meal_type(LUNCH), active(false) {}
    void set_meal_id(int id) { meal_id = id; }
    int get_meal_id() const { return meal_id; }
    void set_name(const string& n) { name = n; }
    string get_name() const { return name; }
    void set_price(float p) { price = p; }
    float get_price() const { return price; }
    void set_meal_type(MealType mt) { meal_type = mt; }
    MealType get_meal_type() const { return meal_type; }
    void activate() { active = true; }
    void deactivate() { active = false; }
    bool is_active() const { return active; }
    void print() const {
        cout << "MealID: " << meal_id << " | " << name << " | " << price << " | " << mealTypeToString(meal_type) << endl;
    }
};

class DiningHall {
private:
    int hall_id;
    string name;
    string address;
    int capacity;
public:
    DiningHall() : hall_id(0), name(""), address(""), capacity(0) {}
    void set_hall_id(int id) { hall_id = id; }
    int get_hall_id() const { return hall_id; }
    void set_name(const string& n) { name = n; }
    string get_name() const { return name; }
    void set_address(const string& a) { address = a; }
    string get_address() const { return address; }
    void set_capacity(int c) { capacity = c; }
    int get_capacity() const { return capacity; }
    void print() const {
        cout << "HallID: " << hall_id << " | " << name << " | capacity: " << capacity;
        if (!address.empty()) cout << " | address: " << address;
        cout << endl;
    }
};

class Reservation {
private:
    int reservation_id;
    Meal* meal;
    DiningHall* hall;
    RStatus status;
    time_t created_at;
public:
    Reservation() : reservation_id(0), meal(nullptr), hall(nullptr), status(NOT_PAID), created_at(time(nullptr)) {}
    void set_reservation_id(int id) { reservation_id = id; }
    int get_reservation_id() const { return reservation_id; }
    void set_meal(Meal* m) { meal = m; }
    Meal* get_meal() const { return meal; }
    void set_hall(DiningHall* h) { hall = h; }
    DiningHall* get_hall() const { return hall; }
    void set_status(RStatus s) { status = s; }
    RStatus get_status() const { return status; }
    void set_created_at(time_t t) { created_at = t; }
    time_t get_created_at() const { return created_at; }
    void print() const {
        cout << "Reservation ID: " << reservation_id << " Status: ";
        switch (status) {
        case SUCCESS: cout << "Success"; break;
        case FAILED: cout << "Failed"; break;
        case CANCELLED: cout << "Cancelled"; break;
        case NOT_PAID: cout << "Not Paid"; break;
        }
        cout << "\nCreated: " << ctime(&created_at);
        if (meal) {
            cout << " -> Meal: "; meal->print();
        }
        if (hall) {
            cout << " -> Hall: "; hall->print();
        }
    }
};

class Student : public User {
private:
    string student_id;
    string phone;
    bool active_flag;
    float balance;
    vector<Reservation> reservations;
public:
    Student() : User(), student_id(""), phone(""), active_flag(false), balance(0.0f) {}
    void set_student_id(const string& s) { student_id = s; }
    string get_student_id() const { return student_id; }
    void set_phone(const string& p) { phone = p; }
    string get_phone() const { return phone; }
    void activate() { active_flag = true; }
    void deactivate() { active_flag = false; }
    bool is_active() const { return active_flag; }
    void set_balance(float b) { balance = b; }
    float get_balance() const { return balance; }
    vector<Reservation> get_reservations() const { return reservations; }
    void add_reservation(const Reservation& r) { reservations.push_back(r); }

    void print() const override {
        cout << "[STUDENT] " << name << " " << lastName << " (id=" << id << ", student_id=" << student_id << ")\n";
        cout << "email: " << email << " | phone: " << phone << " | balance: " << balance << " | active: " << (active_flag ? "yes" : "no") << endl;
    }
    string getType() const override { return "Student"; }

    bool reserve_meal(Meal* meal, DiningHall* hall) {
        if (!is_active()) {
            cout << "Reservation failed: student is not active." << endl;
            write_log("Reservation failed (student not active) for student id=" + to_string(id));
            return false;
        }
        if (!meal || !hall) {
            cout << "\033[1;31mReservation failed: invalid meal or hall.\033[0m" << endl;
            write_log("Reservation failed (invalid meal/hall) student id=" + to_string(id));
            return false;
        }
        if (balance < meal->get_price()) {
            cout << "\033[1;31mReservation failed: insufficient balance.\033[0m" << endl;
            write_log("Reservation failed (insufficient balance) student id=" + to_string(id));
            return false;
        }
        time_t now = time(nullptr);
        struct tm now_tm;
#if defined(_WIN32) || defined(_WIN64)
        localtime_s(&now_tm, &now);
#else
        localtime_r(&now, &now_tm);
#endif
        for (const auto& r : reservations) {
            if (r.get_status() == SUCCESS) {
                time_t rt = r.get_created_at();
                struct tm r_tm;
#if defined(_WIN32) || defined(_WIN64)
                localtime_s(&r_tm, &rt);
#else
                localtime_r(&rt, &r_tm);
#endif
                if (now_tm.tm_year == r_tm.tm_year && now_tm.tm_yday == r_tm.tm_yday) {
                    if (r.get_meal() && r.get_meal()->get_meal_type() == meal->get_meal_type()) {
                        cout << "Reservation failed: already reserved for this meal type today." << endl;
                        write_log("Reservation failed (already reserved same meal type) student id=" + to_string(id));
                        return false;
                    }
                }
            }
        }
        Reservation nr;
        nr.set_reservation_id(static_cast<int>(reservations.size() + 1));
        nr.set_meal(meal);
        nr.set_hall(hall);
        nr.set_status(SUCCESS);
        nr.set_created_at(now);
        balance -= meal->get_price();
        reservations.push_back(nr);
        cout << "\033[1;32mReservation successful! New balance: " << balance << "\033[0m\n" << endl;
        write_log("Reservation success: student id=" + to_string(id) + " mealID=" + to_string(meal->get_meal_id()) + " hallID=" + to_string(hall->get_hall_id()));
        return true;
    }

    bool cancel_reservation(int reservation_id) {
        for (auto& r : reservations) {
            if (r.get_reservation_id() == reservation_id) {
                if (r.get_status() == CANCELLED) {
                    cout << "Cancellation failed: already cancelled." << endl;
                    write_log("Cancellation failed (already cancelled) student id=" + to_string(id) + " resID=" + to_string(reservation_id));
                    return false;
                }
                if (r.get_meal()) {
                    balance += r.get_meal()->get_price();
                }
                r.set_status(CANCELLED);
                cout << "\033[1;32mReservation cancelled. Refund issued.\033[0m" << endl;
                write_log("Reservation cancelled: student id=" + to_string(id) + " resID=" + to_string(reservation_id));
                return true;
            }
        }
        cout << "Cancellation failed: reservation not found." << endl;
        write_log("Cancellation failed (not found) student id=" + to_string(id) + " resID=" + to_string(reservation_id));
        return false;
    }
};

// ========== Storage Singleton ==========
class Storage {
private:
    vector<Meal> meals; // today's meals loaded
    vector<DiningHall> halls;
    int nextMealId;
    int nextHallId;
    Storage() : nextMealId(1000), nextHallId(1000) {}
public:
    static Storage& instance() {
        static Storage inst;
        return inst;
    }
    // meals
    vector<Meal>& getMeals() { return meals; }
    void addMeal(const Meal& m) { meals.push_back(m); if (m.get_meal_id() >= nextMealId) nextMealId = m.get_meal_id() + 1; }
    void clearMeals() { meals.clear(); }
    int reserveNextMealID() { return nextMealId++; }
    // halls
    vector<DiningHall>& getHalls() { return halls; }
    void addHall(const DiningHall& d) { halls.push_back(d); if (d.get_hall_id() >= nextHallId) nextHallId = d.get_hall_id() + 1; }
    void clearHalls() { halls.clear(); }
    int reserveNextHallID() { return nextHallId++; }

    Meal* findMealByID(int id) {
        for (auto& m : meals) if (m.get_meal_id() == id) return &m;
        return nullptr;
    }
    DiningHall* findHallByID(int id) {
        for (auto& h : halls) if (h.get_hall_id() == id) return &h;
        return nullptr;
    }
};

// ========== Session Manager for Student ==========
namespace StudentSession {
    class SessionManager {
    private:
        Student* current;
        SessionManager() : current(nullptr) {}
        SessionManager(const SessionManager&) = delete;
        SessionManager& operator=(const SessionManager&) = delete;
    public:
        static SessionManager& instance() {
            static SessionManager inst;
            return inst;
        }
        Student* currentStudent() const { return current; }

        void login(const string& identifier, const string& password) {
            // free existing if any
            if (current) { delete current; current = nullptr; }

            fs::path students_csv = ConfigPaths::instance().c_students;
            ifstream fin(students_csv);
            if (!fin.is_open()) {
                cout << "Student CSV not found at " << students_csv << " — cannot login." << endl;
                write_log("Student login attempted but students CSV missing: " + students_csv.string());
                return;
            }
            string header;
            if (!getline(fin, header)) { cout << "Student CSV empty\n"; fin.close(); return; }
            vector<string> headers = split_csv_line(header);
            unordered_map<string, int> idx;
            for (size_t i = 0; i < headers.size(); ++i) {
                string h = trim_str(headers[i]);
                transform(h.begin(), h.end(), h.begin(), [](unsigned char c) { return (char)tolower(c); });
                idx[h] = static_cast<int>(i);
            }
            auto findIdx = [&](initializer_list<string> names)->int {
                for (auto& n : names) {
                    string low = n; transform(low.begin(), low.end(), low.begin(), [](unsigned char c) { return (char)tolower(c); });
                    if (idx.find(low) != idx.end()) return idx[low];
                }
                return -1;
                };
            int idx_studentid = findIdx({ "studentid","student_id","userid","userID","id" });
            int idx_name = findIdx({ "name","fullname","first_name" });
            int idx_last = findIdx({ "lastname","last_name","family","surname" });
            int idx_hash = findIdx({ "hashpassword","hashedpassword","password","hash","hashpassword" });
            int idx_email = findIdx({ "email","mail" });
            int idx_phone = findIdx({ "phone","mobile","tel" });
            int idx_active = findIdx({ "active","isactive","activated" });

            string line;
            bool found = false;
            while (getline(fin, line)) {
                if (line.empty()) continue;
                vector<string> cols = split_csv_line(line);
                auto getVal = [&](int idxx)->string {
                    if (idxx >= 0 && idxx < (int)cols.size()) return trim_str(cols[idxx]);
                    return string();
                    };
                string s_studentid = getVal(idx_studentid);
                string s_email = getVal(idx_email);
                string s_hash = getVal(idx_hash);

                bool match = false;
                if (identifier.find('@') != string::npos) {
                    if (!s_email.empty() && s_email == identifier) match = true;
                }
                else {
                    if (!s_studentid.empty() && s_studentid == identifier) match = true;
                    if (!match && !cols.empty() && trim_str(cols[0]) == identifier) match = true;
                }
                if (!match) continue;

                // construct student
                Student* s = new Student();
                if (!s_studentid.empty()) { try { s->set_id(stoi(s_studentid)); } catch (...) {} s->set_student_id(s_studentid); }
                string nm = getVal(idx_name); if (!nm.empty()) s->set_name(nm);
                string ln = getVal(idx_last); if (!ln.empty()) s->set_last_name(ln);
                string ph = getVal(idx_phone); if (!ph.empty()) s->set_phone(ph);
                string bal = getVal(findIdx({ "balance","wallet","credit" }));
                if (!bal.empty()) { try { s->set_balance(stof(bal)); } catch (...) {} }
                if (!s_hash.empty()) s->setHashedPassword(s_hash);

                // check password
                if (!password.empty()) {
                    if (!s_hash.empty() && s_hash.rfind("$2", 0) == 0) {
#if defined(__unix__) || defined(__APPLE__)
                        char* res = crypt(password.c_str(), s_hash.c_str());
                        if (!res || s_hash != string(res)) { delete s; cout << "Student login failed: password mismatch (bcrypt)\n"; fin.close(); write_log("Student login failed (bcrypt mismatch) for " + identifier); return; }
#else
                        cout << "Student login: bcrypt hash detected but crypt() not available on this platform.\n";
                        delete s; fin.close(); write_log("Student login failed (bcrypt not supported) for " + identifier); return;
#endif
                    }
                    else {
                        if (!s->checkPassword(password)) { delete s; cout << "\033[1;31mEmail or password doesnt match our records!\033[0m\n"; this_thread::sleep_for(chrono::seconds(2)); fin.close(); write_log("Student login failed (password mismatch) for " + identifier); return; }
                    }
                }

                // active flag from CSV optional
                string act = getVal(idx_active);
                if (!act.empty()) {
                    if (act == "1" || act == "true" || act == "yes") s->activate();
                }
                else {
                    // default: activate upon login (quick fix requested)
                    s->activate();
                }

                current = s;
                found = true;
                break;
            }
            fin.close();
            if (!found) {
                cout << "\033[1;31mEmail or password doesnt match our records!\033[0m\n";
                write_log("Student login failed: identifier=" + identifier);
                this_thread::sleep_for(chrono::seconds(2));
            }
            else {
                cout << "\033[1;32mStudent loggin in: " << current->get_name() << " | balance: " << current->get_balance() << "\033[0m\n";
                write_log("Student logged in: " + (current->get_email().empty() ? identifier : current->get_email()));
                this_thread::sleep_for(chrono::seconds(1));
            }
        }

        void logout() {
            if (current) {
                cout << "\033[1;32mStudent Signing out... \033[0m\n";
                write_log("Student logged out: " + (current->get_email().empty() ? to_string(current->get_id()) : current->get_email()));
                this_thread::sleep_for(chrono::seconds(1));
                delete current;
                current = nullptr;
            }
            else {
                write_log("Student logout called but no student logged in");
            }
        }
    };
} // namespace StudentSession

// ========== Admin session ==========
namespace AdminSession {
    class SessionManager {
    private:
        Admin* current;
        SessionManager() : current(nullptr) {}
        SessionManager(const SessionManager&) = delete;
        SessionManager& operator=(const SessionManager&) = delete;
    public:
        static SessionManager& instance() {
            static SessionManager inst;
            return inst;
        }
        Admin* currentAdmin() const { return current; }

        void login(const string& identifier, const string& password) {
            if (current) { delete current; current = nullptr; }

            fs::path admins_csv = ConfigPaths::instance().c_admins;
            ifstream fin(admins_csv);
            if (!fin.is_open()) { cout << "Admin CSV not found\n"; write_log("Admin login attempted but admins CSV missing"); return; }
            string header; if (!getline(fin, header)) { cout << "Admin CSV empty\n"; fin.close(); return; }
            vector<string> headers = split_csv_line(header);
            unordered_map<string, int> idx;
            for (size_t i = 0; i < headers.size(); ++i) { string h = trim_str(headers[i]); transform(h.begin(), h.end(), h.begin(), [](unsigned char c) { return (char)tolower(c); }); idx[h] = static_cast<int>(i); }
            auto findIdx = [&](initializer_list<string> names)->int {
                for (auto& n : names) { string low = n; transform(low.begin(), low.end(), low.begin(), [](unsigned char c) { return (char)tolower(c); }); if (idx.find(low) != idx.end()) return idx[low]; } return -1;
                };
            int idx_id = findIdx({ "id","admin_id","adminid" });
            int idx_name = findIdx({ "name","firstname" });
            int idx_last = findIdx({ "lastname","last_name" });
            int idx_email = findIdx({ "email","mail" });
            int idx_hash = findIdx({ "hashedpassword","hashed_password","password","hash" });

            string line;
            bool found = false;
            while (getline(fin, line)) {
                if (line.empty()) continue;
                vector<string> cols = split_csv_line(line);
                auto getVal = [&](int ix)->string { if (ix >= 0 && ix < (int)cols.size()) return trim_str(cols[ix]); return string(); };
                string em = getVal(idx_email);
                if (em != identifier) continue;
                Admin* a = new Admin();
                string sid = getVal(idx_id); if (!sid.empty()) { try { a->set_id(stoi(sid)); } catch (...) {} }
                string nm = getVal(idx_name); if (!nm.empty()) a->set_name(nm);
                string ln = getVal(idx_last); if (!ln.empty()) a->set_last_name(ln);
                a->set_email(em);
                string hsh = getVal(idx_hash); if (!hsh.empty()) a->setHashedPassword(hsh);
                if (!password.empty()) {
                    if (!hsh.empty() && hsh.rfind("$2", 0) == 0) {
#if defined(__unix__) || defined(__APPLE__)
                        char* res = crypt(password.c_str(), hsh.c_str());
                        if (!res || hsh != string(res)) { delete a; cout << "Admin login failed: bcrypt mismatch\n"; fin.close(); write_log("Admin login failed (bcrypt mismatch) for " + identifier); return; }
#else
                        cout << "Admin login: bcrypt hash present but crypt not available\n"; delete a; fin.close(); write_log("Admin login failed (bcrypt not supported) for " + identifier); return;
#endif
                    }
                    else {
                        if (!a->checkPassword(password)) { delete a; cout << "\033[1;31mEmail or password doesnt match admin records!\033[0m\n"; this_thread::sleep_for(chrono::seconds(2)); fin.close(); write_log("Admin login failed (password mismatch) for " + identifier); return; }
                    }
                }
                current = a; found = true; break;
            }
            fin.close();
            if (!found) {
                cout << "\033[1;31mEmail or password doesnt match admin records!\033[0m\n"; write_log("Admin login failed: " + identifier);
                this_thread::sleep_for(chrono::seconds(2));
            }
            else { cout << "\033[1;32mAdmin logged in: " << current->get_name() << "\n"; write_log("Admin logged in: \033[0m" + current->get_email()); this_thread::sleep_for(chrono::seconds(1)); }
        }

        void logout() {
            if (current) { write_log("Admin logged out: " + current->get_email()); delete current; current = nullptr; }
            cout << "\033[1;32mAdmin Signing out...\033[0m\n";
            this_thread::sleep_for(chrono::seconds(1));
        }
    };
} // namespace AdminSession

// ========== Demo creator & loaders ==========
void ensure_demo_files() {
    fs::path students_csv = ConfigPaths::instance().c_students;
    if (!fs::exists(students_csv)) {
        ofstream fout(students_csv);
        fout << "userID,studentID,name,lastname,hashpassword,email,phone,balance\n";
        fout << "0,4022604305,Ali,Hashemi," << User::simpleHash("1234")
            << ",ali@example.com,09651065286,20000\n";
        fout.close();
        write_log("Created demo students CSV: " + students_csv.string());
        cout << "Created demo students file: " << students_csv << endl;
    }

    // admins
    fs::path admins_csv = ConfigPaths::instance().c_admins;
    if (!fs::exists(admins_csv)) {
        ofstream fout(admins_csv);
        fout << "id,name,lastName,email,hashedPassword\n";
        fout << "1,Admin,User,admin@example.com," << User::simpleHash("admin") << "\n";
        fout.close();
        write_log("Created demo admins CSV: " + admins_csv.string());
        cout << "Created demo admins file: " << admins_csv << endl;
    }

    // dining halls file
    fs::path dining_csv = ConfigPaths::instance().c_dining;
    if (!fs::exists(dining_csv)) {
        ofstream fout(dining_csv);
        fout << "hallID,name,capacity,address\n";
        fout << "1,AmirAbaad,200,AmirAbaad\n";
        fout << "2,Shokat_BrosPardis,150,Shokat_BrosPardis\n";
        fout.close();
        write_log("Created demo dining CSV: " + dining_csv.string());
        cout << "Created demo dining file: " << dining_csv << endl;
    }

    // per-day meal files: saturday..thursday
    vector<pair<string, vector<pair<int, pair<string, int>>>>> demoDays;
    demoDays.push_back({ "saturday.csv", {{1, {"ZereshkPolo_Morq",15000}}, {2, {"Khorsh_Bamiye",12000}}} });
    demoDays.push_back({ "sunday.csv",   {{1, {"KababKobide",15000}}, {2, {"Stanboli",12000}}} });
    demoDays.push_back({ "monday.csv",   {{1, {"TonMahi",15000}}, {2, {"Khoresh_Qeyme",12000}}} });
    demoDays.push_back({ "tuesday.csv",  {{1, {"Khoresh_QormeSabzi",15000}}, {2, {"LobiaPolo",12000}}} });
    demoDays.push_back({ "wednesday.csv",{{1, {"Joj",15000}}, {2, {"Gosht",12000}}} });
    demoDays.push_back({ "thursday.csv", {{1, {"IDK BRO",15000}}, {2, {"WTF",12000}}} });

    for (auto& day : demoDays) {
        fs::path p = ConfigPaths::instance().meals_dir / day.first;
        if (!fs::exists(p)) {
            ofstream fout(p);
            fout << "mealID,name,price,type\n";
            for (auto& it : day.second) {
                fout << it.first << "," << it.second.first << "," << it.second.second << ",Lunch\n";
            }
            fout.close();
            write_log("Created demo meal file: " + p.string());
            cout << "Created demo meal file: " << p << endl;
        }
    }

    // tx file (touch)
    fs::path tx = ConfigPaths::instance().t_student_transactions;
    if (!fs::exists(tx)) {
        ofstream fout(tx); fout.close();
    }
}

void load_today_meals_into_storage() {
    Storage::instance().clearMeals();
    int uniDay = get_university_day();
    if (uniDay == FRIDAY_OFF) return;
    string fname = dayFileName(uniDay);
    if (fname.empty()) return;
    fs::path p = ConfigPaths::instance().meals_dir / fname;
    if (!fs::exists(p)) {
        cout << "Today meal file not found: " << p << " (no meals)\n";
        return;
    }
    ifstream fin(p);
    string header;
    if (!getline(fin, header)) { fin.close(); return; }
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        vector<string> cols = split_csv_line(line);
        if (cols.size() < 3) continue;
        Meal m;
        try { m.set_meal_id(stoi(trim_str(cols[0]))); }
        catch (...) { m.set_meal_id(Storage::instance().reserveNextMealID()); }
        m.set_name(trim_str(cols[1]));
        try { m.set_price(stof(trim_str(cols[2]))); }
        catch (...) { m.set_price(0.0f); }
        if (cols.size() >= 4) m.set_meal_type(mealTypeFromString(trim_str(cols[3])));
        m.activate();
        Storage::instance().addMeal(m);
    }
    fin.close();
}

void load_halls_into_storage() {
    Storage::instance().clearHalls();
    fs::path p = ConfigPaths::instance().c_dining;
    if (!fs::exists(p)) return;
    ifstream fin(p);
    string header;
    if (!getline(fin, header)) { fin.close(); return; }
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        vector<string> cols = split_csv_line(line);
        if (cols.size() < 2) continue;
        DiningHall d;
        try { d.set_hall_id(stoi(trim_str(cols[0]))); }
        catch (...) { d.set_hall_id(Storage::instance().reserveNextHallID()); }
        d.set_name(trim_str(cols[1]));
        if (cols.size() >= 3) { try { d.set_capacity(stoi(trim_str(cols[2]))); } catch (...) { d.set_capacity(0); } }
        if (cols.size() >= 4) d.set_address(trim_str(cols[3]));
        Storage::instance().addHall(d);
    }
    fin.close();
}

// ========== Admin CRUD helpers (meals per day & dining) ==========
bool admin_append_meal_to_day(int uniDay, const string& name, float price, const string& typeStr) {
    if (uniDay == FRIDAY_OFF) { cout << "Cannot add meal for Friday (off)\n"; return false; }
    string fname = dayFileName(uniDay);
    if (fname.empty()) return false;
    fs::path p = ConfigPaths::instance().meals_dir / fname;
    int nextId = 1;
    if (fs::exists(p)) {
        ifstream fin(p);
        string header;
        if (getline(fin, header)) {
            string line; int maxid = 0;
            while (getline(fin, line)) {
                if (line.empty()) continue;
                vector<string> c = split_csv_line(line);
                if (c.size() >= 1) {
                    try { int id = stoi(trim_str(c[0])); if (id > maxid) maxid = id; }
                    catch (...) {}
                }
            }
            nextId = maxid + 1;
        }
        fin.close();
    }
    else {
        ofstream fout(p);
        fout << "mealID,name,price,type\n";
        fout.close();
    }
    ofstream fout(p, ios::app);
    if (!fout.is_open()) { cout << "Failed to open " << p << " for append\n"; return false; }
    fout << nextId << "," << name << "," << price << "," << typeStr << "\n";
    fout.close();
    write_log("Admin added meal id=" + to_string(nextId) + " to " + p.string());
    cout << "Added meal to " << p << " with id " << nextId << "\n";
    return true;
}

bool admin_remove_meal_from_day(int uniDay, int mealId) {
    if (uniDay == FRIDAY_OFF) { cout << "No meals on Friday\n"; return false; }
    string fname = dayFileName(uniDay);
    fs::path p = ConfigPaths::instance().meals_dir / fname;
    if (!fs::exists(p)) { cout << "Meal file not found: " << p << "\n"; return false; }
    ifstream fin(p);
    string header;
    if (!getline(fin, header)) { fin.close(); return false; }
    vector<string> outLines;
    outLines.push_back(header);
    string line;
    bool removed = false;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        vector<string> c = split_csv_line(line);
        if (c.size() >= 1) {
            try {
                int id = stoi(trim_str(c[0]));
                if (id == mealId) { removed = true; continue; }
            }
            catch (...) {}
        }
        outLines.push_back(line);
    }
    fin.close();
    ofstream fout(p, ios::trunc);
    for (auto& l : outLines) fout << l << "\n";
    fout.close();
    if (removed) {
        write_log("Admin removed meal id=" + to_string(mealId) + " from " + p.string());
        cout << "Removed meal " << mealId << " from " << p << "\n";
    }
    else cout << "Meal id not found in " << p << "\n";
    return removed;
}

bool admin_append_hall(const string& name, int capacity, const string& address) {
    fs::path p = ConfigPaths::instance().c_dining;
    int nextId = 1;
    if (fs::exists(p)) {
        ifstream fin(p);
        string header;
        if (getline(fin, header)) {
            string line; int maxid = 0;
            while (getline(fin, line)) {
                if (line.empty()) continue;
                vector<string> c = split_csv_line(line);
                if (c.size() >= 1) {
                    try { int id = stoi(trim_str(c[0])); if (id > maxid) maxid = id; }
                    catch (...) {}
                }
            }
            nextId = maxid + 1;
        }
        fin.close();
    }
    else {
        ofstream f(p); f << "hallID,name,capacity,address\n"; f.close();
    }
    ofstream fout(p, ios::app);
    if (!fout.is_open()) { cout << "Failed to open " << p << "\n"; return false; }
    fout << nextId << "," << name << "," << capacity << "," << address << "\n";
    fout.close();
    write_log("Admin added dining hall id=" + to_string(nextId));
    cout << "Added dining hall id=" << nextId << "\n";
    return true;
}

bool admin_remove_hall(int hallId) {
    fs::path p = ConfigPaths::instance().c_dining;
    if (!fs::exists(p)) { cout << "Dining file missing\n"; return false; }
    ifstream fin(p);
    string header;
    if (!getline(fin, header)) { fin.close(); return false; }
    vector<string> out;
    out.push_back(header);
    string line; bool removed = false;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        vector<string> c = split_csv_line(line);
        if (c.size() >= 1) {
            try {
                int id = stoi(trim_str(c[0]));
                if (id == hallId) { removed = true; continue; }
            }
            catch (...) {}
        }
        out.push_back(line);
    }
    fin.close();
    ofstream fout(p, ios::trunc);
    for (auto& l : out) fout << l << "\n";
    fout.close();
    if (removed) {
        write_log("Admin removed dining hall id=" + to_string(hallId));
        cout << "Removed hall id=" << hallId << "\n";
    }
    else cout << "Hall id not found\n";
    return removed;
}

// ========== Admin interactive menu ==========
void admin_panel_loop() {
    if (!AdminSession::SessionManager::instance().currentAdmin()) { cout << "No admin logged in\n"; return; }
    int opt = 0;
    do {
        clear_screen();
        cout << "\n\033[1;34m==== Admin Panel ====\033[0m\n";
        cout << "1) Show today's meals\n";
        cout << "2) Add meal for a day\n";
        cout << "3) Remove meal from a day\n";
        cout << "4) Show dining halls\n";
        cout << "5) Add dining hall\n";
        cout << "6) Remove dining hall\n";
        cout << "7) Refresh storage (reload today's meals & halls)\n";
        cout << "8) Logout admin\n";
        cout << "Choose: ";
        if (!(cin >> opt)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "\033[1;31mInvalid\033[0m\n"; continue; }
        if (opt == 1) {
            int uniDay = get_university_day();
            if (uniDay == FRIDAY_OFF) {
                cout << "It's Friday — no meals available.\n";
            }
            else {
                Storage::instance().clearMeals();
                load_today_meals_into_storage();
                auto& meals = Storage::instance().getMeals();
                if (meals.empty()) cout << "No meals for today\n";
                else {
                    cout << "Today's meals:\n";
                    for (auto& m : meals) m.print();
                }
            }
            cout << "Press ENTER to continue..."; cin.ignore(); cin.get();
        }
        else if (opt == 2) {
            cout << "Which day? (0=Saturday,1=Sunday,2=Monday,3=Tuesday,4=Wednesday,5=Thursday): ";
            int d; cin >> d;
            if (d < 0 || d>5) { cout << "Invalid day\n"; cin.ignore(); cin.get(); continue; }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Meal name (no comma): "; string name; getline(cin, name);
            cout << "Price (integer): "; int price; cin >> price;
            cout << "Type (Breakfast/Lunch/Dinner): "; string type; cin >> ws; getline(cin, type);
            if (admin_append_meal_to_day(d, name, (float)price, type)) {
                cout << "Added.\n";
                write_log("Admin added meal " + name + " day=" + to_string(d));
            }
            cout << "Press ENTER to continue..."; cin.ignore(); cin.get();
        }
        else if (opt == 3) {
            cout << "Which day to remove from? (0=Sat..5=Thu): "; int d; cin >> d;
            if (d < 0 || d>5) { cout << "Invalid\n"; cin.ignore(); cin.get(); continue; }
            cout << "Enter mealID to remove: "; int mid; cin >> mid;
            admin_remove_meal_from_day(d, mid);
            cout << "Press ENTER to continue..."; cin.ignore(); cin.get();
        }
        else if (opt == 4) {
            load_halls_into_storage();
            auto& h = Storage::instance().getHalls();
            if (h.empty()) cout << "No dining halls\n";
            else for (auto& hh : h) hh.print();
            cout << "Press ENTER to continue..."; cin.ignore(); cin.get();
        }
        else if (opt == 5) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Hall name: "; string name; getline(cin, name);
            cout << "Capacity: "; int cap; cin >> cap; cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Address: "; string addr; getline(cin, addr);
            admin_append_hall(name, cap, addr);
            cout << "Press ENTER to continue..."; cin.ignore(); cin.get();
        }
        else if (opt == 6) {
            cout << "Enter hallID to remove: "; int hid; cin >> hid;
            admin_remove_hall(hid);
            cout << "Press ENTER to continue..."; cin.ignore(); cin.get();
        }
        else if (opt == 7) {
            load_halls_into_storage();
            load_today_meals_into_storage();
            cout << "Reloaded storage from CSVs\n";
            cout << "Press ENTER to continue..."; cin.ignore(); cin.get();
        }
        else if (opt == 8) {
            AdminSession::SessionManager::instance().logout();
            break;
        }
    } while (opt != 8);
}

// ========== Student interactive loop ==========
void student_panel_loop() {
    Student* cur = StudentSession::SessionManager::instance().currentStudent();
    if (!cur) { cout << "No student logged in\n"; return; }
    if (!cur->is_active()) cur->activate();

    load_halls_into_storage();
    load_today_meals_into_storage();

    int opt = 0;
    do {
        clear_screen();
        cout << "\n\033[1;32mWelcome " << cur->get_name() << " | Balance: " << cur->get_balance() << "\033[0m\n";
        cout << "\n\033[1;34m--- Student Menu ---\033[0m\n";
        cout << "1) View Today's Meals\n";
        cout << "2) View Dining Halls\n";
        cout << "3) Reserve a Meal\n";
        cout << "4) View Reservations\n";
        cout << "5) Increase Balance\n";
        cout << "6) Logout\n";
        cout << "7) Cancel a Reservation\n";
        cout << "Choose: ";
        if (!(cin >> opt)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid\n"; continue; }
        if (opt == 1) {
            int uniDay = get_university_day();
            if (uniDay == FRIDAY_OFF) { cout << "Today is Friday — no meals.\n"; }
            else {
                auto& meals = Storage::instance().getMeals();
                if (meals.empty()) { cout << "No meals today.\n"; }
                else {
                    cout << "Meals for today:\n";
                    for (auto& m : meals) m.print();
                }
            }
            cout << "Press ENTER to continue..."; cin.ignore(); cin.get();
        }
        else if (opt == 2) {
            auto& halls = Storage::instance().getHalls();
            if (halls.empty()) { cout << "No dining halls\n"; }
            else {
                cout << "Dining Halls:\n";
                for (auto& h : halls) h.print();
            }
            cout << "Press ENTER to continue..."; cin.ignore(); cin.get();
        }
        else if (opt == 3) {
            int mealId, hallId;
            cout << "Enter mealID: "; cin >> mealId;
            cout << "Enter hallID: "; cin >> hallId;
            Meal* m = Storage::instance().findMealByID(mealId);
            DiningHall* h = Storage::instance().findHallByID(hallId);
            if (!m) { cout << "Meal not found\n"; cout << "Press ENTER to continue..."; cin.ignore(); cin.get(); continue; }
            if (!h) { cout << "Hall not found\n"; cout << "Press ENTER to continue..."; cin.ignore(); cin.get(); continue; }
            bool ok = cur->reserve_meal(m, h);
            if (!ok) {
                cout << "Reservation failed.\n";
            }
            cout << "Press ENTER to continue..."; cin.ignore(); cin.get();
        }
        else if (opt == 4) {
            auto rs = cur->get_reservations();
            if (rs.empty()) cout << "No reservations\n";
            else for (auto& r : rs) r.print();
            cout << "Press ENTER to continue..."; cin.ignore(); cin.get();
        }
        else if (opt == 5) {
            cout << "Amount to add: "; float amt; cin >> amt; cur->set_balance(cur->get_balance() + amt);
            cout << "Balance updated: " << cur->get_balance() << "\n";
            write_log("Student balance increased: id=" + to_string(cur->get_id()) + " amount=" + to_string(amt));
            cout << "Press ENTER to continue..."; cin.ignore(); cin.get();
        }
        else if (opt == 6) {
            StudentSession::SessionManager::instance().logout();
            break;
        }
        else if (opt == 7) {
            cout << "Enter reservation ID to cancel: "; int rid; cin >> rid;
            bool ok = cur->cancel_reservation(rid);
            if (!ok) cout << "Cancel failed.\n";
            cout << "Press ENTER to continue..."; cin.ignore(); cin.get();
        }
    } while (opt != 6);
}

// ========== Main ==========
int main() {
    cout << "Reservation System - Phase 4\n";

    // ensure demo files exist (only creates if missing)
    ensure_demo_files();

    // initial load
    load_halls_into_storage();
    load_today_meals_into_storage();

    while (true) {
        clear_screen();
        cout << "\n\033[1;32m=== Main Menu ===\033[0m\n";
        cout << "1) Student Login\n";
        cout << "2) Admin Login\n";
        cout << "3) Exit\n";
        cout << "Choose: ";
        int ch;
        if (!(cin >> ch)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "\033[1;31mInvalid input!\033[0m\n"; this_thread::sleep_for(chrono::seconds(1)); continue; }
        if (ch == 1) {
            cout << "Enter email or studentID: ";
            string id; cin >> id;
            cout << "Enter password: ";
            string pw; cin >> pw;
            StudentSession::SessionManager::instance().login(id, pw);
            if (StudentSession::SessionManager::instance().currentStudent()) {
                student_panel_loop();
            }
            // after return go back to main
        }
        else if (ch == 2) {
            cout << "Enter admin email: "; string em; cin >> em;
            cout << "Enter password: "; string pw; cin >> pw;
            AdminSession::SessionManager::instance().login(em, pw);
            if (AdminSession::SessionManager::instance().currentAdmin()) {
                admin_panel_loop();
            }
        }
        else if (ch == 3) {
            cout << "Bye\n";
            write_log("Exited program via main menu");
            break;
        }
        else {
            cout << "\033[1;31mInvalid option!\033[0m\n";
            this_thread::sleep_for(chrono::seconds(1));
        }
    }

    return 0;
}
