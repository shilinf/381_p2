#include "Utility.h"


// Do we need to make it a structure?
/*void print_Record (Record *data_ptr, std::ostream& os)
{
    os << *data_ptr << std::endl;
}*/

void swap_int (int &first, int &second)
{
    int temp = first;
    first = second;
    second = temp;
}