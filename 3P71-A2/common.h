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

struct gene {
	int room_idx;
	int time_idx;
};

// The index of any given gene is the same as the course index, save 4 bits of memory
inline std::vector<gene> chromosome;

// Acceleration structures
inline std::vector<bool> timeslot_taken;
inline std::vector<int> room_taken;
inline std::vector<int> prof_taken;


// Helper functions

std::vector<std::string> split(std::string s, char delim);