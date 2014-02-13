#include "Collection.h"
#include "Utility.h"
#include "Record.h"
#include <fstream>

using std::cout; using std::endl;

void discard_file_input_remainder(std::ifstream& is);

void save_Record_name(Record* record_ptr, std::ostream& os);

Collection::Collection(std::ifstream& is, const Ordered_list<Record*, Less_than_ptr<Record*>>& library)
{
    int num_records;
    if (!(is >> name >> num_records))
        throw Error("Invalid data found in file!");
    //cout << name << num_records <<"   collection info read" <<endl;
    
    discard_file_input_remainder(is);
    for (int i = 0; i < num_records; ++i) {
        
        //cout << i<<endl;
        
        String title;
        getline(is, title);
        discard_file_input_remainder(is);
        
        //cout << title <<": here is title" <<endl;
        
        Record temp(title);
        auto find_Record_item_iterator = library.find(&temp);
        if (find_Record_item_iterator == library.end()) {
            //cout << "find fail???"<<endl;
    
            throw Error("Invalid data found in file!");
        }
        add_member(*find_Record_item_iterator);
    }
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
    apply_arg_ref(members.begin(), members.end(), save_Record_name, os);


}

void save_Record_name(Record* record_ptr, std::ostream& os)
{
    os << record_ptr->get_title() << endl;
}

std::ostream& operator<< (std::ostream& os, const Collection& collection)
{
    os << "Collection " << collection.name << " contains:";
    if (collection.empty())
        os << " None" << endl;
    else {
        cout << endl;
        apply_arg_ref(collection.members.begin(), collection.members.end(), print_Record, os);
    }
    return os;
}

/* discard input remaining on the current line */
void discard_file_input_remainder(std::ifstream& is)
{
    while (is.get() != '\n') {
        ;
    }
}

