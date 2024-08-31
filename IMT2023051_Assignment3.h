#pragma once

#include <string>
#include <vector>

using namespace std; 

// Enum for status codes
enum status {
    OK,//prints 0 for success
    DUPLICATE_VENUE,
    NO_VENUE,
    INVALID_DATE_TIME,
    NO_EVENT,
    TIME_CONFLICT,
    MEMORY_ERROR,
    FORMAT_ERROR,
    NOPRINT_NEED,
    DUPLICATE_CONG_NAME,
    CONG_NOT_FOUND
};

class Date;
class Venue;
class VenueManager;
class CongregationManager;
class Congregation;
class Event;


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
    
    //Method to print date
    void printDate() const;
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
    // Destructor
    ~Congregation();//have to delete all the reservations inside the venues for this Congregation here
    // Getter methods
    string getName() const;
    string getType() const;
    Date getStartDate() const;
    Date getEndDate() const;
};

// CongregationManager class declaration
class CongregationManager {
private:
    vector<Congregation> congs;//congs.size() will now give the no. of present congregations

public:
    status addCongregation(string name, string type, Date startDate, Date endDate);
    status showCongregations() const;
    status delCongregation(string name);
};
