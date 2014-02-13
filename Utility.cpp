#include "Utility.h"
#include "Record.h"


// Do we need to make it a structure?
void print_Record (Record *data_ptr, std::ostream& os)
{
    os << *data_ptr << std::endl;
}
