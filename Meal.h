//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_2 : Meal.h
//               -----------------------------------------
#pragma once
#include <iostream>
#include <string>
#include <vector>
enum MealType { BREAKFAST, LUNCH, DINNER };

class Meal {
private:
    int meal_id;
    std::string name;
    float price;
    MealType meal_type;
    std::vector<std::string> side_items;

public:
    Meal();
    void print();
    void update_price(float new_price);
    void add_side_item(const std::string& item);

    int get_meal_id();
    std::string get_name();
    float get_price();
    MealType get_meal_type();
    std::vector<std::string> get_side_items();

    void set_meal_id(int id);
    void set_name(const std::string& name);
    void set_price(float price);
    void set_meal_type(MealType type);
};

