#include "Collection.h"
#include "Utility.h"
#include <fstream>


Collection::Collection(std::ifstream& is, const Ordered_list<Record*, Less_than_ptr<Record*>>& library)
{
    





}


void Collection::add_member(Record* record_ptr)
{
    if (is_member_present(record_ptr))
        throw Error("Record is already a member in the collection!");
    else
        members.insert(record_ptr);
}

bool Collection::is_member_present(Record* record_ptr) const
{
    return (members.find(record_ptr) == members.end()) ? false : true;
}

void Collection::remove_member(Record* record_ptr)
{
    if (is_member_present(record_ptr))
        members.erase(members.find(record_ptr));
    else
        throw Error("Record is not a member in the collection!");
}

void Collection::save(std::ostream& os) const
{
    


}

std::ostream& operator<< (std::ostream& os, const Collection& collection)
{



    return os;
}



