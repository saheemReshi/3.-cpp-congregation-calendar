#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "IMT2023051_Assignment3.h"
using namespace std; 

struct Event;
struct Reservation;

class Event{
    Date d;
    string name;
    Time s,e;//start and end time
    Event *next=nullptr;//initialized to null by default

    Event(string name,Time s,Time e);
};

class Reservation{
    //this will have the start and end date and the name of the congregation, which has reserved the venue
    //this will also store the linked list of events
    Date s,e;
    string congName;

    Reservation(string name,Date s,Date e);

};