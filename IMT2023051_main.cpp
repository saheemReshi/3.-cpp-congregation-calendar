#include "IMT2023051_Assignment3.h"

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
