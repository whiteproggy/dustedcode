#ifndef TIMES_H
#define TIMES_H

#include <chrono>
#include <ratio>
#include <iostream>
#include <ios>
#include <iomanip>
#include <string>

inline void print_duration(std::chrono::microseconds duration, const std::string& prefix, std::ostream& out) {
	using namespace std::chrono;
	hours hh = duration_cast<hours>(duration);
	minutes mm = duration_cast<minutes>(duration % hours(1));
	seconds ss = duration_cast<seconds>(duration % minutes(1));
	milliseconds ms = duration_cast<milliseconds>(duration % seconds(1));
	out << prefix << std::fixed << std::setw(2) << std::setprecision(12) << hh.count() << ":" << mm.count() << ":"
		<< ss.count() << ":" << ms.count() << std::endl;
}

#endif // TIMES_H
