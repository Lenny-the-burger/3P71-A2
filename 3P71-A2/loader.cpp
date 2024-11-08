#include "loader.h"

using namespace std;

// load the problem data
// directly populates the global variables
void load(string dir) {
	// Navigate to the given directory and there should be three files:
	// rooms.txt, courses.txt, timeslots.txt
	// - courses contains data in the format: name,professor,students,duration
	// - rooms contains data in the format: name,capacity
	// - timeslots contains data in the format: day,hour
	// Data entries is seperated by newline, the first line is the format and can be ignored.
	// Data member seperated by commas.
	// All data is assumed correctly formatted if it isnt thats on u reader

	// Load the data into the structures
	string prefix = dir + "/";

	// Load the courses
	vector<course> courses;

	ifstream courses_file(prefix + "courses.txt");
	string line;
	getline(courses_file, line); // ignore the first line

	while (getline(courses_file, line)) {

		vector<string> data = split(line, ',');

		course c;
		c.name = data[0];
		c.duration = stoi(data[3]);
		c.students = stoi(data[2]);

		// Check if the professor is already in the list, if not add them, if is get the index
		auto it = find(profs.begin(), profs.end(), data[1]);
		if (it == profs.end()) {
			profs.push_back(data[1]);
			c.prof = profs.size() - 1;
		}
		else {
			c.prof = it - profs.begin();
		}


		courses.push_back(c);

	}


	// Load the rooms
	ifstream rooms_file(prefix + "rooms.txt");
	getline(rooms_file, line); // ignore the first line
	while (getline(rooms_file, line)) {
		vector<string> data = split(line, ',');

		room r;
		r.name = data[0];
		r.capacity = stoi(data[1]);

		rooms.push_back(r);
	}

	// Load the timeslots
	ifstream timeslots_file(prefix + "timeslots.txt");
	getline(timeslots_file, line); // ignore the first line

	num_slots = 0;

	while (getline(timeslots_file, line)) {
		vector<string> data = split(line, ',');

		timename t;
		t.time = data[1];
		t.day = data[0];
		t.timeslot_idx = num_slots;

		num_slots++;

		timetable_display.push_back(t);
	}

}