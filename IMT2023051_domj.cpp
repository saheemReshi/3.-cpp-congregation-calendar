// #pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <ctime>
using namespace std; 

// Enum for status codes
enum status {
    OK,//prints 0 for success
    DUPLICATE_VENUE,//used when venue names are same within a country
    NO_VENUE,
    INVALID_DATE_TIME,
    NO_EVENT,//to delete
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
    INVALID_TIME_LENGTH,//FOR addEvent min duration is 30min
    VENUE_HAS_RESERVATIONS,// venue has active or future reservations
};

class Time;
class Date;
class Venue;
class VenueManager;
class CongregationManager;
class Congregation;
class Calendar;
struct Decoder;//this will be used to parse the userInput string and see if date, time etc. are valid or not
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
    Date nextDay() const;//this method will give me the next date,uses chrono inside
    bool isPast() const;
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

//these 2 function convert input string to Date and Time
Date stringToDate(const string &dateStr);
Time stringToTime(const string &timeStr);
//this method is for location parsing
void parseLocation(const string &locationStr, string &address, string &city,string &state, string &postalCode, string &country);

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
    bool isActiveOrFuture() const;
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
    vector<CongVenueResData> getReservedList() const;

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
    //method to extract the list of reserved venues stored in a congregation. Useful for delCongr
    vector<CongVenueResData> getReservedList(string congName) const;
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

    bool hasActiveOrFutureReservations() const;
    //methods for dealing with reservations
    status addReservation(string congName,Date start, Date end);
    bool checkReservation(string congName, Date d, Time from, Time to) const;//helper method to check if a reservation for a cong exists for this venue
    //methods for dealing with events
    status addEvent(string eName,string congName, Date d, Time from, Time to);
    status showEvents(Date d) const;
    status deleteEvent(string congName, Date d, Time from, string eName);
    status freeReservation(string congName);
    status showCalendar(const string& congName, const Date& startDate, const Date& endDate) const;

    
};

class VenueManager{
private:
    vector<Venue> venues;

public:
    status addVenue(string name, string city, string addr, string state, string postalCode, string country, int capacity);
    status deleteVenue(string name, string country);
    status showVenues(string city, string state, string postalCode, string country) const;
    bool VenueExist(string vName,string country) const;
    status reserveVenue(string vName, string countryName, string congName,Date start,Date end);
    status freeVenue(string vName, string countryName, string congName);

    status addEvent(string eName, string congName, string vName, string countryName, Date d, Time from, Time to);
    status showEvents(string venueName, string countryName, Date d) const;
    status deleteEvent(string congName, string vName, string countryName, Date d, Time from, string eName);
    status showCalendar(const string& congName, const string& venueName, const string& venueCountry,Date startDate, Date endDate) const;

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
    status delVenue(string name, string country);

    status showReserved(string congName) const;
    status reserveVenue(string vName, string countryName, string congName);
    status freeVenue(string vName, string countryName, string congName);
    
    status addEvent(string eName,string congName, string vName, string countryName, Date d, Time from, Time t);
    status showEvents(string venueName, string venueCountry, Date d) const;
    status deleteEvent(string congName, string vName, string countryName, Date d, Time from, string eName);
    status showCalendar(const string& congName, const string& venueName, const string& venueCountry) const;

};


//Decoder struct

struct Decoder {
    Calendar manager;

    void printCode(status code);

    void addCongregation(istringstream &iss);
    void showCongregations();
    void addVenue(istringstream &iss);
    void showVenues(istringstream &iss);
    void delVenue(istringstream &iss);
    void reserveVenue(istringstream &iss);
    void showReserved(istringstream &iss);
    void freeVenue(istringstream &iss);
    void addEvent(istringstream &iss);
    void delEvent(istringstream &iss);
    void showEvents(istringstream &iss);
    void showCalendar(istringstream &iss);
    void deleteCongregation(istringstream &iss);
};


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
    return abs(thisTotalMinutes - otherTotalMinutes);
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
//** Utility functions for date and time and location parsing*/

Date stringToDate(const string &dateStr) {//this function isn't a method of Date class but is used to convert a string to Date
    int year, month, day;
    char delimiter;

    istringstream iss(dateStr);

    // Extract year, month, and day from the input string
    iss >> year >> delimiter >> month >> delimiter >> day;

    // Construct and return the Date object
    return Date(day, month,year);
}

Time stringToTime(const string &timeStr) {
    int hours, minutes;
    char colon;

    istringstream iss(timeStr);

    // Extract hours and minutes from the input string
    iss >> hours >> colon >> minutes;

    // Construct and return the Time object
    return Time(hours, minutes);
}
void parseLocation(const string &locationStr, string &address, string &city,string &state, string &postalCode, string &country) {
    istringstream iss(locationStr);
    getline(iss, address, ':');
    getline(iss, city, ':');
    getline(iss, state, ':');
    getline(iss, postalCode, ':');
    getline(iss, country);
}

void parseLocation_ShowVenues(const string &locationStr, string &city,string &state, string &postalCode, string &country) {
    istringstream iss(locationStr);
    // getline(iss, address, ':');
    getline(iss, city, ':');
    getline(iss, state, ':');
    getline(iss, postalCode, ':');
    getline(iss, country);
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
    os << date.year<<"-"<<(date.month<10?"0":"")<<date.month<<"-"<<(date.day<10?"0":"")<<date.day;
    return os;
}

//for checking if dates are adjacent
bool Date::isAdjacent(const Date& other) const {
    // If the dates are in the same year and month
    if (year == other.year && month == other.month) {
        return (day == other.day + 1) || (day + 1 == other.day);
    }
    // If the dates are in the same year but different months
    else if (year == other.year) {
        if ((month == other.month + 1) && (day == 1)) {
            return other.day == (isValidDay(31, other.month, year) ? 31 : 30);
        } else if ((month + 1 == other.month) && (other.day == 1)) {
            return day == (isValidDay(31, month, year) ? 31 : 30);
        }
    }
    // If the dates are in different years
    else if (year + 1 == other.year && month == 12 && other.month == 1) {
        return day == 31 && other.day == 1;
    } else if (year == other.year + 1 && month == 1 && other.month == 12) {
        return day == 1 && other.day == 31;
    }
    return false;
}

Date Date::nextDay() const {
    // Convert the Date object to a time_t
    std::tm time_in = {};
    time_in.tm_year = year - 1900; // years since 1900
    time_in.tm_mon = month - 1;    // months since January
    time_in.tm_mday = day;         // day of the month
    time_in.tm_hour = 0;
    time_in.tm_min = 0;
    time_in.tm_sec = 0;

    // Convert to time_t (this represents time in seconds since the epoch)
    std::time_t time_temp = std::mktime(&time_in);

    time_temp += 24 * 3600;
    //  Convert back to std::tm
    std::tm* time_out = std::localtime(&time_temp);

    //  Return the new Date object
    return Date(time_out->tm_mday, time_out->tm_mon + 1, time_out->tm_year + 1900);
}

bool Date::isPast() const {
    // Get current date
    time_t t = time(0);
    struct tm *now = localtime(&t);
    
    // Compare with current date
    if (year < (now->tm_year + 1900)) {
        return true;
    } else if (year == (now->tm_year + 1900)) {
        if (month < (now->tm_mon + 1)) {
            return true;
        } else if (month == (now->tm_mon + 1)) {
            if (day < now->tm_mday) {
                return true;
            }
        }
    }
    return false;
}

//Reservation methods

Reservation::Reservation(string name, Date s, Date e) 
    : congName(name), s(s), e(e) {}

bool Reservation::isActiveOrFuture() const {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* localTime = std::localtime(&currentTime);
    Date currentDate(localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900);
    return e >= currentDate; 
}

//Event class methods

Event::Event(string eName,string cName,Time s, Time e, Date d)
    :eName(eName),cName(cName),s(s),e(e),d(d) {}

bool Event::timeGap_30(const Event& other) const{
    //here I know that the two events don't overlap.I just need to find the time difference bw
    if(d==other.d){
        //both are on same day
        int diff=0;
        if(e<other.s){
            diff=e.differenceInMinutes(other.s);
        }
        else{
            diff=s.differenceInMinutes(other.e);
        }
        if(diff<30) return false;
    }    
    else if(d.isAdjacent(other.d)){
        //only need to check this case for this calendar
        Time e1(0,0),s2(0,0);//end time of first evnet and start time of next day event 
        if(d<other.d){
            e1=e;
            s2=other.s;
        }
        else{
            e1=other.e;
            s2=s;
        }
        //only one case of conflict bw two events on adjacent days- when one ends on 23:45 and other one starts on 00:00
        //this is because any event must end on 23:45 max bcz 23:59 is max time for a day, and start and end time must align in 15-min interval
        if(e1==Time(23,45) && s2==Time(00,00)){
            return false;
        }        
    }
    return true;
}





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

void Congregation::addReservation(const CongVenueResData& data ){
    //just does a simple push_back
    _reserved.push_back(data);//here I'm not checking if it already exists or not, because duplicate check was already made in addReservation
}

void Congregation::showReserved() const{
    // cout<<"hiya"<<endl;
    cout<<_reserved.size()<<endl;
    for(auto res:_reserved){
        cout<<res.name<<" "<<res.addr<<":"<<res.city<<":"<<res.state<<":"<<res.postalCode<<":"<<res.country<<" "<<res.capacity<<endl;
    }
}


status Congregation::removeReservation(const string& vName, const string& countryName){
    for (auto it = _reserved.begin(); it != _reserved.end(); ++it) {
        if (it->name == vName && it->country == countryName) {
            _reserved.erase(it);
            return OK;
        }
    }
    return NO_RESERVATION_OR_CONG;
}


vector<CongVenueResData> Congregation::getReservedList() const{
    return _reserved;
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
    cout<<congs.size()<<endl;
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
            congs.erase(it);
            return OK;
        }
        else{
            it++;
        }  
    }
    return CONG_NOT_FOUND;
}

bool CongregationManager::congExists(string name) const{
    //this is a helper method that i use to check if a congregation exists or not in commands like reserveVenue etc.
    //first I'll iterate through the list of congs and match it with the given name
    for(auto it=congs.begin(); it!=congs.end();){
        if(it->getName()==name){
            return true;
        }
        else{
            it++;
        }  
    }
    return false;
}

void CongregationManager::getDatesForCong(string congName, Date &start, Date &end) const{
    //Note: this function always expects the congName to exist in the vector
    for(auto cong:congs){
        if(cong.getName()==congName){
            start=cong.getStartDate();
            end=cong.getEndDate();
            return;
        }
    }
}

void CongregationManager::addReservationToCong(string congName, const CongVenueResData& data){
    for(auto &cong:congs){
        if(cong.getName()==congName){
            cong.addReservation(data);
            return;
        }
    }
}

status CongregationManager::showReserved(string congName) const{
    for(auto cong:congs){
        if(cong.getName()==congName){
            cong.showReserved();//will print everything here, no need to print anything else
            return NOPRINT_NEED;
        }
    }
    return CONG_NOT_FOUND;
}


status CongregationManager::removeReservationFromCongregation(string congName, string vName, string countryName){
    for (auto& cong : congs) {
        if (cong.getName() == congName) {
            return cong.removeReservation(vName, countryName);
        }
    }
    return CONG_NOT_FOUND;
}


vector<CongVenueResData> CongregationManager::getReservedList(string congName) const{
    //this method assumes that the congregation will surely exist
    for(auto cong:congs){
        if(cong.getName()==congName){
            return cong.getReservedList();
        }
    }
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





status Venue::addReservation(string congName,Date start, Date end){
    //here i'll try to add this cong Name in the resList
    // Check for conflicting or duplicate reservations
    Reservation* current = resList;
    Reservation* prev = nullptr;
    while (current != nullptr) {
        // Check if the new reservation is exactly the same as an existing one
        if (congName == current->congName && start == current->s && end == current->e) {
            return DUPLICATE_RESERVATION;
        }

        // Check if the new reservation conflicts with any existing ones
        if (start <= current->e && end >= current->s) {
            return TIME_CONFLICT;
        }

        // Find the correct position to insert the new reservation
        if (end < current->s) {
            break;
        }
        prev = current;
        current = current->next;
    }

    // Create the new reservation
    Reservation* newRes = new Reservation(congName, start, end);

    if (newRes == nullptr) {
        return MEMORY_ERROR;
    }

    // Insert the new reservation in the linked list
    if (prev == nullptr) { // Insert at the beginning
        newRes->next = resList;
        resList = newRes;
    } else { // Insert in the middle or at the end
        newRes->next = prev->next;
        prev->next = newRes;
    }

    return OK;
}

bool Venue::checkReservation(string congName, Date d, Time from, Time to) const {
    // Traverse the reservation list to check if a reservation exists for this congregation on the given date and time range
    Reservation* current = resList;
    while (current != nullptr) {
        if (current->congName == congName && d >= current->s && d <= current->e) {
            // Found a reservation for the congregation on the given date
            return true;
        }
        current = current->next;
    }
    return false; // No reservation found
}

status Venue::addEvent(string eName,string congName, Date d, Time from, Time to){
    // i'll ensure that no two events are scheduled with time difference less that 30 min
    int diff=from.differenceInMinutes(to);
    if(diff<30) return INVALID_TIME_LENGTH;
    
    //first I've to check if such a reservation exists for this cong at date d
    if(checkReservation(congName,d,from,to)==false){
        return NO_RESERVATION_OR_CONG;
    }
    
    // Create the new event
    Event* newEvent = new Event(eName, congName, from, to, d); // Placeholder event name, should be passed as a parameter

    Time s=from,e=to;
    //now I know that such a reservation exits for this congName, now i'll insert in the eventList,ensuring chronological order of date and time
    Event *prev=nullptr,*current=eList;
    while(current!=nullptr && ((current->d)<d)){
        prev=current;
        current=current->next;
    }
    // cout<<"In Venue::addEvent"<<endl;
        // cout<<"move on condn"<<endl;
    while(current!=nullptr){//here I dont need to break out if date >d bcz till then if event isn't inserted, TIME_CONFLICT will be retruned
        if(current->e<s){
            //move on condn
            prev=current;
            current=current->next; 
        }
        else if(e>=current->s && current->e>=s){
            delete newEvent;
            return TIME_CONFLICT_EVENT;
        }
        else{
            //here I'll insert after checking if 30 min time diff bw prev and current events
            if(prev->timeGap_30(*newEvent)==false || current->timeGap_30(*newEvent)==false){
                delete newEvent;
                return TIME_CONFLICT_EVENT_30MIN;
            }
            //now insert here after breaking
            break;
        }
    }
    if (prev == nullptr) { // Insert at the beginning
        newEvent->next = eList;
        eList = newEvent;
    } else if(current){ // Insert in the middle
        newEvent->next = prev->next;
        prev->next = newEvent;
    }else{
        //insert in end
        if(prev->timeGap_30(*newEvent)==false ){
            delete newEvent;
            return TIME_CONFLICT_EVENT_30MIN;
        }
        newEvent->next = prev->next;
        prev->next = newEvent;
    }
    // newEvent->next=current;
    // prev->next=newEvent;
    return OK;
}

status Venue::showEvents(Date d) const {
    // Pointer to traverse the event list
    Event* current = eList;
    int eventCount = 0;
    vector<Event*> eventsOnDate; // To store events for the specified date

    // Traverse the event list to find events on the specified date
    while (current != nullptr) {
        if (current->d == d) { // If the date matches, store the event
            eventsOnDate.push_back(current);
            ++eventCount;
        }
        current = current->next;
    }

    // Print the number of events found
    cout << eventCount << endl;

    // Print event details if any events were found
    for (Event* event : eventsOnDate) {
        cout << event->eName << " " << event->s << " " << event->e << endl;
    }

    return NOPRINT_NEED;
}


status Venue::freeReservation(string congName) {
    Reservation* current = resList;
    Reservation* prev = nullptr;

    while (current != nullptr) {
        if (current->congName == congName) {
            // Remove the reservation
            if (prev == nullptr) {
                resList = current->next;  // Remove the first node
            } else {
                prev->next = current->next;  // Bypass the current node
            }
            delete current;

            // Now remove all events associated with this congregation
            Event* eCurrent = eList;
            Event* ePrev = nullptr;

            while (eCurrent != nullptr) {
                if (eCurrent->cName == congName) {
                    if (ePrev == nullptr) {
                        eList = eCurrent->next;  // Remove the first event
                    } else {
                        ePrev->next = eCurrent->next;  // Bypass the current event
                    }
                    Event* temp = eCurrent;
                    eCurrent = eCurrent->next;
                    delete temp;  // Delete the event
                } else {
                    ePrev = eCurrent;
                    eCurrent = eCurrent->next;
                }
            }

            return OK;  // Successfully removed the reservation and associated events
        }
        prev = current;
        current = current->next;
    }
    return NO_RESERVATION_OR_CONG;  // No matching reservation found
}


status Venue::deleteEvent(string congName, Date d, Time from, string eName) {
    if (eList == nullptr) {
        return NO_EVENT; // No events to delete
    }

    Event *prev = nullptr;
    Event *current = eList;

    while (current != nullptr) {
        if (current->cName == congName && current->d == d && current->s == from && current->eName == eName) {
            // Found the event to delete
            if (prev == nullptr) {
                eList = current->next; // If the event is the first in the list
            } else {
                prev->next = current->next; // Bypass the current event
            }
            delete current; // Free memory
            return OK; // Success
        }
        prev = current;
        current = current->next;
    }

    return NO_EVENT; // Event not found
}

bool Venue::hasActiveOrFutureReservations() const {
    Reservation* current = resList;
    while (current != nullptr) {
        if (current->isActiveOrFuture()) {
            return true;
        }
        current = current->next;
    }
    return false;
}


status Venue::showCalendar(const string& congName, const Date& startDate, const Date& endDate) const {
    // Check if the venue has a reservation for this congregation in the given date range
    Reservation* res = resList;
    bool reservationExists = false;
    
    while (res != nullptr) {
        if (res->congName == congName && res->s <= endDate && res->e >= startDate) {
            reservationExists = true;
            break;
        }
        res = res->next;
    }

    if (!reservationExists) {
        return NO_RESERVATION_OR_CONG; // No reservation found
    }
    //i need to print totalEvents on the first line, so I'll traverse through the eList twice
    Event* event = eList;
    int totalEvents = 0;
    while (event != nullptr) {
        if (event->cName == congName) {
            totalEvents++;
        }
        event = event->next;
    }
    //now i have the total number of events
    cout<<totalEvents<<endl;

    Date currentDate = startDate;
    while (currentDate <= endDate) {
        int eventsOnThisDate = 0;
        Event* event = eList;
        // Count events for the current date
        while (event != nullptr) {
            if (event->cName == congName && event->d == currentDate) {
                eventsOnThisDate++;
            }
            event = event->next;
        }

        // Print date and number of events
        cout << currentDate << " " << eventsOnThisDate << endl;

        // Print event details if any
        event = eList;
        while (event != nullptr) {
            if (event->cName == congName && event->d == currentDate) {
                cout << event->eName << " " << event->s << " " << event->e << endl;
            }
            event = event->next;
        }

        // Move to the next date
        currentDate = currentDate.nextDay(); // Assuming you have a method to get the next day
    }
    return NOPRINT_NEED;
}

//VenueManager class methods

status VenueManager::addVenue(string name, string city, string addr, string state, string postalCode, string country, int capacity){
    //venue names must be unique within a country
    //so i'll have to iterate through the vector venues and check all the venues for same name and coutry
    for(auto v:venues){
        if(v.getName()==name && v.getCountry()==country){
            return DUPLICATE_VENUE;
        }
    }
    //now I can directly add
    Venue newVenue=Venue(name,city,addr,state,postalCode,country,capacity);
    // auto it=venues.end()--;
    // venues.insert(it,newVenue);
    venues.push_back(newVenue);
    return OK;
}

status VenueManager::deleteVenue(string name, string country) {
    for (auto it = venues.begin(); it != venues.end(); ++it) {
        auto venue = it;
        if (venue->getName() == name && venue->getCountry() == country) {
            if (venue->hasActiveOrFutureReservations()) {
                return VENUE_HAS_RESERVATIONS;
            }
            venues.erase(it);
            return OK;
        }
    }
    return NO_VENUE;
}

status VenueManager::showVenues(string city, string state, string postalCode, string country) const{
    //if some parameter isn't given in the command for showVenues- I'll make it default to ""
    int matching=0;
    string s;//i'll store everything in this string till i iterate through the entire vector and then first print matching
    for(auto v:venues){
        if((city=="" || city==v.getCity()) && (state=="" || state==v.getState()) && (postalCode=="" || postalCode==v.getPostalCode()) && (country=="" || country==v.getCountry())){
            matching++;
            string temp=v.getName()+" "+v.getAddr()+":"+v.getCity()+":"+v.getState()+":"+v.getPostalCode()+":"+v.getCountry()+" "+to_string(v.getCapacity())+"\n";
            s+=temp;
        }
    }
    cout<<matching<<endl;
    cout<<s;
    return NOPRINT_NEED;
}


bool VenueManager::VenueExist(string vName, string country) const{
    //this is a helper fuction which will tell me if such a venue exists or not
    for(auto v:venues){
        if(vName==v.getName() && country==v.getCountry()){
            return true;
        }
    }
    return false;
}


status VenueManager::reserveVenue(string vName, string countryName, string congName, Date start, Date end){
    //here I'll find the venue given in command and then call methods of Venue class to do the rest
    for(auto &v:venues){
        if(vName==v.getName() && countryName==v.getCountry()){
            return v.addReservation(congName,start,end);
        }
    }
    return VENUE_MANAGER_RESERVE_VENUE_EXCEPTION;
}

status VenueManager::freeVenue(string vName, string countryName, string congName) {
    for (Venue &venue : venues) {
        if (venue.getName() == vName && venue.getCountry() == countryName) {
            return venue.freeReservation(congName);
        }
    }
    return NO_VENUE;  // Venue not found
}



CongVenueResData VenueManager::getDetailsOfVenue(string vName, string countryName) const{
    for(auto v:venues){
        if(v.getName()==vName && v.getCountry()==countryName){
            //this should always get executed as I had checked earlier that such a venue exists            
            return {v.getName(),v.getCity(),v.getAddr(),v.getState(),v.getPostalCode(),v.getCountry(),v.getCapacity()};
        }
    }
    //just for error checking
    string a="ErrorInVM::getDetailsOfVenue";
    return {a,a,a,a,a,a,0};
}


status VenueManager::addEvent(string eName,string congName, string vName, string countryName, Date d, Time from, Time to){
    //here I'll search for the venue
    for(auto &v:venues){
        if(v.getName()==vName && v.getCountry()==countryName){
            //now pass this to the Venue class
            // cout<<"In VM::addEvent"<<endl;

            return v.addEvent(eName,congName,d,from,to);
        }
    }
    return NO_VENUE;//when no such venue exists
}

status VenueManager::showEvents(string venueName, string countryName, Date d) const {
    // Find the venue in the vector
    for (auto venue : venues) {
        if (venue.getName() == venueName && venue.getCountry() == countryName) {
            // Call the showEvents method of the Venue class
            return venue.showEvents(d);
        }
    }
    return NO_VENUE; // If venue was not found in the vector
}

status VenueManager::deleteEvent(string congName, string vName, string countryName, Date d, Time from, string eName) {
    for (auto& venue : venues) {
        if (venue.getName() == vName && venue.getCountry() == countryName) {
            return venue.deleteEvent(congName, d, from, eName); // Delegate to Venue's deleteEvent method
        }
    }
    return NO_VENUE; // Venue not found
}


status VenueManager::showCalendar(const string& congName, const string& venueName, const string& venueCountry,Date startDate, Date endDate) const{
    //here i'll search for this venue 
    for(auto v:venues){
        if(v.getName()==venueName && v.getCountry()==venueCountry){
            return v.showCalendar(congName,startDate,endDate);
        }
    }
    return NO_VENUE;    
}



// ** Calendar class methods

status Calendar::addCongregation(string name, string type, Date startDate, Date endDate) {
    return cm.addCongregation(name, type, startDate, endDate);
}

status Calendar::showCongregations() const {
    return cm.showCongregations();
}

status Calendar::delCongregation(string name) {
    if(cm.congExists(name)==false) return CONG_NOT_FOUND;
    //here first i'll extract the _reserved vector from this congregation
    vector<CongVenueResData> freeCongVenues=cm.getReservedList(name);
    //now i'll call freeVenue on all these venues for this congregation
    for(auto it:freeCongVenues){
        vm.freeVenue(it.name,it.country,name);//all the venues associated with this congr. are getting freed and all the events are being deleted
    }
    cm.delCongregation(name);
    return OK;
}

status Calendar::addVenue(string name, string addr,string city,  string state, string postalCode, string country, int capacity) {
    return vm.addVenue(name, city, addr, state, postalCode, country, capacity);
}

status Calendar::showVenues(string city, string state, string postalCode, string country) const {
        return vm.showVenues(city, state, postalCode, country);
}

status Calendar::delVenue(string name, string country){
    return vm.deleteVenue(name,country);
}


status Calendar::showReserved(string congName) const {
    return cm.showReserved(congName);
}


status Calendar::reserveVenue(string vName, string countryName, string congName){
    //here i'll first check if such a venue and country exist or not
    if(vm.VenueExist(vName,countryName)==false){
        //such a venue doesn't exist
        return NO_VENUE;
    }
    if(cm.congExists(congName)==false){
        //such a congregation doesn't exist
        return CONG_NOT_FOUND;
    }
    //now we know that both venue and congregation exist
    //now I'll call methods of CongregationManager and VenueManager that will try to reserse the venue
    
    //only two errors left to deal with
    //first error-this reservation has already happened for this congregation or this date slot is occupied by some other congregation
    //I'll deal with both of these errors in VenueManager::reserveVenue method

    Date start,end;
    cm.getDatesForCong(congName,start,end);//using references for returning multiple 
    // cout<<"hiya"<<endl;
    // cout<<start<<"  "<<end<<endl;
    status check=vm.reserveVenue(vName,countryName,congName,start,end);
    // cout<<"hiya--"<<check<<endl;
    if(check==OK){
        //if no error occurs, then I'll also have to add that venue to the  vector<CongVenueResData> _reserved in Congregation class. This vector is useful for showReserved command for congregations
        CongVenueResData temp=vm.getDetailsOfVenue(vName,countryName);
        cm.addReservationToCong(congName,temp);
    }
    return check;
}



status Calendar::addEvent(string eName, string congName, string vName, string countryName, Date d, Time from, Time to){
    /*addEvent <congregation-name-string> <venueName-string> <venueCountry-string> <date-string> 
    <fromTime-string> <toTime-string> <eventName-string>*/
    // cout<<"In Cal::addEvent"<<endl;
    //here first I've to search for the venue so I'll pass this to the venueManager
    return vm.addEvent(eName,congName,vName,countryName,d,from,to);
}


status Calendar::showEvents(string venueName, string venueCountry, Date d) const {
    return vm.showEvents(venueName,venueCountry,d);

}

status Calendar::freeVenue(string vName, string countryName, string congName) {
    status venueStatus = vm.freeVenue(vName, countryName, congName);
    
    if (venueStatus == OK) {
        // Also remove the reservation from the congregation's _reserved vector
        status congStatus = cm.removeReservationFromCongregation(congName, vName, countryName);
        
        if (congStatus != OK) {
            return congStatus;  // Return error if reservation removal fails
        }
    }

    return venueStatus;  // Return the status of the venue freeing operation
}

status Calendar::deleteEvent(string congName, string vName, string countryName, Date d, Time from, string eName) {
    return vm.deleteEvent(congName, vName, countryName, d, from, eName);
}

status Calendar::showCalendar(const string& congName, const string& venueName, const string& venueCountry) const {
    //first I'll check if the congr exists or not
    if(cm.congExists(congName)==false){
        return CONG_NOT_FOUND;
    }
    
    // Get the start and end dates for the congregation
    Date startDate, endDate;
    cm.getDatesForCong(congName, startDate, endDate);

    //now I'll search for the venue in the venueManager class
    return vm.showCalendar(congName,venueName,venueCountry,startDate,endDate);

}


//struct Decoder methods

void Decoder::printCode(status code) {
    if (code == OK) cout << 0 << endl;
    else if (code == NOPRINT_NEED) cout <<"";
    else cout<<-1<<"\nError"<<endl;
//     else if (code == DUPLICATE_CONG_NAME) cout << "-1\nError: This congregation already exists" << endl;
//     else if (code == DUPLICATE_VENUE) cout << "-1\nError: This venue already exists" << endl;
//     else if (code == NO_VENUE) cout << "-1\nError: No such venue" << endl;
//     else if (code == INVALID_DATE_TIME) cout << "-1\nError: Invalid date or time" << endl;
//     else if (code == CONG_NOT_FOUND) cout << "-1\nError: No such congregation" << endl;
//     else if (code == TIME_CONFLICT) cout << "-1\nError: Time slot is occupied" << endl;
//     else if (code == TIME_CONFLICT_EVENT) cout << "-1\nError: Event time slot is occupied" << endl;
//     else if (code == TIME_CONFLICT_EVENT_30MIN) cout << "-1\nError: Event time slot is occupied within 30 minutes interval" << endl;
//     else if (code == INVALID_TIME_LENGTH) cout << "-1\nError: Event duration is less than 30 minutes" << endl;
//     else if (code == MEMORY_ERROR) cout << "-1\nError: Out of memory" << endl;
//     else if (code == FORMAT_ERROR) cout << "-1\nError: Format error" << endl;
//     else if (code == VENUE_MANAGER_RESERVE_VENUE_EXCEPTION) cout << "-1\nError: Exception occurred while reserving venue" << endl;
//     else if (code == DUPLICATE_RESERVATION) cout << "-1\nError: Duplicate reservation" << endl;
//     else if (code == NO_RESERVATION_OR_CONG) cout << "-1\nError: No reservation or congregation found" << endl;
//     else if (code == VENUE_HAS_RESERVATIONS) cout << "-1\nError: Venue has active or future reservations" << endl;
}


void Decoder::addCongregation(istringstream &iss) {
    string name, type, startDate, endDate;

    // Parse congregation name
    iss >> ws;
    getline(iss, name, '"');
    getline(iss, name, '"');

    // Parse congregation type
    iss >> ws;
    getline(iss, type, '"');
    getline(iss, type, '"');

    // Parse start date
    iss >> ws;
    getline(iss, startDate, '"');
    getline(iss, startDate, '"');

    // Parse end date
    iss >> ws;
    getline(iss, endDate, '"');
    getline(iss, endDate, '"');

    // Convert strings to Date objects
    Date start = stringToDate(startDate);
    Date end = stringToDate(endDate);
    // cout<<start<<endl<<end<<endl;
    //I need to validate the dates
    if(start.isValid()==0 || end.isValid()==0 || start>end){
        // cout<<"hoal"<<endl;
        printCode(INVALID_DATE_TIME);
        return;
    }
    //i also have to make sure that I never add a congregation to the past
    if(end.isPast()==true){
        printCode(INVALID_DATE_TIME);
        return;
    }
    // Add the congregation through the manager
    status code = manager.addCongregation(name, type, start, end);
    printCode(code);
}

void Decoder::deleteCongregation(istringstream &iss) {
    string cName;
    iss >> ws;
    getline(iss, cName, '"');
    getline(iss, cName, '"');

    status code = manager.delCongregation(cName);
    printCode(code);
}

void Decoder::showCongregations() {
    manager.showCongregations();//returns NOPRINT_NEED 
}



void Decoder::addVenue(istringstream &iss) {
    string vName, loc;
    int cap;
    iss >> ws;
    getline(iss, vName, '"');
    getline(iss, vName, '"');
    iss >> ws;
    getline(iss, loc, '"');
    getline(iss, loc, '"');
    iss >> ws >> cap;
    //here I also need to parse the location string
    string address, city, state, postalCode, country;
    parseLocation(loc, address, city, state, postalCode, country);
    status code = manager.addVenue(vName, address, city, state, postalCode, country, cap);
    printCode(code);
}

void Decoder::delVenue(istringstream &iss) {
    string vName,country;
    iss >> ws;
    getline(iss, vName, '"');
    getline(iss, vName, '"');
    iss >> ws;
    getline(iss, country, '"');
    getline(iss, country, '"');

    status code = manager.delVenue(vName,country);
    printCode(code);
}

void Decoder::showVenues(istringstream &iss) {
    string locationStr, city, state, postalCode, country;

    // Read location string
    iss >> ws;
    getline(iss, locationStr, '"');
    getline(iss, locationStr, '"');

    // Parse the location string into components
    parseLocation_ShowVenues(locationStr, city, state, postalCode, country);

    // Call the manager's showVenues method
    manager.showVenues(city, state, postalCode, country);
}



void Decoder::reserveVenue(istringstream &iss) {
    string vName, country, cName;
    iss >> ws;
    getline(iss, vName, '"');
    getline(iss, vName, '"');
    iss >> ws;
    getline(iss, country, '"');
    getline(iss, country, '"');
    iss >> ws;
    getline(iss, cName, '"');
    getline(iss, cName, '"');

    status code = manager.reserveVenue(vName, country, cName);
    printCode(code);
}

void Decoder::showReserved(istringstream &iss) {
    string cName;
    iss >> ws;
    getline(iss, cName, '"');
    getline(iss, cName, '"');

    status code = manager.showReserved(cName);
    printCode(code);
}

void Decoder::freeVenue(istringstream &iss) {
    string vName, country, cName;
    iss >> ws;
    getline(iss, vName, '"');
    getline(iss, vName, '"');
    iss >> ws;
    getline(iss, country, '"');
    getline(iss, country, '"');
    iss >> ws;
    getline(iss, cName, '"');
    getline(iss, cName, '"');

    status code = manager.freeVenue(vName, country, cName);
    printCode(code);
}



void Decoder::addEvent(istringstream &iss) {
    string congregationName, venueName, venueCountry, dateStr, fromTimeStr, toTimeStr, eventName;

    // Parse congregation name
    iss >> ws;
    getline(iss, congregationName, '"');
    getline(iss, congregationName, '"');

    // Parse venue name
    iss >> ws;
    getline(iss, venueName, '"');
    getline(iss, venueName, '"');

    // Parse venue country
    iss >> ws;
    getline(iss, venueCountry, '"');
    getline(iss, venueCountry, '"');

    // Parse date
    iss >> ws;
    getline(iss, dateStr, '"');
    getline(iss, dateStr, '"');

    // Parse start time
    iss >> ws;
    getline(iss, fromTimeStr, '"');
    getline(iss, fromTimeStr, '"');

    // Parse end time
    iss >> ws;
    getline(iss, toTimeStr, '"');
    getline(iss, toTimeStr, '"');

    // Parse event name
    iss >> ws;
    getline(iss, eventName, '"');
    getline(iss, eventName, '"');

    // Convert strings to Date and Time objects
    Date date = stringToDate(dateStr);
    Time fromTime = stringToTime(fromTimeStr);
    Time toTime = stringToTime(toTimeStr);

    //I need to validate the dates
    if(date.isValid()==0 || fromTime.isValid()==0 || toTime.isValid()==0 || fromTime>=toTime){
        printCode(INVALID_DATE_TIME);
        return;
    }
    // Add the event through the manager
    status code = manager.addEvent(eventName,congregationName, venueName, venueCountry, date, fromTime, toTime);
    printCode(code);
}

void Decoder::showEvents(istringstream &iss) {
    string venueName, venueCountry, dateStr;

    // Parse venue name
    iss >> ws;
    getline(iss, venueName, '"');
    getline(iss, venueName, '"');

    // Parse venue country
    iss >> ws;
    getline(iss, venueCountry, '"');
    getline(iss, venueCountry, '"');

    // Parse date
    iss >> ws;
    getline(iss, dateStr, '"');
    getline(iss, dateStr, '"');

    // Convert string to Date object
    Date date = stringToDate(dateStr);

    //I need to validate the dates
    if(date.isValid()==0 ){
        printCode(INVALID_DATE_TIME);
        return;
    }
    // Show events through the manager
    status code=manager.showEvents(venueName, venueCountry, date);
    printCode(code);
}

void Decoder::delEvent(istringstream &iss) {
    string congregationName, venueName, venueCountry, dateStr, fromTimeStr, eventName;

    // Parse congregation name
    iss >> ws;
    getline(iss, congregationName, '"');
    getline(iss, congregationName, '"');

    // Parse venue name
    iss >> ws;
    getline(iss, venueName, '"');
    getline(iss, venueName, '"');

    // Parse venue country
    iss >> ws;
    getline(iss, venueCountry, '"');
    getline(iss, venueCountry, '"');

    // Parse date
    iss >> ws;
    getline(iss, dateStr, '"');
    getline(iss, dateStr, '"');

    // Parse start time
    iss >> ws;
    getline(iss, fromTimeStr, '"');
    getline(iss, fromTimeStr, '"');

    // Parse event name
    iss >> ws;
    getline(iss, eventName, '"');
    getline(iss, eventName, '"');

    // Convert strings to Date and Time objects
    Date date = stringToDate(dateStr);
    Time fromTime = stringToTime(fromTimeStr);

    // Validate the date and time
    if (date.isValid() == 0 || fromTime.isValid() == 0) {
        printCode(INVALID_DATE_TIME);
        return;
    }

    // Delete the event through the manager
    status code = manager.deleteEvent(congregationName, venueName, venueCountry, date, fromTime, eventName);
    printCode(code);
}

void Decoder::showCalendar(istringstream &iss) {
    string cName, vName, vCountry;
    iss >> ws;
    getline(iss, cName, '"');
    getline(iss, cName, '"');
    iss >> ws;
    getline(iss, vName, '"');
    getline(iss, vName, '"');
    iss >> ws;
    getline(iss, vCountry, '"');
    getline(iss, vCountry, '"');

    status code = manager.showCalendar(cName, vName, vCountry);
    printCode(code);
}

int main() {
    string userInput;
    Decoder dec; // Create an instance of the Decoder class

    while (true) {
        getline(cin, userInput);
        istringstream iss(userInput);
        string command;
        iss >> ws >> command; // Read the command

        if (command == "End") {
            // Exit condition
            break;
        } else if (command == "addCongregation") {
            dec.addCongregation(iss);
        } else if (command == "showCongregations") {
            dec.showCongregations();
        } else if (command == "addVenue") {
            dec.addVenue(iss);
        } else if (command == "showVenues") {
            dec.showVenues(iss);
        } else if (command == "reserveVenue") {
            dec.reserveVenue(iss);
        } else if (command == "showReserved") {
            dec.showReserved(iss);
        } else if (command == "freeVenue") {
            dec.freeVenue(iss);
        } else if (command == "addEvent") {
            dec.addEvent(iss);
        } else if (command == "showEvents") {
            dec.showEvents(iss);
        } else if (command == "deleteEvent") {
            dec.delEvent(iss);
        } else if (command == "deleteVenue") {
            dec.delVenue(iss);
        } else if (command == "deleteCongregation") {
            dec.deleteCongregation(iss);
        } else if (command == "showCalendar") {
            dec.showCalendar(iss);
        } else {
            cout << "-1\nError: No such command" << endl;
        }
    }

    return 0;
}
