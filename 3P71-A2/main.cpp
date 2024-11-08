#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Outline: Use GA to solve class scheduling problem
// 
// 
// Time slots are standardized 0 -> n, where each day is divided into n/5 slots. So a typical day from
//	8 am to 6 pm would have 10 slots, witc thw whole week having 50 slots, no classes on weekends.
// 
// Each room has a capacity, and each course has a number of students. A course can be taught in only one
//	room and each room can only have one course at a time.
// 
// Courses have a duration and so can take up multiple slots.
// 
// Each course has a professor, and each professor can only teach one course at a time. No considerations
//	are made for time to travel between rooms, professors have mastered teleportation.



// This is for pureley display, we do not care what day or hour/minute it is, just the slot.
// All classes only occur once so we dont care about the day.
struct timename {
	string name;
	string time;
	int timeslot_idx;
};

vector<timename> timetable_display;

// Compute structures

vector<string> profs;

struct room {
	int capacity;
	string name;
};

vector<room> rooms;

struct course {
	string name;
	int duration;
	int students;
	int prof; // index of the professor in the profs vector
};

vector<course> courses;

struct timeslot {
	int room;
	int time;
	int course;
};

struct gene {
	int room_idx;
	int time_idx;
};

// The index of any given gene is the same as the course index, save 4 bits of memory
vector<gene> chromosome;

// Acceleration structures
vector<bool> timeslot_taken;
vector<int> room_taken;
vector<int> prof_taken;

// Input to pass to the loader because theres like a million things it needs to return
struct big_input {

};

// load the problem data
void load(big_input output) {
	vector<string> profs_in;

}

int main() {
	big_input bigdata;

}

