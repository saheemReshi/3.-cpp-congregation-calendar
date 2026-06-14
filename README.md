# Congregation calendar in C++

**Design Document & System Overview**

---

# 1. Introduction

This project is designed to manage large-scale congregations such as Games, Concerts, Conferences, and Conventions. The system supports:

* Multiple congregations simultaneously
* Venue management across countries
* Event scheduling with strict time constraints
* Reservation handling between congregations and venues
* Calendar-based event management per venue

The focus is on building a modular, object-oriented system with strong validation rules for dates, times, and scheduling constraints.

---

# 2. Core Classes and System Design

The system is built using multiple interacting classes, each responsible for a specific part of the domain.

---

## 2.1 Date Class

Responsible for handling date representation and validation.

### Attributes

* `int day`
* `int month`
* `int year`

### Responsibilities

* Validate correct calendar dates (including leap years)
* Compare two dates
* Support ordering and range checking

---

## 2.2 Time Class

Handles time representation and validation at 15-minute granularity.

### Attributes

* `int hour`
* `int minute`

### Responsibilities

* Validate time format (00:00 to 23:59)
* Ensure 15-minute alignment (00, 15, 30, 45)
* Compute time differences in minutes
* Support comparison between times

---

## 2.3 Event Struct

Represents an event scheduled at a venue.

### Attributes

* `string eventName`
* `string congregationName`
* `Date date`
* `Time startTime`
* `Time endTime`

### Responsibilities

* Store event metadata
* Support validation of event duration (minimum 30 minutes)
* Assist in detecting scheduling conflicts

---

## 2.4 Reservation Struct

Represents a booking of a venue by a congregation over a date range.

### Attributes

* `Date startDate`
* `Date endDate`
* Linked list of reservation nodes (chronological order)

### Responsibilities

* Maintain reservation intervals
* Support overlap checking
* Track venue allocation to congregations

---

## 2.5 Congregation Class

Manages congregation-level data and their associated reservations.

### Attributes

* `string name`
* `string type`
* `Date startDate`
* `Date endDate`
* `vector<Reservation>` reservedVenues

### Responsibilities

* Add and delete reservations
* Maintain list of venues reserved
* Handle cleanup when congregation is deleted

---

## 2.6 Venue Class

Represents a physical venue where events occur.

### Attributes

* `string name`
* `Location location`
* `int capacity`
* `LinkedList<Event> eventList`
* `vector<Reservation>` reservations

### Responsibilities

* Manage event scheduling
* Ensure no overlapping events in 15-minute slots
* Maintain reservation constraints
* Handle cleanup of events and reservations

---

## 2.7 VenueManager Class

Manages all venues in the system.

### Attributes

* `vector<Venue> venues`

### Responsibilities

* Add / delete venues
* Search venues by location filters
* Handle country-based uniqueness constraints
* Validate deletion constraints (future reservations check)

---

## 2.8 CongregationManager Class

Manages all congregations in the system.

### Attributes

* `vector<Congregation> congregations`

### Responsibilities

* Add / delete congregations
* Display all congregations
* Manage global congregation lookup

---

## 2.9 Calendar Class

Central controller of the system.

### Attributes

* `VenueManager venueManager`
* `CongregationManager congregationManager`

### Responsibilities

* Acts as the main API layer
* Coordinates between venues and congregations
* Handles:

  * Reservations
  * Event scheduling
  * Queries and display operations

---

## 2.10 Decoder Struct

Responsible for parsing user input commands.

### Attributes

* `Calendar calendar`

### Responsibilities

* Parse input commands
* Validate argument formats
* Call appropriate Calendar functions
* Ensure correct routing of operations

---

# 3. Command-Level Functionality

---

## 3.1 Venue Management

* `addVenue`
* `deleteVenue`
* `showVenues`

Supports:

* Location-based filtering
* Country-based uniqueness
* Deletion constraints (no active/future reservations)

---

## 3.2 Congregation Management

* `addCongregation`
* `deleteCongregation`
* `showCongregations`

Supports:

* Unique congregation names
* Date validation
* Cascading deletion of reservations and events

---

## 3.3 Reservation Management

* `reserveVenue`
* `freeVenue`
* `showReserved`

Constraints:

* One congregation per venue per time period
* Must exist before event creation
* Prevents scheduling conflicts

---

## 3.4 Event Management

* `addEvent`
* `deleteEvent`
* `showEvents`
* `showCalendar`

Constraints:

* Must be within valid reservation window
* Minimum 30-minute duration
* 15-minute slot alignment
* No overlapping events
* Multi-day events split into daily entries

---

# 4. Error Handling Strategy

The system uses a centralized status mechanism:

### Status Enum Examples

* `OK`
* `DUPLICATE_VENUE`
* `NO_CONGREGATION`
* `NO_VENUE`
* `TIME_CONFLICT`
* `INVALID_DATE`
* `INVALID_TIME`
* `NO_RESERVATION`

### Output Handling

* `0` → success
* `-1` → failure
* Detailed internal mapping handled via `printCode()`

---

# 5. Design Principles

### 5.1 Encapsulation

Each class handles its own responsibility (Date, Time, Venue, etc.)

### 5.2 Modularity

Separation into managers ensures scalability:

* VenueManager
* CongregationManager

### 5.3 Data Structures

* Vectors for collections
* Linked lists for ordered events/reservations

### 5.4 Strong Validation

* Strict date validation (including leap years)
* Time slot validation (15-minute rule)
* Reservation conflict detection
* Event scheduling constraints

---

# 6. Constraints Summary

* Events must be ≥ 30 minutes
* Must align to 15-minute boundaries
* No overlapping events in same venue slot
* Venue requires 30-minute buffer between events
* Reservations must exist before event creation
* Venue deletion blocked if future reservations exist
* Congregation and venue names must be unique

---

# 7. Assumptions

* Input format is syntactically correct
* No external library-based date validation is used
* Timezone handling is not required
* All string inputs are well-formed and trimmed

---

# 8. UML Overview (Text Representation)

---

## Class Relationships

* `Calendar`

  * contains → `VenueManager`
  * contains → `CongregationManager`

* `VenueManager`

  * manages → `vector<Venue>`

* `CongregationManager`

  * manages → `vector<Congregation>`

* `Venue`

  * contains → `Event (linked list)`
  * contains → `Reservation list`

* `Congregation`

  * contains → `Reservation list`

* `Decoder`

  * uses → `Calendar`

* `Event`

  * depends on → `Date`, `Time`

* `Reservation`

  * depends on → `Date`

---

## Logical Flow

```
Decoder → Calendar → Managers → Entities (Venue / Cong / Event / Reservation)
```

