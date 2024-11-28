#pragma once

#include <vector>
#include <string>

// These need to be defined basically everywhere in the project this will make
// it easier to do so.

// shhh i know globals bad but dont feel like making this better and its only globals from like 3 files

// This is for pureley display, we do not care what day or hour/minute it is, just the slot.
// All classes only occur once so we dont care about the day.
struct timename {
	std::string day;
	std::string time;
};

inline std::vector<timename> timetable_display;

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

inline std::vector<course> courses;
inline std::vector<room> rooms;
inline int num_slots;

inline std::vector<std::string> profs;


// Helper functions

std::vector<std::string> split(std::string s, char delim);