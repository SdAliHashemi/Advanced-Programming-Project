//               Seyyed Ali Moaven Hashemi
//               Project Reservation System : Phase_2 : Meal.h
//               -----------------------------------------
#pragma once
#include <string>
#include <vector>

enum MealType { BREAKFAST, LUNCH, DINNER };
enum ReserveDay { SATURDAY, SUNDAY, MONDAY, TUESDAY, WEDNESDAY };

class Meal {
private:
    int meal_id;
    std::string name;
    float price;
    MealType meal_type;
    std::vector<std::string> side_items;
    ReserveDay reserve_day;
    bool isActive;

public:
    Meal();

    void print();
    void update_price(float new_price);
    void add_side_item(const std::string& item);

    void set_meal_id(int id);
    void set_name(const std::string& name);
    void set_price(float price);
    void set_meal_type(MealType type);
    void set_reserve_day(ReserveDay day);
    void activate();
    void deactivate();

    int get_meal_id();
    std::string get_name();
    float get_price();
    MealType get_meal_type();
    std::vector<std::string> get_side_items();
    ReserveDay get_reserve_day();
    bool is_active();
};

