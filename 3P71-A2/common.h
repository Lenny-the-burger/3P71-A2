#pragma once

#include <vector>
#include <string>

// These need to be defined basically everywhere in the project this will make
// it easier to do so.

// This is for pureley display, we do not care what day or hour/minute it is, just the slot.
// All classes only occur once so we dont care about the day.
struct timename {
	std::string day;
	std::string time;
	int timeslot_idx;
};

std::vector<timename> timetable_display;

// Compute structures

struct course {
	std::string name;
	int students;
	int prof; // index of the professor in the profs std::vector
	int duration;
};

struct room {
	int capacity;
	std::string name;
};

std::vector<course> courses;
std::vector<room> rooms;
int num_slots;

std::vector<std::string> profs;

struct gene {
	int room_idx;
	int time_idx;
};

// The index of any given gene is the same as the course index, save 4 bits of memory
std::vector<gene> chromosome;

// Acceleration structures
std::vector<bool> timeslot_taken;
std::vector<int> room_taken;
std::vector<int> prof_taken;


// Helper functions

std::vector<std::string> split(std::string s, char delim);