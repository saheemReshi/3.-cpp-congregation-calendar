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
        if(cong.getName()==congName)
            start=cong.getStartDate();
            end=cong.getEndDate();
            return;
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
    auto it=venues.end()--;
    venues.insert(it,newVenue);
    // venues.push_back(newVenue);
    return OK;
}

status VenueManager::showVenues(string city, string state, string postalCode, string country) const{
    //if some parameter isn't given in the command for showVenues- I'll make it default to ""
    int matching=0;
    string s;//i'll store everything in this string till i iterate through the entire vector and then first print matching
    for(auto v:venues){
        if((city=="" || city==v.getCity()) && (state=="" || state==v.getState()) && (postalCode=="" || postalCode==v.getPostalCode()) && (country=="" || country==v.getCountry())){
            matching++;
            string temp=v.getName()+" "+v.getCity()+":"+v.getState()+":"+v.getState()+":"+v.getPostalCode()+":"+v.getCountry()+" "+to_string(v.getCapacity())+"\n";
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
    for(auto v:venues){
        if(vName==v.getName() && countryName==v.getCountry()){
            return v.addReservation(congName,start,end);
        }
    }
    return VENUE_MANAGER_RESERVE_VENUE_EXCEPTION;
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


// ** Calendar class methods
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
    status check=vm.reserveVenue(vName,countryName,congName,start,end);
    if(check==OK){
        //if no error occurs, then I'll also have to add that venue to the  vector<CongVenueResData> _reserved in Congregation class. This vector is useful for showReserved command for congregations

    }
}









int main(){//ibrahim said its necessary to make default constructor for VEnues and congr. bcz using a vector --pushback
    VenueManager a;
    a.addVenue("j","j","j","j","j","j",89);
    a.addVenue("a","j","j","j","j","j",89);
    a.addVenue("f","j","j","j","j","j",89);
    a.addVenue("s","j","j","j","j","j",89);
    a.addVenue("dd","j","j","j","j","j",89);
    a.addVenue("d","j","j","j","j","j",89);
    a.addVenue("e","j","j","j","j","j",89);
    a.addVenue("ew","j","j","j","j","j",89);
    a.addVenue("aa","j","j","j","j","j",89);
    a.addVenue("ffd","j","j","j","j","j",89);
    a.addVenue("sde","j","j","j","j","j",89);
    a.addVenue("ded","j","j","j","j","j",89);
    a.addVenue("dre","j","j","j","j","j",89);
    a.addVenue("eret","j","j","j","j","j",89);
    a.addVenue("ewww","j","j","j","j","j",89);
    a.showVenues("","","","");
    return 0;
}