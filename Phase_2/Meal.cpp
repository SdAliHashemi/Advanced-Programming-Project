//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_1 : Meal.cpp
//               -----------------------------------------
#include "Meal.h"
using namespace std;

Meal::Meal() {
    meal_id = 0;
    name = "";
    price = 0.0;
    meal_type = BREAKFAST;
    side_items.clear();
}

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
    cout << endl;

    cout << "Side Items: ";
    for (const auto& item : side_items) {
        cout << item << " ";
    }
    cout << endl;
}

void Meal::set_meal_id(int id) { meal_id = id; }
void Meal::set_name(const string& name) { this->name = name; }
void Meal::set_price(float price) { this->price = price; }
void Meal::set_meal_type(MealType type) { meal_type = type; }

int Meal::get_meal_id() { return meal_id; }
string Meal::get_name() { return name; }
float Meal::get_price() { return price; }
MealType Meal::get_meal_type() { return meal_type; }
vector<string> Meal::get_side_items() { return side_items; }

void Meal::update_price(float new_price) { price = new_price; }
void Meal::add_side_item(const string& item) { side_items.push_back(item); }

