//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_2 : User.cpp
//               -----------------------------------------
#include "User.h"
using namespace std;

// ------ USER::USER ------
User::User() : id(0), name(""), email(""), password("") {}
//--------------------------------------
void User::set_id(int _id) { id = _id; }
void User::set_name(const string& _name) { name = _name; }
void User::set_email(const string& _email) { email = _email; }
void User::set_password(const string& _password) { password = _password; }
//--------------------------------------
int User::get_id() const { return id; }
string User::get_name() const { return name; }
string User::get_email() const { return email; }
string User::get_password() const { return password; }
//--------------------------------------
User::~User() {}

