#include "common.h"

std::vector<std::string> split(std::string s, char delim) {
	std::vector<std::string> out;
	std::string current = "";
	for (char c : s) {
		if (c == delim) {
			out.push_back(current);
			current = "";
		}
		else {
			current += c;
		}
	}
	out.push_back(current);
	return out;
}