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



/*


int main() {
    // Create a Calendar object
    Calendar cal;

    // Test Case 1: Add Congregations
    cout << "Adding Congregations:" << endl;
    cout << "MadMax Tour India 2024: " << cal.addCongregation("MadMax Tour India 2024", "Concert", Date(26, 7, 2024), Date(29, 7, 2024)) << endl;
    cout << "Paris Olympics 2024: " << cal.addCongregation("Paris Olympics 2024", "Games", Date(26, 9, 2024), Date(5, 10, 2024)) << endl;
    cout << "Justin Bieber Tour: " << cal.addCongregation("Justin Bieber Tour", "Concert", Date(10, 10, 2024), Date(12, 10, 2024)) << endl;  // Adjusted to a valid date range
    cout << "Summer Fest 2025: " << cal.addCongregation("Summer Fest 2025", "Concert", Date(1, 6, 2025), Date(15, 6, 2025)) << endl;
    cout << "G20 US 2026: " << cal.addCongregation("G20 US 2026", "Convention", Date(28, 2, 2026), Date(29, 2, 2026)) << endl;  // Adjusted to a valid leap year date
    cout << endl;

    // Test Case 2: Show Congregations
    cout << "Showing Congregations:" << endl;
    cal.showCongregations();
    cout << endl;

    // Test Case 3: Add Venues
    cout << "Adding Venues:" << endl;
    cout << "Stade de France: " << cal.addVenue("Stade de France", "Bandra-East", "Saint-Denis", "Ile-de-France", "932100", "France", 80000) << endl;
    cout << "Wembley Stadium: " << cal.addVenue("Wembley Stadium", "Kormangala", "London", "England", "HA9000", "UK", 90000) << endl;
    cout << "Olympic Stadium: " << cal.addVenue("Olympic Stadium", "JPNagar", "Montreal", "Quebec", "H1V3N7", "Canada", 60000) << endl;
    cout << endl;

    // Test Case 4: Show Venues based on Location
    cout << "Showing Venues in London, England, UK:" << endl;
    cal.showVenues("London", "England", "", "UK");
    cout << endl;

    // Test Case 5: Reserve Venue
    cout << "Reserving Venues for Paris Olympics 2024:" << endl;
    cout << "Stade de France, France: " << cal.reserveVenue("Stade de France", "France", "Paris Olympics 2024") << endl;
    cout << "Stade de France, France: " << cal.reserveVenue("Stade de France", "France", "Paris Olympics 2024") << endl;  // Expecting failure due to duplicate reservation
    cout << "Wembley Stadium, UK: " << cal.reserveVenue("Wembley Stadium", "UK", "Paris Olympics 2024") << endl;
    cout << "Wembley Stadium, UK: " << cal.reserveVenue("Wembley Stadium", "UK", "MadMax Tour India 2024") << endl;
    cout << endl;

    // Test Case 6: Show Reservations for a Non-Existent Congregation
    cout << "Showing Reservations for Mars Olympics 1985:" << endl;
    cal.showReserved("Mars Olympics 1985");
    cout << endl;

    // Test Case 7: Show Reservations for an Existing Congregation
    cout << "Showing Reservations for Paris Olympics 2024:" << endl;
    cal.showReserved("Paris Olympics 2024");
    cout << endl;

    // Test Case 8: Add Events
    cout << "Adding Events for Paris Olympics 2024 at Stade de France:" << endl;
    cout << "Event 1: " << cal.addEvent("Opening Ceremony", "Paris Olympics 2024", "Stade de France", "France", Date(27, 9, 2024), Time(9, 0), Time(11, 0)) << endl;
    cout << "Event 2: " << cal.addEvent("Athletics - Morning Session", "Paris Olympics 2024", "Stade de France", "France", Date(27, 9, 2024), Time(11, 15), Time(13, 30)) << endl;
    cout << "Event 3: " << cal.addEvent("Athletics - Night Session", "Paris Olympics 2024", "Stade de France", "France", Date(27, 9, 2024), Time(20, 30), Time(23, 45)) << endl;
    cout << "Event 4: " << cal.addEvent("Football - Quarter Final", "Paris Olympics 2024", "Stade de France", "France", Date(28, 9, 2024), Time(0, 0), Time(2, 0)) << endl;
    cout << endl;

    // Test Case 9: Show Events for a Specific Date
    cout << "Showing Events for Stade de France, France on 2024-09-27:" << endl;
    cal.showEvents("Stade de France", "France", Date(27, 9, 2024));
    cout << endl;

    return 0;
}


*/



int main() {
    // Create a Calendar object
    Calendar cal;

    // Test Case 1: Add Congregations
    std::cout << "Adding Congregations:" << std::endl;
    std::cout << "Paris Olympics 2024: " 
              << cal.addCongregation("Paris Olympics 2024", "Games", Date(1, 10, 2024), Date(28, 10, 2024)) << std::endl;
    std::cout << "U-17 Football World Cup: " 
              << cal.addCongregation("U-17 Football World Cup", "Games", Date(1, 11, 2024), Date(15, 11, 2024)) << std::endl;
    std::cout << "Summer Fest: " 
              << cal.addCongregation("Summer Fest", "Concert", Date(1, 11, 2024), Date(5, 11, 2024)) << std::endl;
    std::cout << std::endl;

    // Test Case 2: Show Congregations
    std::cout << "Showing Congregations:" << std::endl;
    cal.showCongregations();
    std::cout << std::endl;

    // Test Case 3: Add Venues
    std::cout << "Adding Venues:" << std::endl;
    std::cout << "Paris stadium: " 
              << cal.addVenue("Paris stadium", "ParisStadium", "Paris", "Ile-de-France", "932100", "France", 20000) << std::endl;
    std::cout << "Lyon stadium: " 
              << cal.addVenue("Lyon stadium", "LyonStadium", "Lyon", "Auvergne-Rhône-Alpes", "69001", "France", 10000) << std::endl;
    std::cout << "Parc des Princes: " 
              << cal.addVenue("Parc des Princes", "FootballStadium", "Paris", "Ile-de-France", "75016", "France", 50000) << std::endl;
    std::cout << std::endl;

    // Test Case 4: Show Venues based on Location
    std::cout << "Showing Venues in Paris, Ile-de-France, France:" << std::endl;
    cal.showVenues("Paris", "Ile-de-France", "", "France");
    std::cout << std::endl;

    // Test Case 5: Reserve Venue
    std::cout << "Reserving Venues for Paris Olympics 2024:" << std::endl;
    std::cout << "Paris stadium, France: " 
              << cal.reserveVenue("Paris stadium", "France", "Paris Olympics 2024") << std::endl;
    std::cout << "Lyon stadium, France: " 
              << cal.reserveVenue("Lyon stadium", "France", "Paris Olympics 2024") << std::endl;
    std::cout << "Parc des Princes, France: " 
              << cal.reserveVenue("Parc des Princes", "France", "U-17 Football World Cup") << std::endl;
    std::cout << std::endl;

    // Test Case 6: Show Reservations for an Existing Congregation
    std::cout << "Showing Reservations for Paris Olympics 2024:" << std::endl;
    cal.showReserved("Paris Olympics 2024");
    std::cout << std::endl;

    // Test Case 7: Free a Venue
    std::cout << "Freeing Lyon stadium for Paris Olympics 2024:" << std::endl;
    std::cout << "Result: " 
              << cal.freeVenue("Lyon stadium", "France", "Paris Olympics 2024") << std::endl;
    std::cout << std::endl;

    // Test Case 8: Show Reservations for Paris Olympics 2024 After Freeing a Venue
    std::cout << "Showing Reservations for Paris Olympics 2024 (After Freeing Lyon stadium):" << std::endl;
    cal.showReserved("Paris Olympics 2024");
    std::cout << std::endl;

    // Test Case 9: Add Events to Venues
    std::cout << "Adding Events to Paris stadium for Paris Olympics 2024:" << std::endl;
    std::cout << "Javelin Throw (12:00 - 15:00, 2024-10-01): " 
              << cal.addEvent( "Javelin Throw","Paris Olympics 2024", "Paris stadium", "France", Date(1, 10, 2024), Time(12, 0), Time(15, 0)) << std::endl;
    std::cout << "Pole Vault (18:00 - 21:00, 2024-10-01): " 
              << cal.addEvent("Pole Vault","Paris Olympics 2024", "Paris stadium", "France", Date(1, 10, 2024), Time(18, 0), Time(21, 0)) << std::endl;
    std::cout << "5000m Mens (12:00 - 15:00, 2024-10-02): " 
              << cal.addEvent("5000m Mens","Paris Olympics 2024", "Paris stadium", "France", Date(2, 10, 2024), Time(12, 0), Time(15, 0)) << std::endl;
    std::cout << "Spain vs England (18:00 - 21:00, 2024-11-02): " 
              << cal.addEvent("Spain vs England","U-17 Football World Cup", "Parc des Princes", "France", Date(2, 11, 2024), Time(18, 0), Time(21, 0)) << std::endl;
    std::cout << std::endl;

    // Test Case 10: Show Events for a Venue on a Specific Date
    std::cout << "Showing Events for Paris stadium, France on 2024-10-01:" << std::endl;
    cal.showEvents("Paris stadium", "France", Date(1, 10, 2024));
    std::cout << std::endl;

    // Test Case 11: Show Events for Parc des Princes, France on 2024-11-02
    std::cout << "Showing Events for Parc des Princes, France on 2024-11-02:" << std::endl;
    cal.showEvents("Parc des Princes", "France", Date(2, 11, 2024));
    std::cout << std::endl;

    return 0;
}



