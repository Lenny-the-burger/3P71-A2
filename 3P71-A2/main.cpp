#include <iostream>
#include "common.h"
#include "loader.h"

using namespace std;

// Outline: Use GA to solve class scheduling problem
// 
// 
// Time slots are standardized 0 -> n, where each day is divided into n/5 slots. So a typical day from
//	8 am to 6 pm could have 10 slots, with the whole week having 50 slots, no classes on weekends.
// 
// Each room has a capacity, and each course has a number of students. A course can be taught in only one
//	room and each room can only have one course at a time.
// 
// Courses have a duration and so can take up multiple slots.
// 
// Each course has a professor, and each professor can only teach one course at a time. No considerations
//	are made for time to travel between rooms, professors have mastered teleportation.


int main() {

	cout << "Enter the directory of the data files (def: data/t1):" << "\n" << "> ";
	string input;
	
	// Use getline to accept no imput for default
	getline(cin, input);

	if (input.empty()) {
		input = "data/t1";
	}

	load(input);

}

