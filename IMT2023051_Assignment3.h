#pragma once

#include <string>
#include <vector>

using namespace std; 

// Enum for status codes
enum status {
    OK,
    DUPLICATE_VENUE,
    NO_VENUE,
    INVALID_DATE_TIME,
    NO_EVENT,
    TIME_CONFLICT,
    MEMORY_ERROR,
    FORMAT_ERROR,
    NOPRINT_NEED
};

// Date class declaration
class Date {
private:
    int day;
    int month;
    int year;

    bool isLeapYear(int year) const;
    bool isValidDay(int day, int month, int year) const;

public:
    // Constructor
    Date(int day, int month, int year);

    // Getter methods
    int getDay() const;
    int getMonth() const;
    int getYear() const;

    // Method to validate the date
    bool isValid() const;
};

class Venue;

// Congregation class declaration
class Congregation {
private:
    string _name;
    string _type;
    Date _startDate;
    Date _endDate;
    vector<Venue *> _reserved;

public:
    // Constructor
    Congregation(string name, string type, Date startDate, Date endDate);

    // Getter methods
    string getName() const;
    Date getStartDate() const;
    Date getEndDate() const;
};

// CongregationManager class declaration
class CongregationManager {
private:
    vector<Congregation> congs;

public:
    void addCongregation(string name, string type, Date startDate, Date endDate);
};
