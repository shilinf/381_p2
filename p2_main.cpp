#include "Ordered_list.h"
#include "Record.h"
#include "Collection.h"
#include "Utility.h"
#include <iostream>
#include <fstream>
#include <new>

using std::cin; using std::cout; using std::endl;
using std::ifstream; using std::ofstream;

// compare two records by ID
struct Compare_Record_ID {
    bool operator() (const Record* p1, const Record* p2) const {return p1->get_ID() < p2->get_ID();}
};

using Ordered_by_title_lib_t = Ordered_list<Record *, Less_than_ptr<Record *>>;
using Ordered_by_id_lib_t = Ordered_list<Record *, Compare_Record_ID>;

// functions to handle command
void find_Record_match_title(const Ordered_by_title_lib_t &library);
void print_Record_match_id(const Ordered_by_id_lib_t &library);
void print_Collection_match_name(const Ordered_list<Collection> &catalog);
void print_Records(const Ordered_by_title_lib_t &library);
void print_Catalog(const Ordered_list<Collection> &catalog);
void print_memory_allocation(int library_size, int catalog_size);
void add_Record(Ordered_by_title_lib_t &library_title, Ordered_by_id_lib_t &library_id);
void add_Collection(Ordered_list<Collection> &catalog);
void add_Record_to_Collection(Ordered_list<Collection> &catalog, const Ordered_by_id_lib_t &library);
void modify_Record_rating(const Ordered_by_id_lib_t &library);
void delete_Record_from_Library(Ordered_by_title_lib_t &library_title, Ordered_by_id_lib_t &library_id, const Ordered_list<Collection> &catalog);
void delete_Collection_from_Catalog(Ordered_list<Collection> &catalog);
void delete_Record_from_Collection(Ordered_list<Collection> &catalog, const Ordered_by_id_lib_t &library);
void clear_Library(Ordered_by_title_lib_t &library_title, Ordered_by_id_lib_t &library_id, const Ordered_list<Collection> &catalog);
void clear_Catalog(Ordered_list<Collection> &catalog);
void clear_all_data(Ordered_by_title_lib_t &library_title, Ordered_by_id_lib_t &library_id, Ordered_list<Collection> &catalog);
void save_all_data(const Ordered_by_title_lib_t &library_title, const Ordered_list<Collection> &catalog);
void restore_all_data(Ordered_by_title_lib_t &library_title, Ordered_by_id_lib_t &library_id, Ordered_list<Collection> &catalog);
void quit(Ordered_by_title_lib_t &library_title, Ordered_by_id_lib_t &library_id, Ordered_list<Collection> &catalog);

// function parameter for OC_apply_if_arg
bool check_record_in_Collection (Collection collection, Record *arg_ptr);

// function parameter for apply_if
bool check_Collection_empty (Collection collection);

// function parameter for apply
void free_Record(Record *record_ptr);


// helper functions
void handle_invalid_command(void);
void discard_input_remainder(void);
void find_collection_iterator(Ordered_list<Collection>::Iterator &catalog_iterator, const Ordered_list<Collection> &catalog);
void read_and_check_integer(int &id_input);
void read_check_title(String &title);
int trim_title(String &title);

int main ()
{
    Ordered_list<Collection> catalog;
    Ordered_by_title_lib_t library_ordered_by_title;
    Ordered_by_id_lib_t library_ordered_by_id;
    while (true) {
        char action, object;
        cout << endl << "Enter command: ";
        cin >> action;
        cin >> object;
        try {
            switch (action) {
                case 'f':
                    switch (object) {
                        case 'r':
                            find_Record_match_title(library_ordered_by_title);
                            break;
                        default:
                            handle_invalid_command();
                            break;
                    }
                    break;
                case 'p':
                    switch (object) {
                        case 'r':
                            print_Record_match_id(library_ordered_by_id);
                            break;
                        case 'c':
                            print_Collection_match_name(catalog);
                            break;
                        case 'L':
                            print_Records(library_ordered_by_title);
                            break;
                        case 'C':
                            print_Catalog(catalog);
                            break;
                        case 'a':
                            print_memory_allocation(library_ordered_by_title.size(), catalog.size());
                            break;
                        default:
                            handle_invalid_command();
                            break;
                    }
                    break;
                case 'a':
                    switch (object) {
                        case 'r':
                            add_Record(library_ordered_by_title, library_ordered_by_id);
                            break;
                        case 'c':
                            add_Collection(catalog);
                            break;
                        case 'm':
                            add_Record_to_Collection(catalog, library_ordered_by_id);
                            break;
                        default:
                            handle_invalid_command();
                            break;
                    }
                    break;
                case 'm':
                    switch (object) {
                        case 'r':
                            modify_Record_rating(library_ordered_by_id);
                            break;
                        default:
                            handle_invalid_command();
                            break;
                    }
                    break;
                case 'd':
                    switch (object) {
                        case 'r':
                            delete_Record_from_Library(library_ordered_by_title, library_ordered_by_id, catalog);
                            break;
                        case 'c':
                            delete_Collection_from_Catalog(catalog);
                            break;
                        case 'm':
                            delete_Record_from_Collection(catalog, library_ordered_by_id);
                            break;
                        default:
                            handle_invalid_command();
                            break;
                    }
                    break;
                case 'c':
                    switch (object) {
                        case 'L':
                            clear_Library(library_ordered_by_title, library_ordered_by_id, catalog);
                            cout <<"All records deleted" <<endl;
                            break;
                        case 'C':
                            clear_Catalog(catalog);
                            cout << "All collections deleted" <<endl;
                            break;
                        case 'A':
                            clear_all_data(library_ordered_by_title, library_ordered_by_id, catalog);
                            cout << "All data deleted" <<endl;
                            break;
                        default:
                            handle_invalid_command();
                            break;
                    }
                    break;
                case 's':
                    switch (object) {
                        case 'A':
                            save_all_data(library_ordered_by_title, catalog);
                            break;
                        default:
                            handle_invalid_command();
                            break;
                    }
                    break;
                case 'r':
                    switch (object) {
                        case 'A':
                            restore_all_data(library_ordered_by_title, library_ordered_by_id, catalog);
                            break;
                        default:
                            handle_invalid_command();
                            break;
                    }
                    break;
                case 'q':
                    switch (object) {
                        case 'q':
                            quit(library_ordered_by_title, library_ordered_by_id, catalog);
                            return 0;
                            break;
                        default:
                            handle_invalid_command();
                            break;
                    }
                default:
                    handle_invalid_command();
                    break;
            }
        } catch (Error& error) {
            cout << error.msg << endl;
            discard_input_remainder();
        } catch (String_exception& exception) {
            cout << exception.msg << endl;
        } catch (...) {
            cout << "Unknown exception caught" <<endl;
            return 1;
        }
    }
    return 0;
}

/* Print error message for invalid comamnd and read to new line */
void handle_invalid_command(void)
{
    cout << "Unrecognized command!" << endl;
    discard_input_remainder();
}

/* Read to new line */
void discard_input_remainder(void)
{
    cin.clear();
    while (cin.get() != '\n')
        ;
}

/* find and print the specified record with the matching title */
void find_Record_match_title(const Ordered_by_title_lib_t &library)
{
    String title;
    read_check_title(title);
    Record probe(title);
    auto library_iterator = library.find(&probe);
    if (library_iterator != library.end())
        cout << **library_iterator << endl;
    else
        throw Error("No record with that title!");
}


/* print the specified record with the matching ID number */
void print_Record_match_id(const Ordered_by_id_lib_t &library)
{
    int id_input;
    read_and_check_integer(id_input);
    Record probe(id_input);
    auto library_iterator = library.find(&probe);
    if (library_iterator != library.end())
        cout << **library_iterator << endl;
    else
        throw Error("No record with that ID!");
}


/* print collection - print each record in the collection with the
 specified name */
void print_Collection_match_name(const Ordered_list<Collection> &catalog)
{
    Ordered_list<Collection>::Iterator catalog_iterator;
    find_collection_iterator(catalog_iterator, catalog);
    cout << *catalog_iterator;
}



/* print all the records in the Library */
void print_Records(const Ordered_by_title_lib_t &library)
{
    if (library.empty())
        cout << "Library is empty" << endl;
    else {
        cout << "Library contains " << library.size() << " records:" <<endl;
        for (auto record_ptr : library)
            cout << *record_ptr <<endl;
    }
}

/* print the Catalog - print all the collections in the Catalog */
void print_Catalog(const Ordered_list<Collection> &catalog)
{
    if (catalog.empty())
        cout << "Catalog is empty" <<endl;
    else {
        cout << "Catalog contains " << catalog.size() << " collections:" <<endl;
        for (auto iterator = catalog.begin(); iterator != catalog.end(); ++iterator)
            cout << *iterator;
    }
}




/* print memory allocations  */
void print_memory_allocation(int library_size, int catalog_size)
{
    cout << "Memory allocations:" <<endl;
    cout << "Records: " << library_size <<endl;
    cout << "Collections: " << catalog_size << endl;
    cout << "Lists: " << g_Ordered_list_count << endl;
    cout << "List Nodes: " << g_Ordered_list_Node_count << endl;
    cout << "Strings: " << String::get_number() << " with " << String::get_total_allocation() << " bytes total" << endl;
}




/* add a record to the Library */
void add_Record(Ordered_by_title_lib_t &library_title, Ordered_by_id_lib_t &library_id)
{
    String medium, title;
    cin >> medium;
    read_check_title(title);
    Record probe(title);
    auto library_iterator = library_title.find(&probe);
    if (library_iterator == library_title.end()) {
        Record *new_record = new Record(medium, title);
        library_title.insert(new_record);
        library_id.insert(new_record);
        cout << "Record " << new_record->get_ID() << " added" <<endl;
    }
    else
        throw Error("Library already has a record with this title!");
}



/* add a collection with the specified name */
void add_Collection(Ordered_list<Collection> &catalog)
{
    String collection_name;
    cin >> collection_name;
    Collection probe(collection_name);
    auto catalog_iterator = catalog.find(probe);
    if (catalog_iterator == catalog.end()) {
        Collection new_collection(collection_name);
        catalog.insert(new_collection);
        cout << "Collection " << collection_name << " added" <<endl;
    }
    else
        throw Error("Catalog already has a collection with this name!");
}



/* add a record to a specified collection */
void add_Record_to_Collection(Ordered_list<Collection> &catalog, const Ordered_by_id_lib_t &library)
{
    Ordered_list<Collection>::Iterator catalog_iterator;
    find_collection_iterator(catalog_iterator, catalog);
    int id_input;
    read_and_check_integer(id_input);
    Record probe(id_input);
    auto library_iterator = library.find(&probe);
    if (library_iterator == library.end())
        throw Error("No record with that ID!");
    catalog_iterator->add_member(*library_iterator);
    cout << "Member " << id_input << " " << (*library_iterator)->get_title() <<" added" << endl;
}

/* modify the rating of the specified record with the matching ID number */
void modify_Record_rating(const Ordered_by_id_lib_t &library)
{
    int id_input, rating_input;
    read_and_check_integer(id_input);
    Record probe(id_input);
    auto library_iterator = library.find(&probe);
    if (library_iterator == library.end())
        throw Error("No record with that ID!");
    read_and_check_integer(rating_input);
    (*library_iterator)->set_rating(rating_input);
    cout << "Rating for record " << id_input << " changed to " << rating_input <<endl;
}


/* delete the specified record from the Library */
void delete_Record_from_Library(Ordered_by_title_lib_t &library_title, Ordered_by_id_lib_t &library_id, const Ordered_list<Collection> &catalog)
{
    String title;
    read_check_title(title);
    Record probe(title);
    auto library_title_iterator = library_title.find(&probe);
    if (library_title_iterator == library_title.end())
        throw Error("No record with that title!");
    if (apply_if_arg(catalog.begin(), catalog.end(), check_record_in_Collection, *library_title_iterator))
        throw Error("Cannot delete a record that is a member of a collection!");
    int record_ID = (*library_title_iterator)->get_ID();
    Record *delete_pointer = * library_title_iterator;
    library_title.erase(library_title_iterator);
    Record probe2(record_ID);
    library_id.erase(library_id.find(&probe2));
    delete delete_pointer;
    cout << "Record " << record_ID << " " << title << " deleted" <<endl;
}

bool check_record_in_Collection (Collection collection, Record *arg_ptr)
{
    return collection.is_member_present(arg_ptr);
}




/* delete the specified collection from the Catalog */
void delete_Collection_from_Catalog(Ordered_list<Collection> &catalog)
{
    Ordered_list<Collection>::Iterator catalog_iterator;
    find_collection_iterator(catalog_iterator, catalog);
    String collection_name = catalog_iterator->get_name();
    catalog.erase(catalog_iterator);
    cout << "Collection " << collection_name << " deleted" << endl;
}

/* delete the specified record as member of the a specified collection */
void delete_Record_from_Collection(Ordered_list<Collection> &catalog, const Ordered_by_id_lib_t &library)
{
    
    // very similar to add record to collection, find way to simplify it
    Ordered_list<Collection>::Iterator catalog_iterator;
    find_collection_iterator(catalog_iterator, catalog);
    int id_input;
    read_and_check_integer(id_input);
    
    // this part should be simplified
    Record probe(id_input);
    auto library_iterator = library.find(&probe);
    
    if (library_iterator == library.end())
        throw Error("No record with that ID!");
    catalog_iterator->remove_member(*library_iterator);
    cout << "Member " << id_input << " " << (*library_iterator)->get_title() <<" deleted" << endl;

}



/* Clear the Library. If collections are not empty, throw exception */
void clear_Library(Ordered_by_title_lib_t &library_title, Ordered_by_id_lib_t &library_id, const Ordered_list<Collection> &catalog)
{
    if (apply_if(catalog.begin(), catalog.end(), check_Collection_empty))
        throw Error("Cannot clear all records unless all collections are empty!");
    apply(library_title.begin(), library_title.end(), free_Record);
    library_title.clear();
    library_id.clear();
    Record::reset_ID_counter();
}

bool check_Collection_empty (Collection collection)
{
    return !collection.empty();
}


/* clear the Catalog: destroy all of the collections in the Catalog,
 and clear the Catalog */
void clear_Catalog(Ordered_list<Collection> &catalog)
{
    catalog.clear();
}


/*  clear all data: first clear the Catalog as in cC,
 then clear the Library as in cL */
void clear_all_data(Ordered_by_title_lib_t &library_title, Ordered_by_id_lib_t &library_id, Ordered_list<Collection> &catalog)
{
    clear_Catalog(catalog);
    clear_Library(library_title, library_id, catalog);
}




/* save all data: write the Library and Catalog data to the named file */
void save_all_data(const Ordered_by_title_lib_t &library_title, const Ordered_list<Collection> &catalog)
{
    String file_name;
    cin >> file_name;
    ofstream output_file(file_name.c_str());
    if (!output_file.is_open())
        throw Error("Could not open file!");
    output_file << library_title.size() << endl;
    for (auto record_ptr : library_title)
        record_ptr->save(output_file);
    output_file << catalog.size() << endl;
    for (auto collection : catalog)
        collection.save(output_file);
    output_file.close();
    cout << "Data saved" <<endl;
}




/* restore all data - restore the Library and Catalog data from the file */
void restore_all_data(Ordered_by_title_lib_t &library_title, Ordered_by_id_lib_t &library_id, Ordered_list<Collection> &catalog)
{
    String file_name;
    cin >> file_name;
    ifstream input_file(file_name.c_str());
    if (!input_file.is_open())
        throw Error("Could not open file!");
    Ordered_list<Collection> local_catalog(catalog);
    Ordered_list<Record *, Less_than_ptr<Record *>> local_library_ordered_by_title(library_title);
    Ordered_list<Record *, Compare_Record_ID> local_library_ordered_by_id(library_id);
    catalog.clear();
    library_id.clear();
    library_title.clear();
    Record::save_ID_counter();
    Record::reset_ID_counter();
    try {
        int num_record;
        if (!(input_file >> num_record))
            throw Error("Invalid data found in file!");
        for (int i=0; i < num_record; i++) {
            Record *new_record = new Record(input_file);
            library_title.insert(new_record);
            library_id.insert(new_record);
        }
        int num_collection;
        if (!(input_file >> num_collection))
            throw Error("Invalid data found in file!");
        for (int i = 0; i < num_collection; i++) {
            Collection new_collection(input_file, library_title);
            catalog.insert(new_collection);
        }
        input_file.close();
        apply(local_library_ordered_by_title.begin(), local_library_ordered_by_title.end(), free_Record);
        cout << "Data loaded" <<endl;
    } catch (Error &error) {
        clear_all_data(library_title, library_id, catalog);
        library_title.swap(local_library_ordered_by_title);
        library_id.swap(local_library_ordered_by_id);
        catalog.swap(local_catalog);
        Record::restore_ID_counter();
        input_file.close();
        throw;
    }
}

void free_Record(Record *record_ptr)
{
    delete record_ptr;
}

/* clear all data (as in cA), and also destroy the Library and Catalog
 containers themselves, so that all memory is deallocated, and then terminate */
void quit(Ordered_by_title_lib_t &library_title, Ordered_by_id_lib_t &library_id, Ordered_list<Collection> &catalog)
{
    clear_all_data(library_title, library_id, catalog);
    cout << "All data deleted" << endl;
    cout << "Done" << endl;
}

/* Find the collection pointer from catalog with the specified collection name.
 Print error message and read to new line if the collection doesn't exist*/
void find_collection_iterator(Ordered_list<Collection>::Iterator &catalog_iterator, const Ordered_list<Collection> &catalog)
{
    String collection_name;
    cin >> collection_name;
    Collection find_temp(collection_name);
    catalog_iterator = catalog.find(find_temp);
    if (catalog_iterator == catalog.end())
        throw Error("No collection with that name!");
}

/* Read in an integer and check whether it succeeds.
 throw exceptoin if read fails */
void read_and_check_integer(int &id_input)
{
    if (!(cin >> id_input))
        throw Error("Could not read an integer value!");
}

/* Read title and check whether its valid.
 return 1 if the title is valie, 0 if not.*/
void read_check_title(String &title)
{
    getline(cin, title);
    if (!trim_title(title))
        throw Error("Could not read a title!");
}

int trim_title(String &title)
{
    int valid = 0, i = 0;
    while (i < title.size()) {
        while (i < title.size() && isspace(title[i]))
            title.remove(i, 1);
        while (i < title.size() && !isspace(title[i])) {
            ++i;
            valid = 1;
        }
        /* If title doesn't touch '\0', add one space to end of current string*/
        if (i < title.size())
            title[i++] = ' ';
    }
    /* If the last character before '\0' is space and there exists non-sapce
     character in the string, put the '\0' one position before to
     occupy the trailing whitespace. */
    if (valid && isspace(title[title.size()-1]))
        title.remove(title.size()-1, 1);
    return valid;
}
