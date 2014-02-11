#include "Record.h"
#include "Utility.h"
#include <fstream>

using std::endl;

int Record::ID_counter = 0;
int Record::ID_counter_backup = 0;



Record::Record(const String& medium_, const String& title_) : ID(++ID_counter), rate(0), title(title_), medium(medium_) {}


Record::Record(const String& title_) : ID(0), rate(0), title(title_) {}


Record::Record(int ID_) : ID(ID_), rate(0) {}


Record::Record(std::ifstream& is)
{
    if (is >> ID >> medium >> rate >> title)
        throw Error("Invalid data found in file!");
    if (ID > ID_counter)
        ID_counter = ID;
}


void Record::set_rating(int rating_)
{
    if (rating_ >=1 && rating_ <= 5)
        rate = rating_;
    else
        throw Error("Rating is out of range!");
}


void Record::save(std::ostream& os) const
{
    os << ID << " " << medium << " " << rate << " " << title << endl;
}


std::ostream& operator<< (std::ostream& os, const Record& record)
{
    if (record.rate)
        os << record.ID << ": " << record.medium << " " << record.rate << " " << record.title;
    else
        os << record.ID << ": " << record.medium << " u " << record.title;
    return os;
}
