#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
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

class Time;
class Date;
class Venue;
class VenueManager;
class CongregationManager;
class Congregation;
struct Event;
struct Reservation;


class Time {
private:
    int hour;
    int min; 

    bool isValidHour(int h) const {
        return h >= 0 && h <= 23;
    }

    bool isValidMinute(int m) const {
        return m >= 0 && m < 60 && (m % 15 == 0);
    }

public:
    // Constructor
    Time(int h, int m);

    // Getter methods
    int getHour() const;
    int getMinute() const;

    // Method to validate the time
    bool isValid() const;

    // Method to calculate the difference between two times in minutes
    int differenceInMinutes(const Time& other) const;//this will be useful if both the events are scheduled in the same day

    // Overloading the comparison operators
    bool operator<(const Time& other) const;
    bool operator<=(const Time& other) const;
    bool operator>(const Time& other) const;
    bool operator>=(const Time& other) const;
    bool operator==(const Time& other) const;
    bool operator!=(const Time& other) const;

    // for printing
    friend ostream& operator<<(ostream& os, const Time& time);
};

class Date {//this is a class for storing and validating dates
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
    friend ostream& operator<<(ostream& os, const Date& date);

    //operator overloading methods
    bool operator<(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator>=(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;

};


struct Event{
    Date d;
    string name;
    Time s,e;//start and end time
    Event *next=nullptr;//initialized to null by default

    Event(string name,Time s,Time e, Date d);
};

struct Reservation{
    //this will have the start and end date and the name of the congregation, which has reserved the venue
    //this will also store the linked list of events
    Date s,e;
    string congName;
    Event * eList=nullptr;
    Reservation * next=nullptr;//to make it a linked list of reservations
    //constructor
    Reservation(string name,Date s,Date e);

};


// Congregation class 
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

// CongregationManager class 
class CongregationManager {
private:
    vector<Congregation> congs;//congs.size() will now give the no. of present congregations

public:
    status addCongregation(string name, string type, Date startDate, Date endDate);
    status showCongregations() const;
    status delCongregation(string name);
};



//venue class

class Venue {
private:
    string name;
    string city;
    string addr;
    string state;
    string postalCode;
    string country;
    int capacity;
    Reservation * resList=nullptr;// this is to sort the reservation linked list in chronological order of dates
public:
    // Constructor
    Venue(string name, string city, string addr, string state, string postalCode, string country, int capacity);

    // Getter methods
    string getName() const;
    string getCity() const;
    string getAddr() const;
    string getState() const;
    string getPostalCode() const;
    string getCountry() const;
    int getCapacity() const;
};

