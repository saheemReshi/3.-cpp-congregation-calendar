#pragma once

using namespace std; 

struct Event;
struct Reservation;

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