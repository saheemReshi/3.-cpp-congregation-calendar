#include <iostream>
#include <sstream>
#include "IMT2023051_Assignment3.h"

using namespace std;

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

// Congregation class method implementations

Congregation::Congregation(string name, string type, Date startDate, Date endDate)
    : _name(name), _type(type), _startDate(startDate), _endDate(endDate) {}

string Congregation::getName() const {
    return _name;
}

Date Congregation::getStartDate() const {
    return _startDate;
}

Date Congregation::getEndDate() const {
    return _endDate;
}

// CongregationManager class method implementations

void CongregationManager::addCongregation(string name, string type, Date startDate, Date endDate) {
    
    Congregation newCongregation(name, type, startDate, endDate);
    congs.push_back(newCongregation);
}
