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

class Congregation{
    private:
        string _name;
        string _type;
        string _startDate;
        string _endDate;
        vector<Venue *> _reserved;
    public:
        Congregation(string name,string type, string startDate ,string endDate)
            :_name(name),_type(type),_startDate(startDate),_endDate(endDate){};
};
