#include "IMT2023051_Assignment3.h"
using namespace std;

//Time class methods

// Constructor implementation
Time::Time(int hh, int mm) {
        hour = hh;
        min = mm;
        //after making the object, i'll check if the time is valid separately
}

// Getter methods implementation
int Time::getHour() const {
    return hour;
}

int Time::getMinute() const {
    return min;
}

// Method to validate the time
bool Time::isValid() const {
    return isValidHour(hour) && isValidMinute(min);
}

// Method to calculate the difference between two times in minutes
int Time::differenceInMinutes(const Time& other) const {//will only be be useful when both events are in same date
    int thisTotalMinutes = hour * 60 + min;
    int otherTotalMinutes = other.hour * 60 + other.min;
    return thisTotalMinutes - otherTotalMinutes;
}

// Overloaded comparison operators
bool Time::operator<(const Time& other) const {
    return hour < other.hour || (hour == other.hour && min < other.min);
}

bool Time::operator<=(const Time& other) const {
    return *this < other || (*this == other);
}

bool Time::operator>(const Time& other) const {
    return other < *this;
}

bool Time::operator>=(const Time& other) const {
    return !(*this < other);
}

bool Time::operator==(const Time& other) const {
    return hour == other.hour && min == other.min;
}

bool Time::operator!=(const Time& other) const {
    return !(*this == other);
}

ostream& operator<<(ostream& os, const Time& time) {
    os << (time.hour < 10 ? "0" : "") << time.hour << ":"
       << (time.min < 10 ? "0" : "") << time.min;
    return os;
}



// Date class method implementations

bool Date::isLeapYear(int year) const {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool Date::isValidDay(int day, int month, int year) const {
    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (month < 1 || month > 12) return false; // Invalid month
    
    int daysInCurrentMonth = daysInMonth[month - 1];
    if (month == 2 && isLeapYear(year)) {
        daysInCurrentMonth = 29; // February in a leap year
    }

    return day >= 1 && day <= daysInCurrentMonth;
}

Date::Date(int day, int month, int year) : day(day), month(month), year(year) {}

int Date::getDay() const {
    return day;
}

int Date::getMonth() const {
    return month;
}

int Date::getYear() const {
    return year;
}

bool Date::isValid() const {
    if (year < 1) return false; // Invalid year

    return isValidDay(day, month, year);
}

//operator methods
bool Date::operator<(const Date& other) const {
    if (year < other.year) {
        return true;
    } else if (year == other.year) {
        if (month < other.month) {
            return true;
        } else if (month == other.month) {
            return day < other.day;
        }
    }
    return false;
}

bool Date::operator<=(const Date& other) const {
    return *this < other || *this == other;
}

// Greater than operator
bool Date::operator>(const Date& other) const {
    return other < *this;
}

// Greater than or equal to operator
bool Date::operator>=(const Date& other) const {
    return *this > other || *this == other;
}

// Equality operator
bool Date::operator==(const Date& other) const {
    return year == other.year && month == other.month && day == other.day;
}

// Inequality operator
bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}

void Date::printDate() const{
    cout<<year<<"-"<<month<<"-"<<day;
    // cout.flush();
}
//for printing date
ostream& operator<<(ostream& os, const Date& date) {
    os << date.year<<(date.month<10?"0":"")<<date.month<<(date.day<10?"0":"")<<date.day;
    return os;
}

//Reservation methods

Reservation::Reservation(string name, Date s, Date e) 
    : congName(name), s(s), e(e) {}



//Event class methods

Event::Event(string name,Time s, Time e, Date d)
    :name(name),s(s),e(e),d(d) {}







// Congregation class method implementations

Congregation::Congregation(string name, string type, Date startDate, Date endDate)
    : _name(name), _type(type), _startDate(startDate), _endDate(endDate) {}

string Congregation::getName() const {
    return _name;
}

string Congregation::getType() const {
    return _type;
}

Date Congregation::getStartDate() const {
    return _startDate;
}

Date Congregation::getEndDate() const {
    return _endDate;
}

Congregation::~Congregation(){
    //have to iterate through the _reserved vector and remove the reservations
    //*****later! */
}


// CongregationManager class methods

status CongregationManager::addCongregation(string name, string type, Date startDate, Date endDate) {
    //we must keep the congregation names unique across the system
    //so I need to check all the congregations in the vector, if any name matches, return ERROR_CODE
    for(auto cong:congs){
        if(cong.getName()==name)
            return DUPLICATE_CONG_NAME;
    }
    Congregation newCongregation(name, type, startDate, endDate);
    congs.push_back(newCongregation);
    return OK;
}

status CongregationManager::showCongregations() const{
    cout<<congs.size();
    for(auto cong:congs){
        cout<<cong.getName()<<" "<<cong.getType()<<" "<<cong.getStartDate()<<" "
            <<cong.getEndDate()<<endl;
    }
    return NOPRINT_NEED;
}

status CongregationManager::delCongregation(string name){
    //i also have to delete all the reservations for this congregation
    //first I'll iterate through the list of congs and match it with the given name
    for(auto it=congs.begin(); it!=congs.end();){
        if(it->getName()==name){
            //this is the congregation to delete
            congs.erase(it);//i'll deal with deleting the reservations in the destructor of the congregation
            return OK;
        }
        else{
            it++;
        }  
    }
    return CONG_NOT_FOUND;
}


//Venue class methods

// Constructor implementation
Venue::Venue(string name, string city, string addr, string state, string postalCode, string country, int capacity)
    : name(name), city(city), addr(addr), state(state), postalCode(postalCode), country(country), capacity(capacity) {}

// Getter methods implementation
string Venue::getName() const {
    return name;
}

string Venue::getCity() const {
    return city;
}

string Venue::getAddr() const {
    return addr;
}

string Venue::getState() const {
    return state;
}

string Venue::getPostalCode() const {
    return postalCode;
}

string Venue::getCountry() const {
    return country;
}

int Venue::getCapacity() const {
    return capacity;
}



