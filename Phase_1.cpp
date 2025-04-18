#include <iostram>
#include <string>
#include <vector>
using namespace std;



class Student{
	private :
		int user_id;
		string student_id,name,email;
		float balance;
		bool is_active;
	public : 
		Student();
		Student();
		void print();
		Meal reserve_meal;
		
		
	//getters
		int GetUser_id();
		string GetStudent_id();
		string GetName();
		string GetEmail();
		float GetBalance();
		bool GetActive();
	//setters
		void SetUser_id(int _uid);
		void SetStudent_id(string _sid);
		void SetName(string _n);
		void SetEmail(string _e);
		void SetBalance(float _b);
		void SetActive(bool _a);
};


class Reservation{
	private :
		int reservation_id;
		Student student;
		DiningHall dHall;
		Meal meal;
		Enum status;
		
		string student_id,name,email;
		float balance;
		bool is_active;
		
		
	public :
	//getters
		int GetUser_id();
		string GetStudent_id();
		string GetName();
		string GetEmail();
		float GetBalance();
		bool GetActive();
	//setters
		void SetUser_id(int _uid);
		void SetStudent_id(string _sid);
		void SetName(string _n);
		void SetEmail(string _e);
		void SetBalance(float _b);
		void SetActive(bool _a);
};

class Meal{
	
	
	//getters
		int GetUser_id();
		string GetStudent_id();
		string GetName();
		string GetEmail();
		float GetBalance();
		bool GetActive();
	//setters
		void SetUser_id(int _uid);
		void SetStudent_id(string _sid);
		void SetName(string _n);
		void SetEmail(string _e);
		void SetBalance(float _b);
		void SetActive(bool _a);
};


class DiningHall{
	
	
	
	
	//getters
		int GetUser_id();
		string GetStudent_id();
		string GetName();
		string GetEmail();
		float GetBalance();
		bool GetActive();
	//setters
		void SetUser_id(int _uid);
		void SetStudent_id(string _sid);
		void SetName(string _n);
		void SetEmail(string _e);
		void SetBalance(float _b);
		void SetActive(bool _a);
};
