#ifndef UTILITY_H
#define UTILITY_H
/* Utility functions, constants, and classes used by more than one other modules */

#include "Record.h"
#include <iostream>

// a simple class for error exceptions - msg points to a C-string error message
struct Error {
	Error(const char* msg_ = "") :
		msg(msg_)
		{}
	const char* msg;
};

/*  *** add any other functions or declarations here and define in Utility.cpp */

void print_Record (Record *data_ptr, std::ostream& os);


#endif
