#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std; 

// Enum for status codes
enum status {
    OK,//prints 0 for success
    DUPLICATE_VENUE,//used when venue names are same within a country
    NO_VENUE,
    INVALID_DATE_TIME,
    NO_EVENT,
    TIME_CONFLICT,
    TIME_CONFLICT_EVENT,//specially for events
    TIME_CONFLICT_EVENT_30MIN,
    MEMORY_ERROR,
    FORMAT_ERROR,
    NOPRINT_NEED,
    DUPLICATE_CONG_NAME,
    CONG_NOT_FOUND,
    VENUE_MANAGER_RESERVE_VENUE_EXCEPTION,
    DUPLICATE_RESERVATION,
    NO_RESERVATION_OR_CONG,
    INVALID_TIME_LENGTH//FOR addEvent min duration is 30min
};

class Time;
class Date;
class Venue;
class VenueManager;
class CongregationManager;
class Congregation;
class Decoder;//this will be used to parse the userInput string and see if date, time etc. are valid or not
class Calendar;
struct Event;
struct Reservation;
struct CongVenueResData;

struct CongVenueResData{
    //this is just a simple struct which stores the data members of a venue
    //it will be used in storing the details of all the venues that a congregation has reserved-(inside the congregation class)
    string name;
    string city;
    string addr;
    string state;
    string postalCode;
    string country;
    int capacity;
};


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
    Date()=default;

    // Getter methods
    int getDay() const;
    int getMonth() const;
    int getYear() const;

    // Method to validate the date
    bool isValid() const;
    bool isAdjacent(const Date& other) const;

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
    string eName;
    string cName;//cong to which the event belongs to
    Time s,e;//start and end time
    Event *next=nullptr;//initialized to null by default

    Event(string eName,string cName,Time s,Time e, Date d);
    bool timeGap_30(const Event& other) const;
};

struct Reservation{
    //this will have the start and end date and the name of the congregation, which has reserved the venue
    Date s,e;
    string congName;
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
    vector<CongVenueResData > _reserved;//this will store the info of all the venues that this congr. has reserved//this will be sorted in chronological order of dates

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
    void addReservation(const CongVenueResData& data);
    void showReserved() const;
    status removeReservation(const string& vName, const string& countryName);

};

// CongregationManager class 
class CongregationManager {
private:
    vector<Congregation> congs;//congs.size() will now give the no. of present congregations

public:
    status addCongregation(string name, string type, Date startDate, Date endDate);
    status showCongregations() const;
    status delCongregation(string name);
    status showReserved(string congName) const;
    bool congExists(string name) const;
    void getDatesForCong(string congName, Date &start, Date &end) const;//here I'm using references for getting the start and end date of a particular congregaion.
    void addReservationToCong(string congName, const CongVenueResData& data);

    // Method to remove a reservation from the `_reserved` vector of a congregation// this will be called when freeVenue will be called
    status removeReservationFromCongregation(string congName, string vName, string countryName);
       
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
    Event * eList=nullptr;//this linked list will store all the events in chronological order of date and time
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

    // bool timegap_30(Date d1,Date d2, Time t1, Time t2) const;
    //methods for dealing with reservations
    status addReservation(string congName,Date start, Date end);
    bool checkReservation(string congName, Date d, Time from, Time to) const;//helper method to check if a reservation for a cong exists for this venue
    //methods for dealing with events
    status addEvent(string eName,string congName, Date d, Time from, Time to);
    status showEvents(Date d) const;
    status freeReservation(string congName);
    
};

class VenueManager{
private:
    vector<Venue> venues;

public:
    status addVenue(string name, string city, string addr, string state, string postalCode, string country, int capacity);
    status delVenue(string vName, string countryName);//a venue cant be deleted if it has any active or future reservations
    status showVenues(string city, string state, string postalCode, string country) const;
    bool VenueExist(string vName,string country) const;
    status reserveVenue(string vName, string countryName, string congName,Date start,Date end);
    status freeVenue(string vName, string countryName, string congName);

    status addEvent(string eName, string congName, string vName, string countryName, Date d, Time from, Time to);
    status showEvents(string venueName, string countryName, Date d) const;

    CongVenueResData getDetailsOfVenue(string vName, string countryName) const;
};

//Calendar class

class Calendar{
private:
    VenueManager vm;
    CongregationManager cm;
public:
    //I will ensure that date and time are valid in the Decoder class itself
    status addCongregation(string name, string type, Date startDate, Date endDate);
    status showCongregations() const;
    status delCongregation(string name);
    
    status addVenue(string name, string addr, string city,  string state, string postalCode, string country, int capacity);
    status showVenues(string city, string state, string postalCode, string country) const;

    status showReserved(string congName) const;
    status reserveVenue(string vName, string countryName, string congName);
    status freeVenue(string vName, string countryName, string congName);
    
    status addEvent(string eName,string congName, string vName, string countryName, Date d, Time from, Time t);
    status showEvents(string venueName, string venueCountry, Date d) const;

};