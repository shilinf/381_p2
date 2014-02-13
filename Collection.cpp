#include "Collection.h"
#include "Utility.h"
#include "Record.h"
#include <fstream>

using std::cout; using std::endl;

void discard_file_input_remainder(std::ifstream& is);

Collection::Collection(std::ifstream& is, const Ordered_list<Record*, Less_than_ptr<Record*>>& library)
{
    int num_records;
    if (!(is >> name >> num_records))
        throw Error("Invalid data found in file!");
    discard_file_input_remainder(is);
    for (int i = 0; i < num_records; ++i) {
        String title;
        getline(is, title);
        discard_file_input_remainder(is);
        Record probe(title);
        auto find_Record_item_iterator = library.find(&probe);
        if (find_Record_item_iterator == library.end())
            throw Error("Invalid data found in file!");
        add_member(*find_Record_item_iterator);
    }
}

// discard input remaining on the current line
void discard_file_input_remainder(std::ifstream& is)
{
    while (is.get() != '\n')
        ;
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
    os << name << " " << members.size() << endl;
    for (auto record_ptr : members)
        os << record_ptr->get_title() << endl;
}

std::ostream& operator<< (std::ostream& os, const Collection& collection)
{
    os << "Collection " << collection.name << " contains:";
    if (collection.empty())
        os << " None" << endl;
    else {
        os << endl;
        for (auto record_ptr : collection.members)
            os << *record_ptr <<endl;
    }
    return os;
}
