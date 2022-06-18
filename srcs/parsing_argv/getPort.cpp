#include <iostream>
#include <string>
#include <cctype> // isdigit()
#include <cstdlib> // strtol()
#include <climits> // strtol() out of range
#include <cerrno> // errno & ERANGE

int	getPort(std::string s, unsigned short& port) {
	for (std::string::const_iterator it = s.begin(); it != s.end(); it++) {
		if (!isdigit(*it)) {
			std::cerr << "error: port: wrong value" << std::endl;
			return 1;
		}
	}
	port = strtol(s.c_str(), NULL, 10);
	if (errno == ERANGE) {
		std::cerr << "error: port: out of range" << std::endl;
		return 1;
	}
	return 0;
}
