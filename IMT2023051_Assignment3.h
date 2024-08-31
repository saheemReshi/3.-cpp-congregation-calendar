#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<sstream>
using namespace std;

enum status{//these are returned to the printcode() function after execution of each command to print -1(error) or 0 etc
    OK,//print 0
    DUPLICATE_VENUE,//ADD VENUE
    NO_VENUE,//DEL VENUE,
    INVALID_DATE_TIME,//when out of range etc
    NO_EVENT,//when no matching event in delEvent etc
    TIME_CONFLICT,//when time slot is occupied in addEvent
    MEMORY_ERROR,//when cant add any more venues or events
    FORMAT_ERROR,//Not used
    NOPRINT_NEED//for showVenues etc, no need to print 0
};

class Venue;
class VenueManager;
class Event;
class Congregation;
class CongregationManager;
class Decoder;
class Date;

class Date {
private:
    int day;
    int month;
    int year;

    bool isLeapYear(int year) const {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    bool isValidDay(int day, int month, int year) const {
        static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        
        if (month < 1 || month > 12) return false; // Invalid month
        
        int daysInCurrentMonth = daysInMonth[month - 1];
        if (month == 2 && isLeapYear(year)) {
            daysInCurrentMonth = 29; // February in a leap year
        }

        return day >= 1 && day <= daysInCurrentMonth;
    }

public:
    // Constructor
    Date(int day, int month, int year) : day(day), month(month), year(year) {}

    // Getter methods
    int getDay() const {
        return day;
    }

    int getMonth() const {
        return month;
    }

    int getYear() const {
        return year;
    }

    // Method to validate the date
    bool isValid() const {
        if (year < 1) return false; // Invalid year

        return isValidDay(day, month, year);
    }
};


class Congregation{
    private:
        string _name;
        string _type;
        Date _startDate;
        Date _endDate;
        vector<Venue *> _reserved;
    public:
        // Constructor
        Congregation(string name, string type, Date startDate, Date endDate)
            : _name(name), _type(type), _startDate(startDate), _endDate(endDate) {}

        // Getter methods
        string getName() const {
            return _name;
        }

        Date getStartDate() const {
            return _startDate;
        }

        Date getEndDate() const {
            return _endDate;
        }
};

class CongregationManager{
    private:
        vector<Congregation> congs;
    public:
        void addCongregation(string name, string type, string sDate, string eDate)
};
