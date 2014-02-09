#include "String.h"


using std::string;
using std::cout; using std::endl;


char String::a_null_byte = '\0';
int String::number = 0;
int String::total_allocation = 0;
bool String::messages_wanted = false;

String::String(const char* cstr_) : str_length((int) strlen(cstr_)) // check whether cast work
{
    if (messages_wanted)
        cout << "Ctor: \"" << cstr_ << "\"" << endl;
    if (str_length) {
        str_allocation = str_length + 1;
        str = new char[str_allocation];
        strcpy(str, cstr_);
    }
    else {
        str_allocation = 0;
        str = &a_null_byte;
    }
    ++number;
    total_allocation += str_allocation;
}

String::String(const String& original) : str_length(original.size())
{
    if (messages_wanted)
        cout << "Copy ctor: \""<< original << "\"" << endl;
    if (original.size()) {
        str_allocation = str_length + 1;
        str = new char[str_allocation];
        strcpy(str, original.c_str());
    }
    else {
        str_allocation = 0;
        str = &a_null_byte;
    }
    ++number;
    total_allocation += str_allocation;
}

String::String(String&& original) noexcept : str_length(0), str_allocation(0), str(&a_null_byte)
{
    if (messages_wanted)
        cout << "Move ctor: \"" << original << "\"" << endl;
    swap(original);
}

String::~String() noexcept
{
    if (messages_wanted)
        cout << "Dtor: \""  << str << "\"" << endl;
    --number;
    total_allocation -= str_allocation;
    if (str_allocation)
        delete[] str;
}

String& String::operator= (const String& rhs)
{
    if (messages_wanted)
        cout << "Copy assign from String:  \"" << rhs << "\"" << endl;
    String temp(rhs);
    swap(temp);
    return *this;
}

String& String::operator= (const char* rhs)
{
    if (messages_wanted)
        cout << "Move assign from String:  \"" << rhs << "\"" << endl;
    String temp(rhs);
    swap(temp);
    return *this;
}

String& String::operator= (String&& rhs) noexcept
{
    if (messages_wanted)
        cout << "Assign from C-string:  \"" << rhs << "\"" << endl;
    swap(rhs);
    return *this;
}

char& String::operator[] (int i)
{
    if (i >= 0 && i < str_length) {
        return *(str + i);
    }
    else
        throw String_exception("Subscript out of range");
}

const char& String::operator[] (int i) const
{
    if (i >= 0 && i < str_length) {
        return *(str + i);
    }
    else
        throw String_exception("Subscript out of range");
}

String String::substring(int i, int len) const
{
    if (i >= 0 && len >=0 && i <= str_length && (i + len) <= str_length) {
        return String(*this, i, len);
    }
    else
        throw String_exception("Substring bounds invalid");
}


void String::clear()
{
    String empty_string;
    swap(empty_string);
}

void String::remove(int i, int len)
{
    if (i >= 0 && len >=0 && i <= str_length && (i + len) <= str_length) {
        str_length -= len;
        for (int index = i; index < str_length; index++) {
            str[index] = str[index + len];
        }
        str[str_length] = '\0';
    }
    else
        throw String_exception("Remove bounds invalid");
}

void String::insert_before(int i, const String& src)
{
    if (i <0 || i>str_length)
        throw String_exception("Insertion point out of range");
    
    if (str_allocation >= str_length + src.size() + 1) {
        for (int index = str_length; index >= i; --index)
            str[index + src.size()] = str[index];
        for (int index = 0; index < src.size(); ++index)
            str[index + i] = src[index];
        str_length += src.size();
    }
    else {
        int pre_allocation = str_allocation;
        str_allocation = 2 * (str_length + src.size() + 1);
        total_allocation += str_allocation - pre_allocation;
        char *new_str = new char[str_allocation];
        for (int index = 0; index < i; ++index)
            new_str[index] = str[index];
        for (int index = str_length; index >= i; --index)
            new_str[index + src.size()] = str[index];
        for (int index = 0; index < src.size(); ++index)
            new_str[index + i] = src[index];
        if (pre_allocation)
            delete[] str;
        str_length += src.size();
        str = new_str;
    }
}

String& String::operator += (char rhs)
{
    if (str_allocation >= str_length + 1 + 1) {
        str[str_length] = rhs;
        str_length += 1;
        str[str_length] = '\0';
    }
    else {
        int pre_allocation = str_allocation;
        str_allocation = 2 * (str_length + 1 + 1);
        total_allocation += str_allocation - pre_allocation;
        char *new_str = new char[str_allocation];
        strcpy(new_str, str);
        str[str_length] = rhs;
        str_length += 1;
        str[str_length] = '\0';
        if (pre_allocation)
            delete[] str;
        str = new_str;
    }
    return *this;
}

String& String::operator += (const char* rhs)
{
    if (str_allocation >= str_length + strlen(rhs) + 1) {
        strcpy(str+str_length-1, rhs);
        str_length += strlen(rhs);
    }
    else {
        int pre_allocation = str_allocation;
        str_allocation = 2 * (str_length + strlen(rhs) + 1);
        total_allocation += str_allocation - pre_allocation;
        char *new_str = new char[str_allocation];
        strcpy(new_str, str);
        strcpy(new_str+str_length-1, rhs);
        str_length += strlen(rhs);
        if (pre_allocation)
            delete[] str;
        str = new_str;
    }
    return *this;
}

String& String::operator += (const String& rhs)
{
    insert_before(str_length, rhs);
    return *this;
}


String::String(const String& original, int i, int len) : str_length(len)
{
    if (len) {
        str_allocation = str_length + 1;
        str = new char[str_allocation];
        strncpy(str, original.c_str()+i, len);
        str[len] = '\0';
    }
    else {
        str_allocation = 0;
        str = &a_null_byte;
    }
    ++number;
    total_allocation += str_allocation;
}

void String::swap(String& other) noexcept
{
    std::swap(str, other.str);
    std::swap(str_length, other.str_length);
    std::swap(str_allocation, other.str_allocation);
}



bool operator== (const String& lhs, const String& rhs)
{
    if (!strcmp(lhs.c_str(), rhs.c_str()))
        return true;
    else
        return false;
}

bool operator!= (const String& lhs, const String& rhs)
{
    if (strcmp(lhs.c_str(), rhs.c_str()))
        return true;
    else
        return false;
}

bool operator< (const String& lhs, const String& rhs)
{
    if (strcmp(lhs.c_str(), rhs.c_str()) < 0)
        return true;
    else
        return false;
}


bool operator> (const String& lhs, const String& rhs)
{
    if (strcmp(lhs.c_str(), rhs.c_str()) > 0)
        return true;
    else
        return false;
}

String operator+ (const String& lhs, const String& rhs)
{
    String concatenation(lhs);
    concatenation += rhs;
    return concatenation;
}

std::ostream& operator<< (std::ostream& os, const String& str)
{
    os << str.c_str();
    return os;
}

std::istream& operator>> (std::istream& is, String& str)
{
    char input;
    while (isspace(input = is.get())) {
        if (!is)
            return is;
    }
    str += input;
    while (!isspace(input = is.get())) {
        if (is)
            str += input;
        else
            return is;
    }
    if (is)
        is.putback(input);
    return is;
}

std::istream& getline(std::istream& is, String& str)
{
    char input;
    while ((input = is.get()) != '\n') {
        if (is)
            str += input;
        else
            return is;
    }
    if (is)
        is.putback(input);
    return is;
}



















